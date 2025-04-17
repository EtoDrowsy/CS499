#ifndef MODIFYITEMDIALOG_H
#define MODIFYITEMDIALOG_H

#include <QDialog>
#include <QString>
#include <QMap>
#include "Lumber.cpp"

namespace Ui {
class modifyitemdialog;
}

class modifyitemdialog : public QDialog
{
    Q_OBJECT

public:
    explicit modifyitemdialog(std::vector<Lumber*> inv, int id = NULL, QWidget *parent = nullptr);
    ~modifyitemdialog();

    QString getNoteText() const;
    void setIdNoteMap(const QMap<QString, QString> &map);

private slots:
    void on_viewButton_clicked();

private:
    Ui::modifyitemdialog *ui;
    QMap<QString, QString> idNoteMap;
    std::vector<Lumber*> inventory;
    int modifyID;
};

#endif // MODIFYITEMDIALOG_H
