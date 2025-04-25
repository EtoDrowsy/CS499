#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <set>

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

private slots:
    void handleDialogAccept();

private:
    Ui::AddItemDialog *ui;

    QVector<QString> validAttributes;

    std::set<std::string> seenIDs;

    void resetStyles();
};

#endif // ADDITEMDIALOG_H
