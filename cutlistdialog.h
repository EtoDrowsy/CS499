#ifndef CUTLISTDIALOG_H
#define CUTLISTDIALOG_H

#include <QDialog>
#include "Lumber.cpp"

namespace Ui {
class cutlistdialog;
}

class cutlistdialog : public QDialog
{
    Q_OBJECT

public:
    explicit cutlistdialog(std::vector<Lumber*> invvec, QWidget *parent = nullptr);
    ~cutlistdialog();

private slots:
    void on_rowsetbutton_clicked();

    void on_genmaterials_clicked();

    std::vector<Lumber*> getInventory();

    std::vector<std::vector<int>> getReturnVector();

    void setReturnVector(std::vector<std::vector<int>> retVec);

private:
    Ui::cutlistdialog *ui;
    std::vector<Lumber*> inventory;
    std::vector<std::vector<int>> returnVector;
};

#endif // CUTLISTDIALOG_H
