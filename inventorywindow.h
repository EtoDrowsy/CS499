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

private:
    Ui::InventoryWindow *ui;
};
#endif // INVENTORYWINDOW_H
