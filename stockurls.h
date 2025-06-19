#ifndef STOCKURLS_H
#define STOCKURLS_H
#include <QString>

// 行情地址: https://qt.gtimg.cn/q=sz300599,sz399001
const QString C_URL_REALTIME = "https://qt.gtimg.cn/q=%1";
// 分时图: https://image.sinajs.cn/newchart/min/n/sz300599.gif?1749781312769
const QString C_URL_GIF_MIN = "https://image.sinajs.cn/newchart/min/n/%1.gif?%2";
// 日K线图：http://image.sinajs.cn/newchart/daily/n/sh601006.gif?1749781312769
const QString C_URL_GIF_DAY = "http://image.sinajs.cn/newchart/daily/n/%1.gif?%2";

#endif // STOCKURLS_H
