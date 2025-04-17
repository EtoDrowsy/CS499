#ifndef SOLDINVENTORYDIALOG_H
#define SOLDINVENTORYDIALOG_H

#include <QDialog>

namespace Ui {
class soldinventorydialog;
}

class soldinventorydialog : public QDialog
{
    Q_OBJECT

public:
    explicit soldinventorydialog(QWidget *parent = nullptr, int id = NULL);
    ~soldinventorydialog();
    bool getQuantityCheckState();
    int getQuantitySold();
    std::string getNoteString();

private:
    Ui::soldinventorydialog *ui;
    int saleID;
};

#endif // SOLDINVENTORYDIALOG_H
