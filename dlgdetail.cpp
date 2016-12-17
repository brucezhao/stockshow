#include <QUrl>
#include <QDateTime>
#include <QNetworkRequest>
#include <QTableWidget>
#include <QBrush>

#include <QDebug>

#include "dlgdetail.h"
#include "ui_dlgdetail.h"

const QString C_URLREALTIME = "http://img.gtimg.cn/images/hq_parts/hushen/stocks/%1.png?%2";
const QString C_URLDAY = "http://img.gtimg.cn/images/kline/hushen/stocks/day/%1.png?%2";

DlgDetail::DlgDetail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDetail)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);//Qt::Popup);

    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &DlgDetail::onManagerFinished);

    ui->gvRealtime->setScene(&m_sceneRealtime);
    ui->gvDay->setScene(&m_sceneDay);

    ui->twBuy->setColumnWidth(0, 50);
    ui->twSale->setColumnWidth(0, 50);
    ui->twInfo->setColumnWidth(0, 40);
    ui->twInfo->setColumnWidth(1, 70);
    ui->twInfo->setColumnWidth(2, 40);
}

DlgDetail::~DlgDetail()
{
    delete m_manager;
    delete ui;
}

void DlgDetail::setStock(const Stock *stock)
{
    //if (stock->code != m_stock.code) {
        m_stock = *stock;
        setUi();
        getImages(m_stock.code.substr(2).c_str());
    //}
}

void DlgDetail::setUi()
{
    ui->lbName->setText(m_stock.datas[INDEX_NAME].c_str());

    //卖五
    setItem(ui->twSale->item(0, 1), INDEX_SALE5, true);
    setItem(ui->twSale->item(0, 2), INDEX_SALEVOLUME5);

    setItem(ui->twSale->item(1, 1), INDEX_SALE4, true);
    setItem(ui->twSale->item(1, 2), INDEX_SALEVOLUME4);

    setItem(ui->twSale->item(2, 1), INDEX_SALE3, true);
    setItem(ui->twSale->item(2, 2), INDEX_SALEVOLUME3);

    setItem(ui->twSale->item(3, 1), INDEX_SALE2, true);
    setItem(ui->twSale->item(3, 2), INDEX_SALEVOLUME2);

    setItem(ui->twSale->item(4, 1), INDEX_SALE1, true);
    setItem(ui->twSale->item(4, 2), INDEX_SALEVOLUME1);

    //买五
    setItem(ui->twBuy->item(0, 1), INDEX_BUY1, true);
    setItem(ui->twBuy->item(0, 2), INDEX_BUYVOLUME1);

    setItem(ui->twBuy->item(1, 1), INDEX_BUY2, true);
    setItem(ui->twBuy->item(1, 2), INDEX_BUYVOLUME2);

    setItem(ui->twBuy->item(2, 1), INDEX_BUY3, true);
    setItem(ui->twBuy->item(2, 2), INDEX_BUYVOLUME3);

    setItem(ui->twBuy->item(3, 1), INDEX_BUY4, true);
    setItem(ui->twBuy->item(3, 2), INDEX_BUYVOLUME4);

    setItem(ui->twBuy->item(4, 1), INDEX_BUY5, true);
    setItem(ui->twBuy->item(4, 2), INDEX_BUYVOLUME5);

    //其它信息
    setItem(ui->twInfo->item(0,1), INDEX_PRICE, true);
    setItem(ui->twInfo->item(0, 3), INDEX_OPEN, true);
    setItem(ui->twInfo->item(1, 1), INDEX_TOP, true);
    setItem(ui->twInfo->item(1, 3), INDEX_LOWEST, true);

    QString sText;
    float f;

    //金额
    sText = m_stock.datas[INDEX_TOTALMONDY].c_str();
    f = sText.toFloat();
    f /= 100000000;
    sText.setNum(f, 'f', 2);
    sText += "亿";
    ui->twInfo->item(2, 3)->setText(sText);
    //总手
    sText = m_stock.datas[INDEX_TOTALCOUNT].c_str();
    f = sText.toFloat();
    f /= 1000000;
    sText.setNum(f, 'f', 2);
    sText += "万";
    ui->twInfo->item(3, 3)->setText(sText);

    //涨跌
    sText = m_stock.datas[INDEX_PRICE].c_str();
    f = sText.toFloat();
    setItem(ui->twInfo->item(2, 1), f - m_stock.lastClose);
    //涨幅
    setItem(ui->twInfo->item(3, 1), m_stock.increase);
    //涨停
    f = m_stock.lastClose * 1.1;
    setItem(ui->twInfo->item(4, 1), f, m_stock.lastClose);
    //跌停
    f = m_stock.lastClose * 0.9;
    setItem(ui->twInfo->item(4, 3), f, m_stock.lastClose);
}

void DlgDetail::getImages(QString code) //DownloadType dt)
{
    QString sUrl;

    //判断是否存在历史纪录
    QMap<QString, PixmapData>::iterator it = m_pixmapRealtimes.find(code);

    if ((it == m_pixmapRealtimes.end()) || (it->lastTime.elapsed() > C_INTERVAL)) {
        //如果没有历史纪录，或者历史纪录已经超时，则重新下载
        //分时图：http://img.gtimg.cn/images/hq_parts/hushen/stocks/600380.png?0.995922569739595
        sUrl = QString(C_URLREALTIME).arg(code).arg(QDateTime::currentMSecsSinceEpoch());

        QUrl urlRealtime(sUrl);
        QNetworkRequest requestRealtime(urlRealtime);
        requestRealtime.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
        QNetworkReply * replyRealtime = m_manager->get(requestRealtime);
        replyRealtime->setProperty("id", "realtime");
        replyRealtime->setProperty("code", code);
    }
    else {
        m_sceneRealtime.addPixmap(it->pixmapRealtime);
    }

    //判断日K图是否存在历史纪录
    QMap<QString, QPixmap>::iterator itDay = m_pixmapDays.find(code);
    if (itDay == m_pixmapDays.end()) {
        //日K图：http://img.gtimg.cn/images/kline/hushen/stocks/day/600380.png?0.7182225310801492
        sUrl = QString(C_URLDAY).arg(code).arg(QDateTime::currentMSecsSinceEpoch());

        QUrl urlDay(sUrl);
        QNetworkRequest requestDay(urlDay);
        requestDay.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
        QNetworkReply * replyDay = m_manager->get(requestDay);
        replyDay->setProperty("id", "day");
        replyDay->setProperty("code", code);
    }
    else {
//        qDebug() << "无需重新下载日K图";
        m_sceneDay.addPixmap(*itDay);
    }
}

void DlgDetail::setItem(QTableWidgetItem *item, int index, bool setColor)
{
    QString s = m_stock.datas[index].c_str();
    item->setText(s);
    if (setColor) {
        QBrush brush;
        float fs = s.toFloat();
        if (fs > m_stock.lastClose) brush = QBrush(Qt::red);
        else if (fs < m_stock.lastClose) brush = QBrush(QColor(85,170,0));
        else brush = QBrush(Qt::black);
        item->setForeground(brush);
    }
}

void DlgDetail::setItem(QTableWidgetItem *item, float value1, float value2)
{
    QBrush brush;
    float value = value1 - value2;

    if (value > 0) brush = QBrush(Qt::red);
    else if (value < 0) brush = QBrush(QColor(85,170,0));
    else brush = QBrush(Qt::black);
    item->setForeground(brush);
    QString sText;
    sText.setNum(value1, 'f', 2);
    item->setText(sText);
}

void DlgDetail::onManagerFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray arr = reply->readAll();
        if (!arr.isEmpty()) {
            QPixmap pixmap;
            pixmap.loadFromData(arr);
            QString sId = reply->property("id").toString();
            QString sCode = reply->property("code").toString();

            if (sId == "realtime") {
                m_sceneRealtime.addPixmap(pixmap);

                QMap<QString, PixmapData>::iterator it = m_pixmapRealtimes.find(sCode);
                if (it != m_pixmapRealtimes.end()) {
                    it->pixmapRealtime = pixmap;
                    it->lastTime.restart();
                }
                else {
                    PixmapData data;
                    data.lastTime.start();
                    data.pixmapRealtime = pixmap;
                    m_pixmapRealtimes.insert(sCode,data);
                }
            }
            else if (sId == "day") {
                m_sceneDay.addPixmap(pixmap);
                m_pixmapDays.insert(sCode, pixmap);
            }
        }
    }
}

