#ifndef DLGDETAIL_H
#define DLGDETAIL_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QGraphicsScene>
#include <QTableWidgetItem>
#include <QMap>
#include <QPixmap>
#include <QTime>

#include "stockparser.h"

namespace Ui {
class DlgDetail;
}

//分时图更新间隔时间，60秒
const int C_INTERVAL = 60000;

struct PixmapData {
    QPixmap pixmapRealtime;
    QTime lastTime;
};
//下载类型，全部下载、只下载分时图
//enum DownloadType {dtAll, dtRealtime};

class DlgDetail : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDetail(QWidget *parent = 0);
    ~DlgDetail();

    void setStock(const Stock *stock);
private:
    Ui::DlgDetail *ui;
    Stock m_stock;
    QNetworkAccessManager * m_manager;
    QGraphicsScene m_sceneRealtime;
    QGraphicsScene m_sceneDay;
    QMap<QString, PixmapData> m_pixmapRealtimes;
    QMap<QString, QPixmap> m_pixmapDays;

    void setUi();
    void getImages(QString code);//DownloadType dt = dtAll);
    void setItem(QTableWidgetItem * item, int index, bool setColor = false);
    void setItem(QTableWidgetItem *item, float value1, float value2 = 0);
private slots:
    void onManagerFinished(QNetworkReply * reply);

protected:

};

#endif // DLGDETAIL_H
