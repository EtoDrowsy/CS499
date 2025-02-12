#include "inventorywindow.h"
#include "./ui_inventorywindow.h"
#include "additemdialog.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> CSVattributes;
std::vector<std::vector<std::string>> dataArray;

void readCSV(std::string filepath){
    std::fstream inputcsv;
    inputcsv.open(filepath);
    std::string line;

    //Read first line to get attributes
    std::getline(inputcsv, line);
    std::stringstream ss(line);
    while (ss.good()){
        std::string substr;
        getline(ss,substr,';');
        CSVattributes.push_back(substr);
    }

    //Reads all "objects"
    while(!inputcsv.eof()){
        std::getline(inputcsv, line);
        std::stringstream ss(line);
        std::vector<std::string> row;
        while (ss.good()){
            std::string substr;
            getline(ss,substr,';');
            row.push_back(substr);
        }
        dataArray.push_back(row);
    }
    inputcsv.close();

    return;
}

InventoryWindow::InventoryWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InventoryWindow)
{
    ui->setupUi(this);
}

InventoryWindow::~InventoryWindow()
{
    delete ui;
}

void InventoryWindow::on_csvLoadButton_clicked()
{
    readCSV("C:\\Users\\Thomas\\Documents\\Uni Spring 2025\\CS499Project\\CS499\\TestInput.csv");

    ui->dataViewer->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->dataViewer->setColumnCount(CSVattributes.size());
    ui->dataViewer->setRowCount(dataArray.size());

    for(int k = 0; k < CSVattributes.size(); k++){
        QString att = QString::fromStdString(CSVattributes[k]);
        QTableWidgetItem *attq = new QTableWidgetItem(att);
        ui->dataViewer->setItem(0,k,attq);
    }

    for(int i = 0; i < dataArray.size(); i++){
        for(int j = 0; j < dataArray[i].size(); j++){
            QString qstr = QString::fromStdString(dataArray[i][j]);
            QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
            ui->dataViewer->setItem(i+1,j,newitem);
        }
    }
    return;
}


void InventoryWindow::on_addObjectButton_clicked()
{
    AddItemDialog addwindow(CSVattributes, this);
    addwindow.exec();
}

