#include "dlgaddstock.h"
#include "ui_dlgaddstock.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

DlgAddStock::DlgAddStock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddStock)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup);
    ui->editCode->installEventFilter(this);

    m_pixmap.load(":/image/frame.png");

    //this->setMask(m_pixmap.mask());
}

DlgAddStock::~DlgAddStock()
{
    delete ui;
}

bool DlgAddStock::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->editCode) {
        if (event->type() == QEvent::FocusOut) {}
//            close();
        else if (event->type() == QEvent::KeyPress) {
            QKeyEvent * keyEvent = dynamic_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return) {
                if (ui->editCode->text().length() != 8) {
                    QMessageBox::warning(this, "提示信息", "股票代码格式不正确，示例：sh000001或者sz399001");
                }
                else {
                    emit addStock(ui->editCode->text());
                    close();
                }
            }
        }
    }

    return false;
    //return QDialog::eventFilter(object, event);
}

void DlgAddStock::showEvent(QShowEvent *)
{
    ui->editCode->setFocus();
    ui->editCode->clear();
}


