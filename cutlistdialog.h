#ifndef CUTLISTDIALOG_H
#define CUTLISTDIALOG_H

#include <QDialog>

namespace Ui {
class cutlistdialog;
}

class cutlistdialog : public QDialog
{
    Q_OBJECT

public:
    explicit cutlistdialog(QWidget *parent = nullptr);
    ~cutlistdialog();

private slots:
    void on_rowsetbutton_clicked();

private:
    Ui::cutlistdialog *ui;
};

#endif // CUTLISTDIALOG_H
