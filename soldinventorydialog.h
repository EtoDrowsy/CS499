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
    explicit soldinventorydialog(QWidget *parent = nullptr);
    ~soldinventorydialog();
    bool getQuantityCheckState();
    int getIDValue();
    int getQuantitySold();
    std::string getNoteString();

private:
    Ui::soldinventorydialog *ui;
};

#endif // SOLDINVENTORYDIALOG_H
