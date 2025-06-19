#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QMap>
#include <QJsonArray>
#include <QUrl>
#include <QNetworkRequest>
#include <QStringConverter>

#include "dlgmain.h"
#include "ui_dlgmain.h"
#include "stockurls.h"
#include "encode.h"


DlgMain::DlgMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMain)
{
    ui->setupUi(this);

    //设置成无边框对话框
    this->setWindowFlags(Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);    //Qt::WindowSystemMenuHint |
    m_mousePressed = false;
    m_manager = new QNetworkAccessManager(this);
    m_dlgAddStock = NULL;
    m_dlgDetail = NULL;

    //第0列为隐藏列
    // ui->twStock->setColumnWidth(0, 1);
    loadConfig();
    //setUi();

    m_timer = new QTimer(this);
    m_timer->setInterval(5000);
    connect(m_timer, &QTimer::timeout, this, &DlgMain::onTimeout);
    m_timer->start();

    //设置trayicon
    m_trayIconMenu = new QMenu(this);
    m_actionShow = m_trayIconMenu->addAction("显示");
    m_actionShow->setFont(this->font());
    m_actionShow->setIcon(QIcon(":/image/show_24.png"));
    connect(m_actionShow, &QAction::triggered, this, &DlgMain::onShowTriggered);
    m_actionAbout =  m_trayIconMenu->addAction("关于");
    m_actionAbout->setFont(this->font());
    m_actionAbout->setIcon(QIcon(":/image/about_24.png"));
    connect(m_actionAbout, &QAction::triggered, this, &DlgMain::onAboutTriggered);
    m_trayIconMenu->addSeparator();
    m_actionQuit = m_trayIconMenu->addAction("退出");
    m_actionQuit->setFont(this->font());
    m_actionQuit->setIcon(QIcon(":/image/exit_24.png"));
    connect(m_actionQuit, &QAction::triggered, this, &DlgMain::onQuitTriggered);

    QIcon icon(":/image/icon_24.png");
    m_trayIcon = new QSystemTrayIcon(icon, this);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &DlgMain::OnSystemTrayIconActivated);
    m_trayIcon->setToolTip("股票秀");
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->show();

    getStockContent();

 }

DlgMain::~DlgMain()
{
    saveConfig();

    delete m_trayIcon;
    delete m_trayIconMenu;
    if (m_dlgAddStock != NULL)
        delete m_dlgAddStock;
    if (m_dlgDetail != NULL)
        delete m_dlgDetail;
    delete m_timer;
    delete m_manager;
    delete ui;
}

void DlgMain::loadConfig()
{
    QString sFileName = qApp->applicationFilePath() + ".json";
    QFile file(sFileName);
    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        QByteArray arr = file.readAll();
        file.close();

        if (!arr.isEmpty()) {
            QJsonParseError jsonError;
            QJsonDocument doc = QJsonDocument::fromJson(arr, &jsonError);
            if (jsonError.error == QJsonParseError::NoError && doc.isObject()) {
                QJsonObject jsonConfig = doc.object();
                //获取上次窗口的位置
                if (jsonConfig.contains("position")) {
                    QJsonObject jsonPos = jsonConfig.take("position").toObject();
                    int x = jsonPos.take("x").toInt();
                    int y = jsonPos.take("y").toInt();
                    int w =  jsonPos.take("width").toInt();
                    int h = jsonPos.take("heigth").toInt();
                    this->setGeometry(x, y, w, h);
                }
                //获取股票代码列表
                m_stockCodes.clear();
                if (jsonConfig.contains("codes")) {
                    QJsonArray jsonCodes = jsonConfig.take("codes").toArray();
                    for (int i = 0; i < jsonCodes.count(); i++) {
                        m_stockCodes.push_back(jsonCodes[i].toString());
                    }
                }
            }
        }
    }
}

void DlgMain::saveConfig()
{
    QString sFileName = qApp->applicationFilePath() + ".json";
    QJsonObject jsonConfig;
    QJsonObject jsonPos;
    QJsonArray jsonCodes;

    jsonPos.insert("x", this->x());
    jsonPos.insert("y", this->y());
    jsonPos.insert("width", this->width());
    jsonPos.insert("height", this->height());
    jsonConfig.insert("position", jsonPos);

    if (m_stockParser.count() > 2) {
        //用m_stockParser而不是m_stockCodes，是因为m_stockCodes中可能包含了不正确的股票代码
        for (size_t i = 2 ; i < m_stockParser.count(); i++) {
            jsonCodes.append(m_stockParser.stocks(i)->code.c_str());
        }
    }
    else {
        //如果stockParser没有股票，则以m_stockCodes里的为准，这是因为如果没有网络连接，导致m_stockParser为空
        for (int i = 0; i < m_stockCodes.count(); i++) {
            jsonCodes.append(m_stockCodes.at(i));
        }
    }
    jsonConfig.insert("codes", jsonCodes);

    QJsonDocument doc;
    doc.setObject(jsonConfig);
    QByteArray arr = doc.toJson();

    //写入文件
    QFile file(sFileName);
    file.open(QIODevice::WriteOnly);
    file.write(arr);
    file.close();
}

void DlgMain::setUi()
{
    int iCount = m_stockParser.count();
    ui->twStock->clearContents();
    ui->twStock->setRowCount(iCount - 2);   //去掉指数的两条记录
    // ui->twStock->setColumnWidth(0, 0);

    //最少应该取回两条大盘指数
    if (iCount < 2) return;

    const Stock * pStock = NULL;
    QString sText, sPrice;
    QBrush brush;

    //取大盘
    pStock = m_stockParser.stocks(0);
    sText.setNum(pStock->increase, 'f', 2);
    sPrice = pStock->datas[INDEX_PRICE].c_str();
    // sPrice.resize(sPrice.length() - 2);
    sText = QString("%1  %2%").arg(sPrice).arg(sText);
    ui->lbSHIndex->setText(sText);
    ui->lbSHIndex->setStyleSheet(getStyleSheet(pStock->increase));

    pStock = m_stockParser.stocks(1);
    sText.setNum(pStock->increase, 'f', 2);
    sPrice = pStock->datas[INDEX_PRICE].c_str();
    // sPrice.resize(sPrice.length() - 1);
    sText = QString("%1  %2%").arg(sPrice).arg(sText);
    ui->lbSZIndex->setText(sText);
    ui->lbSZIndex->setStyleSheet(getStyleSheet(pStock->increase));

    //取各股
    for (size_t i = 2; i < m_stockParser.count(); i++) {
        pStock = m_stockParser.stocks(i);
        if (pStock->increase > 0)
            brush = QBrush(QColor(Qt::red));
        else if (pStock->increase < 0)
            brush = QBrush(QColor(85,170,0));
        else
            brush = QBrush(QColor(Qt::black));

        // QTableWidgetItem * pItemCodeEx = new QTableWidgetItem(pStock->code.c_str());
        QTableWidgetItem * pItemCode = new QTableWidgetItem(pStock->datas[INDEX_CODE].c_str());
        QTableWidgetItem * pItemName = new QTableWidgetItem(pStock->datas[INDEX_NAME].c_str());
        QTableWidgetItem * pItemRate = NULL;
        QTableWidgetItem * pItemPrice = NULL;
        sText = pStock->datas[INDEX_PRICE].c_str();
        if (sText == "0.000") {
            pItemRate = new QTableWidgetItem("-.--");
            pItemPrice = new QTableWidgetItem("-.--");
        }
        else {
            sText.resize(sText.length() - 1);
            pItemPrice = new QTableWidgetItem(sText);
            pItemPrice->setForeground(brush);

            sText.setNum(pStock->increase, 'f', 2);
            pItemRate = new QTableWidgetItem(sText + "%");
            pItemRate->setForeground(brush);
        }
        pItemRate->setTextAlignment(Qt::AlignRight);
        pItemPrice->setTextAlignment(Qt::AlignRight);

        // ui->twStock->setItem(i - 2, 0, pItemCodeEx);
        ui->twStock->setItem(i - 2, 0, pItemCode);
        ui->twStock->setItem(i - 2, 1, pItemName);
        ui->twStock->setItem(i - 2, 2, pItemRate);
        ui->twStock->setItem(i - 2, 3, pItemPrice);
    }
    //调整窗口高度
    int iHeight = ui->wTitleBar->height() + 10 + ui->wStatusbar->height() + 10 + ui->twStock->rowCount() * ui->twStock->rowHeight(0) + 8;
    this->setGeometry(this->x(), this->y(), this->width(), iHeight);
    // this->setGeometry(this->x(), this->y(), 500, 300);
}

void DlgMain::getStockContent()
{
    QString sCode = "sh000001,sz399001";
    QString sUrl = QString(C_URL_REALTIME).arg(sCode);
    for (int i = 0 ; i < m_stockCodes.count(); i++) {
        sUrl += "," + m_stockCodes.at(i);
    }
    QUrl url(sUrl);
    QNetworkRequest request(url);
    // request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    connect(m_manager, &QNetworkAccessManager::finished, this, &DlgMain::onManagerFinish);

    m_manager->get(request);
}

QString DlgMain::getStyleSheet(float value)
{
    if (value < 0) {
        return "color:green";
    }
    else if (value > 0) {
        return "color:red";
    }
    else {
        return "color:black";
    }
}

void DlgMain::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = true;
        m_lastPos = event->globalPosition().toPoint() -  frameGeometry().topLeft();
    }

    QDialog::mousePressEvent(event);
}

void DlgMain::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mousePressed) {
        move(event->globalPosition().toPoint() - m_lastPos);
    }

    QDialog::mouseMoveEvent(event);
}

void DlgMain::mouseReleaseEvent(QMouseEvent *event)
{
    m_mousePressed = false;
    QDialog::mouseReleaseEvent(event);
}

void DlgMain::reject()
{
    if (m_dlgDetail->isVisible())
        m_dlgDetail->hide();
    else
        this->hide();
}

void DlgMain::onAddStock(QString code)
{
    if (m_stockCodes.contains(code)) {
        QMessageBox::warning(this, "提示", "已经存在该股票，无需再次添加。");
    }
    else {
        m_stockCodes.push_back(code);
        // getStockContent();
    }
}

void DlgMain::onTimeout()
{
    getStockContent();
}

void DlgMain::onManagerFinish(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();
        //网站内容是GB18030,要转换成Unicode
        std::string gbkStr = std::string(bytes);
        // std::wstring unicodeStr = ;
        string utf8Str = Encode::Convert("GBK", "UTF-8", gbkStr);
        QString sContent = QString::fromStdString(utf8Str);
        qDebug() << sContent;
        if (!sContent.isEmpty()) {
            if (m_stockParser.parse(sContent.toStdString())) {
                setUi();

                if (m_dlgDetail && m_dlgDetail->isVisible()) {
                    for (size_t i = 0; i < m_stockParser.count(); i++) {
                        if (m_currentStock == m_stockParser.stocks(i)->code) {
                            m_dlgDetail->setStock(m_stockParser.stocks(i));
                        }
                    }
                }
            }
        }
    } else {
        qDebug() << "reply error:" << reply->error();
    }
}

void DlgMain::OnSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    //qDebug() << reason;
    switch (reason) {
    case QSystemTrayIcon::Trigger :
        this->setVisible(!this->isVisible());
        break;
    }
}

void DlgMain::onShowTriggered(bool)
{
    show();
}

void DlgMain::onAboutTriggered(bool)
{
    QMessageBox::about(this, "股票秀", "作者：山贼(赵亦平)\n日期：2016年12月13日\n邮箱：brucezhao@163.com");
}

void DlgMain::onQuitTriggered(bool)
{
    saveConfig();
    close();
}

void DlgMain::on_tbAdd_clicked()
{
    if (m_dlgAddStock == NULL) {
        m_dlgAddStock = new DlgAddStock();
        connect(m_dlgAddStock, &DlgAddStock::addStock, this, &DlgMain::onAddStock);
    }
    QToolButton * btn = dynamic_cast<QToolButton *>(sender());
    QPoint point = btn->mapToGlobal(QPoint(0,0));
    m_dlgAddStock->setGeometry(point.x(), point.y() + btn->height() + 1, m_dlgAddStock->width(), m_dlgAddStock->height());

    m_dlgAddStock->show();
}

void DlgMain::on_tbDelete_clicked()
{
    int iRow = ui->twStock->currentIndex().row();
    if (iRow < 0) {
        QMessageBox::warning(this, "提示", "请选择要删除的股票。");
    }
    else {
        QTableWidgetItem * pItem = ui->twStock->item(iRow, 0);
        QString sCode = pItem->text();
        int iIndex = m_stockCodes.indexOf(sCode);
        if (iIndex >= 0) {
            m_stockCodes.remove(iIndex);
        }
    }
    getStockContent();
}

void DlgMain::on_twStock_doubleClicked(const QModelIndex &index)
{
    if (m_dlgDetail == NULL) {
        m_dlgDetail = new DlgDetail();
    }
    const Stock * pStock = m_stockParser.stocks(index.row() + 2);
    m_currentStock = pStock->code;
    m_dlgDetail->setStock(pStock);
    //确定dlg的位置
    int x = this->x() - m_dlgDetail->width() - 1;
    if (x < 0) x = this->x() + this->width() + 1;
    m_dlgDetail->setGeometry(x, this->y(), m_dlgDetail->width(), m_dlgDetail->height());
    m_dlgDetail->show();
}


void DlgMain::on_tbClose_clicked()
{
    this->hide();
}
