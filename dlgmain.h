#ifndef DLGMAIN_H
#define DLGMAIN_H

#include <QDialog>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QTableWidget>

#include "stockparser.h"

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

    QNetworkAccessManager * m_manager;
    QTimer * m_timer;
protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
private slots:
    void on_toolButton_2_clicked();
};

#endif // DLGMAIN_H
