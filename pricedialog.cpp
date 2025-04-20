#include "pricedialog.h"
#include "ui_pricedialog.h"

pricedialog::pricedialog(std::vector<std::string> speciesList, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pricedialog)
{
    ui->setupUi(this);
    ui->speciesComboBox->clear();
    for(int i = 0; i < speciesList.size(); i++){
        ui->speciesComboBox->addItem(QString::fromStdString(speciesList[i]));
    }
}

pricedialog::~pricedialog()
{
    delete ui;
}

std::string pricedialog::getSpeciesName(){
    return ui->speciesComboBox->currentText().toStdString();
}

float pricedialog::getPriceChange(){
    return ui->priceEdit->text().toFloat();
}
