#include "pricedialog.h"
#include "ui_pricedialog.h"

pricedialog::pricedialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pricedialog)
{
    ui->setupUi(this);
}

pricedialog::~pricedialog()
{
    delete ui;
}

std::string pricedialog::getSpeciesName(){
    return ui->speciesEdit->text().toStdString();
}

float pricedialog::getPricePerCubicInch(){
    return ui->priceEdit->text().toFloat();
}
