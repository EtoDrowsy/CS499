#ifndef MODIFYITEMDIALOG_H
#define MODIFYITEMDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class modifyitemdialog;
}

class modifyitemdialog : public QDialog
{
    Q_OBJECT

public:
    explicit modifyitemdialog(QWidget *parent = nullptr);
    ~modifyitemdialog();

    QString getEnteredID() const;
    QString getNoteText() const;

private:
    Ui::modifyitemdialog *ui;
};

#endif // MODIFYITEMDIALOG_H
