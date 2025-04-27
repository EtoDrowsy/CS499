/*
Author: Thomas Pierce
Source file for implementing cut list ui
*/


#include "../header/cutlistdialog.h"
#include "../ui/ui_cutlistdialog.h"
#include "../header/additemdialog.h"

cutlistdialog::cutlistdialog(std::vector<Lumber*> invvec, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::cutlistdialog)
{
    ui->setupUi(this);
    inventory = invvec;
    ui->cuttable->resizeColumnsToContents();
    cutList.clear();
    confirm = false;

    connect(ui->confirmButton, &QPushButton::clicked, this, &cutlistdialog::accept);
}

cutlistdialog::~cutlistdialog()
{
    delete ui;
}

//Running cut list algorithm to determine best materials
void cutlistdialog::on_genmaterials_clicked()
{
    std::vector<std::vector<std::string>> tableContents;
    for(int i = 1; i < ui->cuttable->rowCount(); i++){
        std::vector<std::string> row;
        for(int j = 0; j < ui->cuttable->columnCount(); j++){
            row.push_back(ui->cuttable->item(i,j)->text().toStdString());
        }
        tableContents.push_back(row);
    }
    std::vector <std::string> empty;

    for(int k = 0; k < tableContents.size(); k++){
        CutListItem* item = new CutListItem(tableContents[k][1],tableContents[k][2],tableContents[k][3],tableContents[k][0],std::stoi(tableContents[k][5]),empty,empty,tableContents[k][6],std::stoi(tableContents[k][4]));
        cutList.push_back(item);
    }

    cutIDVector = cutListToLumberList(inventory, cutList);

    ui->outputTextBrowser->setText(QString::fromStdString(printLumberList(cutIDVector, inventory, cutList)));
}

std::vector<Lumber*> cutlistdialog::getInventory(){
    return inventory;
}

//Call to add item dialog
void cutlistdialog::on_addbutton_clicked()
{
    std::vector<std::string> cutlistAttributes = {"Species", "Length", "Width", "Thickness", "Quantity", "Grade", "Name"};
    AddItemDialog addwindow(cutlistAttributes, this);
    if (addwindow.exec() == QDialog::Accepted) {
        std::vector<std::string> newItem = addwindow.getNewItemData();

        bool isComplete = true;

        for (size_t i = 0; i < newItem.size(); i++) {
            if (newItem[i].empty()) {
                isComplete = false;
                break;
            }
        }

        if (isComplete && newItem.size() == cutlistAttributes.size()) {
            int newRow = ui->cuttable->rowCount();
            ui->cuttable->insertRow(newRow);

            for (int j = 0; j < newItem.size(); j++) {
                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(newItem[j]));
                ui->cuttable->setItem(newRow, j, item);
            }
        }
    }
    ui->cuttable->resizeColumnsToContents();
}

//Applying cut list cuts to inventory
void cutlistdialog::on_confirmButton_clicked()
{
    confirm = true;
    if(cutIDVector.size() > 0){
        inventoryUpdate = confirmLumberList(inventory,cutList,cutIDVector);
    }
}

confirmer cutlistdialog::getInventoryUpdate(){
    return inventoryUpdate;
}
