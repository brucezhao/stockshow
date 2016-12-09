#include "dlgmain.h"
#include "ui_dlgmain.h"

DlgMain::DlgMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMain)
{
    ui->setupUi(this);
}

DlgMain::~DlgMain()
{
    delete ui;
}
