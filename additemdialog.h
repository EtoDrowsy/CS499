#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(std::vector<std::string> attributes, QWidget *parent = nullptr);
    ~AddItemDialog();

    std::vector<std::string> getNewItemData() const;

    bool validate();

private:
    Ui::AddItemDialog *ui;

    QVector<QString> validAttributes;

    void resetStyles();
};

#endif // ADDITEMDIALOG_H
