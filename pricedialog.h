#ifndef PRICEDIALOG_H
#define PRICEDIALOG_H

#include <QDialog>

namespace Ui {
class pricedialog;
}

class pricedialog : public QDialog
{
    Q_OBJECT

public:
    explicit pricedialog(QWidget *parent = nullptr);
    ~pricedialog();

private:
    Ui::pricedialog *ui;
};

#endif // PRICEDIALOG_H
