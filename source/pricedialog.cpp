/*
Author: Thomas Pierce
Source file for implementing price adjustment ui
*/

#include "../header/pricedialog.h"
#include "../ui/ui_pricedialog.h"

pricedialog::pricedialog(std::vector<std::string> speciesList, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pricedialog)
{
    //Setting up price dialog
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
