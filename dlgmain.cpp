#include <QDebug>
#include "dlgmain.h"
#include "ui_dlgmain.h"

DlgMain::DlgMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMain)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint); // 设置成无边框对话框
    m_mousePressed = false;
    m_manager = new QNetworkAccessManager(this);
    m_timer = new QTimer(this);
}

DlgMain::~DlgMain()
{
    delete m_timer;
    delete m_manager;
    delete ui;
}

void DlgMain::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = true;
        m_lastPos = event->globalPos(); - event->pos();
    }

    QDialog::mousePressEvent(event);
}

void DlgMain::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mousePressed) {
        QPoint pos = event->globalPos();
        QPoint newWindowPos = this->pos() + (pos - m_lastPos);
        this->move(newWindowPos);
        m_lastPos = pos;
    }

    QDialog::mouseMoveEvent(event);
}

void DlgMain::mouseReleaseEvent(QMouseEvent *event)
{
    m_mousePressed = false;
    QDialog::mouseReleaseEvent(event);
}

void DlgMain::on_toolButton_2_clicked()
{
    StockParser parser;
    string s = "var hq_str_sz399001=\"深证成指,10802.176,10812.297,10789.624,10835.911,10766.556,0.000,0.000,16529925277,260334417814.717,0,0.000,0,0.000,0,0.000,0,0.000,0,0.000,0,0.000,0,0.000,0,0.000,0,0.000,0,0.000,2016-12-09,15:05:03,00\"; \
        var hq_str_sh000001=\"上证指数,3209.3395,3215.3658,3232.8834,3244.8013,3207.0433,0,0,204023956,226387156318,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2016-12-09,15:01:03,00\"; \
        var hq_str_sh601766=\"中国中车,10.360,10.400,10.550,10.690,10.340,10.540,10.550,236729833,2492015678.000,236571,10.540,233200,10.530,55500,10.520,78400,10.510,155600,10.500,697100,10.550,398110,10.560,430040,10.570,332254,10.580,322400,10.590,2016-12-09,15:00:00,00\";";
    if (parser.parse(s)) {
        //qDebug() << parser.count() ;
        ui->twStock->setRowCount(parser.count());
        const Stock * pStock = NULL;
        for (size_t i = 0; i < parser.count(); i++) {
            pStock = parser.stocks(i);
            QTableWidgetItem * pItemCode = new QTableWidgetItem(pStock->code.substr(2).c_str());
            QTableWidgetItem * pItemName = new QTableWidgetItem(pStock->datas[INDEX_NAME].c_str());
            QTableWidgetItem * pItemRate = new QTableWidgetItem("");
            QTableWidgetItem * pItemPrice = new QTableWidgetItem(pStock->datas[INDEX_PRICE].c_str());

            ui->twStock->setItem(i, 0, pItemCode);
            ui->twStock->setItem(i, 1, pItemName);
            ui->twStock->setItem(i, 2, pItemRate);
            ui->twStock->setItem(i, 3, pItemPrice);
        }
        //调整窗口高度
        int iHeight = ui->wTitleBar->height() + 10 + ui->wStatusbar->height() + 10 + ui->twStock->rowCount() * ui->twStock->rowHeight(0) + ui->twStock->rowCount() + 1;
        //qDebug() << this->height() << iHeight;
        this->setGeometry(this->x(), this->y(), this->width(), iHeight);
    }
}
