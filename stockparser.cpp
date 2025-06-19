#include <cstring>
#include "stockparser.h"

// 字段数量
const int C_FIELD_COUNT = 38;

StockParser::StockParser()
{

}

bool StockParser::parse(const string &str)
{
    m_vstockstrs.clear();
    m_vdatas.clear();
    Stock stock;

    if (stringToVector(str, ';', m_vstockstrs) > 0) {
        for (size_t i = 0; i < m_vstockstrs.size(); i++) {
            if (parseStock(m_vstockstrs.at(i), stock)) {
               m_vdatas.push_back(stock);
            }
        }
        return true;
    }
    else
        return false;
}

const Stock *StockParser::stocks(size_t index) const
{
    if (index >= m_vdatas.size())
        return NULL;
    else
        return &m_vdatas.at(index);
}

int StockParser::stringToVector(const string & str, char split, vector<string> & v)
{
    v.clear();

    if (str.empty())
        return 0;

    char * pHeader = (char *)str.c_str();
    char * pTail = pHeader + str.length();

    for (char *p = pHeader; p != pTail; p ++) {
        if (*p == split) {
            *p = '\0';
            v.push_back(pHeader);
            *p = split;
            pHeader = p + 1;
            while (*pHeader == ' ' || *pHeader == '\n') {
                pHeader ++;
            }
        }
    }
    if (*pHeader != '\0') {
        v.push_back(pHeader);
    }

    return v.size();
}


bool StockParser::parseStock(const string &str, Stock &stock)
{
    //初始化
    stock.code = "";
    stock.datas.clear();

    int iLen = str.length();
    if (iLen < 11) {
        //起码是：v_sz300599=
        return false;
    }
    if (str.substr(0, 2) != "v_") {
        //格式不正确
        return false;
    }

    if (stringToVector(str,'~', stock.datas) < C_FIELD_COUNT) {
        //字段少了，返回错误
        return false;
    }

    // 从内容头部取加了sh/sz的代码
    string sCode = stock.datas[INDEX_HEADER];

    stock.code = sCode.substr(2, 8); // stock.datas[INDEX_CODE];
    //计算涨幅
    stock.increase = std::atof(stock.datas[INDEX_INCREASE_RATE].c_str());
    stock.lastClose = std::atof(stock.datas[INDEX_LASTCLOSE].c_str());;

    return true;
}

string StockParser::trim(const string &str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos) {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos) {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}
