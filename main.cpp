/*--------------------------------------------------------------
 * 简单的股票行情软件 v1.0
 * 赵亦平
 * 2016.12.09
 * 行情地址：http://hq.sinajs.cn/list=sh600133
 * 日k线图: http://image.sinajs.cn/newchart/daily/n/sh600133.gif
 * 分时图：http://img.gtimg.cn/images/hq_parts/hushen/stocks/600380.png?0.995922569739595
 * 日K线图：http://img.gtimg.cn/images/kline/hushen/stocks/day/600380.png?0.7182225310801492
 *--------------------------------------------------------------
 */
#include "dlgmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DlgMain w;
    w.show();

    return a.exec();
}
