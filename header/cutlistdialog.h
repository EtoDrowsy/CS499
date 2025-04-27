#ifndef CUTLISTDIALOG_H
#define CUTLISTDIALOG_H

#include <QDialog>
#include "../source/Lumber.cpp"
#include "../source/CutList.cpp"

namespace Ui {
class cutlistdialog;
}

class cutlistdialog : public QDialog
{
    Q_OBJECT

public:
    explicit cutlistdialog(std::vector<Lumber*> invvec, QWidget *parent = nullptr);
    ~cutlistdialog();

    confirmer getInventoryUpdate();

private slots:
    void on_genmaterials_clicked();

    std::vector<Lumber*> getInventory();

    void on_addbutton_clicked();

    void on_confirmButton_clicked();

private:
    Ui::cutlistdialog *ui;
    std::vector<Lumber*> inventory;
    std::vector<std::vector<int>> cutIDVector;
    confirmer inventoryUpdate;
    std::vector<CutListItem*> cutList;
    bool confirm;
};

#endif // CUTLISTDIALOG_H
