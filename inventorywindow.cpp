#include "inventorywindow.h"
#include "additemdialog.h"
#include "deleteitemdialog.h"
#include "sortitemdialog.h"
#include "cutlistdialog.h"
#include "ui_inventorywindow.h"
#include "Lumber.cpp"
#include "htmlgendialog.h"

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

std::vector<Lumber*> inventory;
QStringList potentialItems;
std::vector<Lumber*> selectedInventory;

void readCSV(std::string filepath)
{
    std::fstream inputcsv;
    inputcsv.open(filepath);
    std::string line;

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
        inventory.push_back(new Lumber(row,CSVattributes));
    }
    inputcsv.close();

    return;
}

void generateHTML(std::vector<Lumber*> woodList, std::string filename) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }

    file << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
    file << "    <meta charset=\"UTF-8\">\n";
    file << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    file << "    <title>Your Wood Order</title>\n";
    file << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n";
    file << "</head>\n<body>\n";

    for (int i = 0; i < woodList.size(); i++) {
        file << "    <div class=\"wood-item\">\n";
        file << "        <h3>Species: " << woodList[i]->getSpecies() << "</h3>\n";
        file << "        <p>Dimensions: " << woodList[i]->getLength() << woodList[i]->getWidth() << woodList[i]->getThickness() << " <br>\n";
        file << "        Quantity: " << "0" << " <br>\n";
        file << "        Price: $" << "0" << "</p>\n";
        file << "    </div>\n";
    }

    file << "</body>\n</html>\n";
    file.close();
    std::cout << "HTML file generated successfully: " << filename << std::endl;
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
    inventory.clear();
}

void InventoryWindow::on_csvLoadButton_clicked()
{
    if(csvLoaded){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "CSV Loaded",
                                      "The CSV has already been loaded. Do you want to load in a new file?",
                                        QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::No){
            return;
        }
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
        ui->dataViewer->clear();
        CSVattributes.clear();
        dataArray.clear();

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

        bool isComplete = true;
        for (size_t i = 0; i < newItem.size(); i++) {
            if (newItem[i].empty()) {
                isComplete = false;
                break;
            }
        }

        if (isComplete && newItem.size() == CSVattributes.size()) {
            dataArray.push_back(newItem);

            int newRow = ui->dataViewer->rowCount();
            ui->dataViewer->insertRow(newRow);

            for (int j = 0; j < newItem.size(); j++) {
                QTableWidgetItem* item = new QTableWidgetItem(
                    QString::fromStdString(newItem[j]));
                ui->dataViewer->setItem(newRow, j, item);
            }

            writeCSV(csvfilepath);
        }
    }
}

void InventoryWindow::writeCSV(const std::string &filePath) {
    qDebug() << "Saving to CSV file:" << QString::fromStdString(filePath);

    std::ofstream file(filePath, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        QMessageBox::warning(this, "File Error", "Could not open CSV file for writing: " + QString::fromStdString(filePath));
        return;
    }

    for (size_t i = 0; i < CSVattributes.size(); i++) {
        file << CSVattributes[i];
        if (i < CSVattributes.size() - 1) file << ";";
    }
    file << "\n";

    bool isFirstRow = true;
    for (auto &row : dataArray) {
        if (std::all_of(row.begin(), row.end(), [](const std::string& str) { return str.empty(); })) {
            continue;
        }

        if (!isFirstRow) file << "\n";

        for (size_t j = 0; j < row.size(); j++) {
            std::string value = row[j];
            qDebug() << "Processing field:" << CSVattributes[j] << "Value:" << QString::fromStdString(value);

            if (CSVattributes[j] == "Length" && !value.empty()) {
                if (value.back() != '\"') value += "\"";
            }
            else if (CSVattributes[j] == "Width" && !value.empty()) {
                if (value.back() != '\"') value += "\"";
            }
            else if (CSVattributes[j] == "Price" && !value.empty() && value[0] != '$') {
                value = "$" + value;
            }

            file << value;
            if (j < row.size() - 1) file << ";";
        }

        isFirstRow = false;
    }

    file.close();
    qDebug() << "CSV file saved successfully.";
}


void InventoryWindow::on_deleteObjectButton_clicked()
{
    DeleteItemDialog *deleteDialog = new DeleteItemDialog(this);

    int rowCount = ui->dataViewer->rowCount() - 1;
    deleteDialog->updateDeleteSpinBox(rowCount);

    if (deleteDialog->exec() == QDialog::Accepted) {
        int idDelete = deleteDialog->getInputId();

        if (idDelete <= -1){
            QMessageBox::warning(this,"Invalid Input","Please enter a valid ID number.");
        }
        else if (!deleteRowId(idDelete)){
            QMessageBox::warning(this,"ID Not Found","The ID number does not exist. Please select an existing ID.");
        }
        else {
            writeCSV(csvfilepath);
            QMessageBox::information(this,"Item Deleted","The selected item has been deleted.");
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

void InventoryWindow::on_sortObjectButton_clicked()
{
    sortitemdialog *sortDialog = new sortitemdialog(CSVattributes, this);

    if (sortDialog->exec() == QDialog::Accepted) {
        QString selectedColumn = sortDialog->getSelectedColumn();

        int columnIndex = -1;
        if (selectedColumn == "Species") {
            columnIndex = 3;
        } else if (selectedColumn == "Location") {
            columnIndex = 1;
        }

        if (columnIndex != -1) {
            std::sort(dataArray.begin(), dataArray.end(), [columnIndex](const std::vector<std::string>& a, const std::vector<std::string>& b) {
                return a[columnIndex] < b[columnIndex];
            });

            ui->dataViewer->setRowCount(dataArray.size() + 1);
            for (int i = 0; i < dataArray.size(); i++) {
                for (int j = 0; j < dataArray[i].size(); j++) {
                    QString qstr = QString::fromStdString(dataArray[i][j]);
                    QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
                    ui->dataViewer->setItem(i + 1, j, newitem);
                }
            }

            writeCSV(csvfilepath);
        }
    }

    delete sortDialog;
}

void InventoryWindow::on_pushButton_clicked(){
    cutlistdialog cutwindow(this);
    if (cutwindow.exec() == QDialog::Accepted) {

    }
}


void InventoryWindow::on_HTMLGenButton_clicked()
{
    for(int i = 0; i < inventory.size(); i++){
        potentialItems.push_back(QString::fromStdString(inventory[i]->getID()));
    }
    htmlgendialog htmlwindow(this, potentialItems);
    if (htmlwindow.exec() == QDialog::Accepted){
        std::vector<std::string> selectedItems = htmlwindow.getCheckedItems();
        for(int i = 0; i < selectedItems.size(); i++){
            for(int j = 0; j < inventory.size(); j++){
                if(inventory[j]->getID() == selectedItems[i]){
                    selectedInventory.push_back(inventory[j]);
                }
            }
        }
    }
    generateHTML(selectedInventory, "index.html");
}
