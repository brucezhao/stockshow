#include "dlgdetail.h"
#include "ui_dlgdetail.h"

DlgDetail::DlgDetail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDetail)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup);
}

DlgDetail::~DlgDetail()
{
    delete ui;
}
