/*--------------------------------------------------------------
 * 简单的股票行情软件 v1.0
 * 赵亦平
 * 2016.12.09
 * 行情地址：http://hq.sinajs.cn/list=sh600133
 * 日k线图: http://image.sinajs.cn/newchart/daily/n/sh600133.gif
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
