#ifndef DLGMAIN_H
#define DLGMAIN_H

#include <QDialog>

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
};

#endif // DLGMAIN_H
