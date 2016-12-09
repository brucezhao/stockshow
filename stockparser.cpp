#include <cstring>
#include "stockparser.h"

const int C_FIELD_COUNT = 33;

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

    string sTemp = trim(str);
    int iLen = sTemp.length();
    if (iLen < 23) {
        //起码是：var hq_str_sh601766=""
        return false;
    }
    if (sTemp.substr(0, 3) != "var") {
        //格式不正确
        return false;
    }

    stock.code = sTemp.substr(11, 8);
    sTemp = sTemp.substr(21, iLen - 22);

    if (stringToVector(sTemp,',', stock.datas) != C_FIELD_COUNT) {
        //字段少了，返回错误
        return false;
    }

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
