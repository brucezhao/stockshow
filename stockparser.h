/*-------------------------------------------------------
 * 行情解析单元
 * 由于该单元比较独立，所以完全使用stl来写，不用qt类
 *
 * 新浪股票行情返回格式：
 * var hq_str_sh601766="中国中车,10.360,10.400,10.550,10.690,10.340,10.540,10.550,236729833,2492015678.000,236571,10.540,233200,10.530,55500,10.520,78400,10.510,155600,10.500,697100,10.550,398110,10.560,430040,10.570,332254,10.580,322400,10.590,2016-12-09,15:00:00,00";
 * 字段依次为：代码，名称，今开盘，昨收，现价，最高，最低，叫买，叫卖，总手，金额，买1-买5，卖1-卖5，日期，时间
 * -----------------------------------------------------*/
#ifndef STOCKPARSER_H
#define STOCKPARSER_H
#include <string>
#include <vector>

using namespace std;

enum FieldIndex {
    INDEX_HEADER = 0,
    INDEX_NAME,
    INDEX_CODE,
    INDEX_PRICE,
    INDEX_LASTCLOSE,
    INDEX_OPEN,
    INDEX_TOTALCOUNT,
    INDEX_BUY,  // 外盘（买入）
    INDEX_SALE, // 内盘（卖出）
    INDEX_BUY1,
    INDEX_BUYVOLUME1,
    INDEX_BUY2,
    INDEX_BUYVOLUME2,
    INDEX_BUY3,
    INDEX_BUYVOLUME3,
    INDEX_BUY4,
    INDEX_BUYVOLUME4,
    INDEX_BUY5,
    INDEX_BUYVOLUME5,
    INDEX_SALE1,
    INDEX_SALEVOLUME1,
    INDEX_SALE2,
    INDEX_SALEVOLUME2,
    INDEX_SALE3,
    INDEX_SALEVOLUME3,
    INDEX_SALE4,
    INDEX_SALEVOLUME4,
    INDEX_SALE5,
    INDEX_SALEVOLUME5,
    INDEX_LASTDEAL, // 最近逐笔成交
    INDEX_TIME, // 包含日期的时间，20250619140814
    INDEX_INCREASE, // 涨跌
    INDEX_INCREASE_RATE, // 涨跌%
    INDEX_HIGHEST, // 最高
    INDEX_LOWEST, // 最低
    INDEX_PRICE_COUNT_MONEY, // 价格/成交量（手）/成交额
    INDEX_COUNT, // 成交量（手）
    INDEX_MONEY, // 成交量（万）
};

//股票信息结构体
struct Stock {
    string code;
    float increase; //涨幅
    float lastClose;    //昨收盘
    vector<string> datas;
};

class StockParser
{
public:
    StockParser();
    bool parse(const string & str);
    size_t count() {return m_vdatas.size();}
    const Stock * stocks(size_t index) const;
private:
    vector<Stock> m_vdatas;
    vector<string> m_vstockstrs;
    int stringToVector(const string & str, char split, vector<string> & v);
    bool parseStock(const string & str, Stock & stock);
    string trim(const string &str);
};

#endif // STOCKPARSER_H
