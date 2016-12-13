#ifndef DLGADDSTOCK_H
#define DLGADDSTOCK_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class DlgAddStock;
}

class DlgAddStock : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAddStock(QWidget *parent = 0);
    ~DlgAddStock();

signals:
    //添加股票代码的信号
    void addStock(QString code);
private:
    Ui::DlgAddStock *ui;
    QPixmap m_pixmap;
protected:
    bool eventFilter(QObject * object, QEvent * event);
    void showEvent(QShowEvent *);
};

#endif // DLGADDSTOCK_H
