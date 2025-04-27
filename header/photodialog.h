#ifndef PHOTODIALOG_H
#define PHOTODIALOG_H

#include <QDialog>

namespace Ui {
class photodialog;
}

class photodialog : public QDialog
{
    Q_OBJECT

public:
    explicit photodialog(QWidget *parent = nullptr, int id = NULL);
    ~photodialog();
    std::string getFileName();
    bool getImageUpStatus();

private:
    QString upfilename;
    bool imageUploaded;
    int photoID;

private slots:
    void on_pushButton_clicked();

private:
    Ui::photodialog *ui;
};

#endif // PHOTODIALOG_H
