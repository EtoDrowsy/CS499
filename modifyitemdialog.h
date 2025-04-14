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
    explicit modifyitemdialog(std::vector<Lumber*> inv, QWidget *parent = nullptr);
    ~modifyitemdialog();

    QString getEnteredID() const;
    QString getNoteText() const;
    void setIdNoteMap(const QMap<QString, QString> &map);

private slots:
    void onIdChanged(const QString &text);



    void on_viewButton_clicked();

private:
    Ui::modifyitemdialog *ui;
    QMap<QString, QString> idNoteMap;
    std::vector<Lumber*> inventory;
};

#endif // MODIFYITEMDIALOG_H
