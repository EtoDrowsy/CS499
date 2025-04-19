#ifndef SORTITEMDIALOG_H
#define SORTITEMDIALOG_H

#include <QDialog>

namespace Ui {
class sortitemdialog;
}

class sortitemdialog : public QDialog
{
    Q_OBJECT

public:
    explicit sortitemdialog(const std::vector<std::string>& attributes, QWidget *parent = nullptr);
    ~sortitemdialog();
    int getSelectedSortColumn() const;
    QString getSelectedColumn() const;

private slots:
    void on_sortPushButton_clicked();
    void on_cancelPushButton_clicked();

private:
    Ui::sortitemdialog *ui;
    std::vector<std::string> attNames;
};

#endif // SORTITEMDIALOG_H
