#ifndef DLGDETAIL_H
#define DLGDETAIL_H

#include <QDialog>

namespace Ui {
class DlgDetail;
}

class DlgDetail : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDetail(QWidget *parent = 0);
    ~DlgDetail();

private:
    Ui::DlgDetail *ui;
};

#endif // DLGDETAIL_H
