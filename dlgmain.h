#ifndef DLGMAIN_H
#define DLGMAIN_H

#include <QDialog>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QTableWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QVector>
#include <QTextCodec>

#include "stockparser.h"
#include "dlgaddstock.h"
#include "dlgdetail.h"

namespace Ui {
class DlgMain;
}

class DlgMain : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMain(QWidget *parent = 0);
    ~DlgMain();

private:
    Ui::DlgMain *ui;
    //鼠标移动事件的上一次位置
    QPoint m_lastPos;
    bool m_mousePressed;
    DlgAddStock * m_dlgAddStock;
    DlgDetail * m_dlgDetail;
    QVector<QString> m_stockCodes;
    StockParser m_stockParser;
    QNetworkAccessManager * m_manager;
    QTimer * m_timer;
    QTextCodec * m_codec;

    //读写配置文件
    void loadConfig();
    void saveConfig();
    //根据配置文件设置UI
    void setUi();
    //获取股票内容
    void getStockContent();
    //根据涨跌幅返回stylesheet
    QString getStyleSheet(float value);
protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
private slots:
    //添加股票事件
    void onAddStock(QString code);
    void onTimeout();
    void onManagerFinish(QNetworkReply * reply);

    void on_tbAdd_clicked();
    void on_tbDelete_clicked();
    void on_twStock_doubleClicked(const QModelIndex &index);
};

#endif // DLGMAIN_H
