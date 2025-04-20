#ifndef PRICEDIALOG_H
#define PRICEDIALOG_H

#include <QDialog>

namespace Ui {
class pricedialog;
}

class pricedialog : public QDialog
{
    Q_OBJECT

public:
    explicit pricedialog(std::vector<std::string> speciesList, QWidget *parent = nullptr);
    ~pricedialog();
    std::string getSpeciesName();
    float getPricePerCubicInch();

private:
    Ui::pricedialog *ui;
};

#endif // PRICEDIALOG_H
