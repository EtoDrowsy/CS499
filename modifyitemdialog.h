#ifndef MODIFYITEMDIALOG_H
#define MODIFYITEMDIALOG_H

#include <QDialog>
#include <QString>
#include <QMap>

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
    void setIdNoteMap(const QMap<QString, QString> &map);

private slots:
    void onIdChanged(const QString &text);



private:
    Ui::modifyitemdialog *ui;
    QMap<QString, QString> idNoteMap;
};

#endif // MODIFYITEMDIALOG_H
