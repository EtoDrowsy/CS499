#ifndef HTMLGENDIALOG_H
#define HTMLGENDIALOG_H

#include <QDialog>

namespace Ui {
class htmlgendialog;
}

class htmlgendialog : public QDialog
{
    Q_OBJECT

public:
    explicit htmlgendialog(QWidget *parent = nullptr, QStringList idList = {});
    ~htmlgendialog();

private:
    Ui::htmlgendialog *ui;
};

#endif // HTMLGENDIALOG_H
