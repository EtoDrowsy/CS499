#ifndef INVENTORYWINDOW_H
#define INVENTORYWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class InventoryWindow;
}
QT_END_NAMESPACE

class InventoryWindow : public QMainWindow
{
    Q_OBJECT

public:
    InventoryWindow(QWidget *parent = nullptr);
    ~InventoryWindow();

private slots:
    void on_csvLoadButton_clicked();

    void on_addObjectButton_clicked();

    void on_deleteObjectButton_clicked();

    void writeCSV(const std::string &filePath);

    bool deleteRowId(int idDelete);

    void on_createCSVButton_clicked();

    void on_sortObjectButton_clicked();

    void on_pushButton_clicked();

    void on_HTMLGenButton_clicked();

private:
    Ui::InventoryWindow *ui;

    bool csvLoaded = false;
};
#endif // INVENTORYWINDOW_H
