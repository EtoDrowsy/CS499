#include "additemdialog.h"
#include "ui_additemdialog.h"

AddItemDialog::AddItemDialog(std::vector<std::string> attributes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);
    ui->attributetable->setRowCount(attributes.size());
    ui->attributetable->setColumnCount(1);
    ui->inputtable->setRowCount(attributes.size());
    ui->inputtable->setColumnCount(1);
    for(int i = 0; i < attributes.size(); i++){
        QString att = QString::fromStdString(attributes[i]);
        QTableWidgetItem *attq = new QTableWidgetItem(att);
        ui->attributetable->setItem(i,0,attq);
    }
}

AddItemDialog::~AddItemDialog()
{
    delete ui;
}
