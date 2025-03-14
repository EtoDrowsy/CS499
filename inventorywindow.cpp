#include "inventorywindow.h"
#include "./ui_inventorywindow.h"
#include "additemdialog.h"
#include "deleteitemdialog.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <QMessageBox>
#include <QFileDialog>

std::vector<std::string> CSVattributes;
std::vector<std::vector<std::string>> dataArray;

std::string csvfilepath;

void readCSV(std::string filepath)
{
    std::fstream inputcsv;
    inputcsv.open(filepath);
    std::string line;

    //Read first line to get attributes
    std::getline(inputcsv, line);
    std::stringstream ss(line);
    while (ss.good()) {
        std::string substr;
        getline(ss, substr, ';');
        CSVattributes.push_back(substr);
    }

    //Reads all "objects"
    while (!inputcsv.eof()) {
        std::getline(inputcsv, line);
        std::stringstream ss(line);
        std::vector<std::string> row;
        while (ss.good()) {
            std::string substr;
            getline(ss, substr, ';');
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
    if(csvLoaded){
        QMessageBox::information(this, "CSV Loaded", "The CSV has already been loaded.");
        return;
    }

    csvfilepath = (QFileDialog::getOpenFileName(nullptr, "Select a file", "", "CSV Files (*.csv)")).toStdString();

    if(csvfilepath.empty()){
        QMessageBox::information(this, "No File Selected", "Please select a file.");
        return;
    }
    else if(csvfilepath.find(".csv") == std::string::npos){
        QMessageBox::information(this, "Wrong File Type", "Please select a .csv file.");
        return;
    }
    else{
        readCSV(csvfilepath);

        ui->dataViewer->setEditTriggers(QAbstractItemView::NoEditTriggers);

        ui->dataViewer->setColumnCount(CSVattributes.size());
        ui->dataViewer->setRowCount(dataArray.size() + 1);

        for (int k = 0; k < CSVattributes.size(); k++) {
            QString att = QString::fromStdString(CSVattributes[k]);
            QTableWidgetItem *attq = new QTableWidgetItem(att);
            ui->dataViewer->setItem(0, k, attq);
        }

        for (int i = 0; i < dataArray.size(); i++) {
            for (int j = 0; j < dataArray[i].size(); j++) {
                QString qstr = QString::fromStdString(dataArray[i][j]);
                QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
                ui->dataViewer->setItem(i + 1, j, newitem);
            }
        }

        csvLoaded = true;
    }
    return;
}

void InventoryWindow::on_addObjectButton_clicked()
{
    AddItemDialog addwindow(CSVattributes, this);
    if (addwindow.exec() == QDialog::Accepted) {
        std::vector<std::string> newItem = addwindow.getNewItemData();

        if (newItem.size() == CSVattributes.size()) {
            int newRow = ui->dataViewer->rowCount();
            ui->dataViewer->insertRow(newRow);

            for (int j = 0; j < newItem.size(); j++) {
                QString qstr = QString::fromStdString(newItem[j]);
                QTableWidgetItem *newItemCell = new QTableWidgetItem(qstr);
                ui->dataViewer->setItem(newRow, j, newItemCell);
            }

            dataArray.push_back(newItem);

            writeCSV(csvfilepath);
        }
    }
}

void InventoryWindow::writeCSV(const std::string &filePath)
{
    std::ofstream file(filePath);
    if (!file.is_open()) {
        QMessageBox::warning(this, "File Error", "Could not open CSV file for writing.");
        return;
    }

    for (size_t i = 0; i < CSVattributes.size(); i++) {
        file << CSVattributes[i];
        if (i < CSVattributes.size() - 1)
            file << ";";
    }
    file << "\n";

    bool isFirstRow = true;
    for (const auto &row : dataArray) {
        if (std::all_of(row.begin(), row.end(), [](const std::string& str) { return str.empty(); })) {
            continue;
        }

        if (!isFirstRow) {
            file << "\n";
        }

        for (size_t j = 0; j < row.size(); j++) {
            file << row[j];
            if (j < row.size() - 1) {
                file << ";";
            }
        }

        isFirstRow = false;
    }

    file.close();
}

void InventoryWindow::on_deleteObjectButton_clicked()
{
    DeleteItemDialog *deleteDialog = new DeleteItemDialog(this);

    int rowCount = ui->dataViewer->rowCount() - 1;
    deleteDialog->updateDeleteSpinBox(rowCount);

    if (deleteDialog->exec() == QDialog::Accepted) {
        int idDelete = deleteDialog->getInputId();

        if (deleteRowId(idDelete)) {
            writeCSV(csvfilepath);
        }
    }

    delete deleteDialog;
}

bool InventoryWindow::deleteRowId(int idDelete)
{
    for (int row = 1; row < ui->dataViewer->rowCount(); ++row) {
        QString id = ui->dataViewer->item(row, 0)->text();
        if (id.toInt() == idDelete) {
            ui->dataViewer->removeRow(row);

            dataArray.erase(dataArray.begin() + row - 1);
            return true;
        }
    }
    return false;
}

void InventoryWindow::on_createCSVButton_clicked()
{
    if(csvLoaded){
        QMessageBox::information(this, "CSV Loaded", "The CSV has already been loaded.");
        return;
    }

    QString tempcsvfilepath = QFileDialog::getSaveFileName(this, tr("Save File"),"",tr("CSV Files (*.csv)"));

    QFile file(tempcsvfilepath);
    file.open(QIODevice::WriteOnly);
    file.close();

    csvfilepath = tempcsvfilepath.toStdString();

    std::cout << csvfilepath;

    if(csvfilepath.empty()){
        QMessageBox::information(this, "File Name Empty", "File must have a name.");
        return;
    }
    else if(csvfilepath.find(".csv") == std::string::npos){
        QMessageBox::information(this, "Wrong File Type", "Incorrect file type, must be .csv.");
        return;
    }
    else{
        std::fstream newcsv;
        newcsv.open(csvfilepath);
        newcsv << "ID;Location;Quantity;Species;Description;Length;Width;Thickness;Price";
        newcsv.close();

        readCSV(csvfilepath);

        ui->dataViewer->setEditTriggers(QAbstractItemView::NoEditTriggers);

        ui->dataViewer->setColumnCount(CSVattributes.size());
        ui->dataViewer->setRowCount(dataArray.size() + 1);

        for (int k = 0; k < CSVattributes.size(); k++) {
            QString att = QString::fromStdString(CSVattributes[k]);
            QTableWidgetItem *attq = new QTableWidgetItem(att);
            ui->dataViewer->setItem(0, k, attq);
        }

        for (int i = 0; i < dataArray.size(); i++) {
            for (int j = 0; j < dataArray[i].size(); j++) {
                QString qstr = QString::fromStdString(dataArray[i][j]);
                QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
                ui->dataViewer->setItem(i + 1, j, newitem);
            }
        }

        csvLoaded = true;

        newcsv.close();
        return;
    }
}


void InventoryWindow::on_pushButton_clicked()
{

}

