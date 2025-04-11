#include "inventorywindow.h"
#include "additemdialog.h"
#include "deleteitemdialog.h"
#include "sortitemdialog.h"
#include "cutlistdialog.h"
#include "ui_inventorywindow.h"
#include "Lumber.cpp"
#include "CutList.cpp"
#include "htmlgendialog.h"
#include "photodialog.h"
#include "soldinventoryviewer.h"
#include "soldinventorydialog.h"
#include "pricedialog.h"

#include "modifyitemdialog.h"

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
std::vector<Lumber*> selectedInventory;

std::vector<Lumber*>soldInventory;
int soldIndex;
bool soldInvExists = false;

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

    for(int i = 0; i < CSVattributes.size(); i++){
        if (CSVattributes[i] == "Sold"){
            soldInvExists = true;
            soldIndex = i;
            CSVattributes.erase(CSVattributes.begin()+i);
        }
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
        if (soldInvExists){
            if (row[soldIndex] == "Yes"){
                soldInventory.push_back(new Lumber(row,CSVattributes));
            }
            else{
                row.erase(row.cbegin()+soldIndex);
                dataArray.push_back(row);
                inventory.push_back(new Lumber(row,CSVattributes));
            }
        }
        else{
            dataArray.push_back(row);
            inventory.push_back(new Lumber(row,CSVattributes));
        }
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

    file << "<!DOCTYPE html>\n";
    file << "<html lang=\"en\">\n";
    file << "<head>\n";
    file << "    <meta charset=\"UTF-8\">\n";
    file << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    file << "    <title>Your Wood Order</title>\n";
    file << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n";
    file << "</head>\n";
    file << "<body>\n";
    file << "    <div class=\"header-wrapper\">\n";
    file << "        <h1 class=\"page-title\">Sawyer Inventory</h1>\n";
    file << "    </div>\n";
    file << "    <div class=\"grid-container\">\n";

    for (int i = 0; i < woodList.size(); i++) {
        file << "        <div class=\"wood-item\">\n";
        file << "            <p class=\"item-title-description\">" << woodList[i]->getDescription() << "<br></p>\n";
        file << "            <p class=\"item-title-species\">" << woodList[i]->getSpecies() << "<br></p>\n";

        // image filename could be derived from species, lowercase if needed
        file << "            <img src=\"/" << woodList[i]->getPhotoPath() << "\" alt=\"" << woodList[i]->getSpecies()
             << "\" class=\"wood-image\" onerror=\"this.onerror=null;this.src='images/default.jpg';\">\n";

        file << "            <p class=\"item-price\">" << woodList[i]->getPriceDisplay() << "<br></p>\n";
        file << "            <p>"
             << woodList[i]->getLengthDisplay() << " x "
             << woodList[i]->getWidthDisplay() << " x "
             << woodList[i]->getThicknessDisplay() << "<br>"
             << woodList[i]->getQuantity() << " in stock</p>\n";

        file << "        </div>\n";
    }

    file << "    </div>\n";
    file << "</body>\n";
    file << "</html>\n";

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
            inventory.push_back(new Lumber(newItem,CSVattributes));

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
    if (soldInvExists) {file << ";Sold";}
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
            else if  (CSVattributes[j] == "Notes" && !value.empty()) {

            }

            file << value;
            if (j < row.size() - 1) file << ";";
        }
        if (soldInvExists) {file << ";";}

        isFirstRow = false;
    }

    if (soldInvExists){
        file << "\n";
        for (int i = 0; i < soldInventory.size(); i++){
            file << soldInventory[i]->toString() << "Yes";
            if (i != soldInventory.size() - 1) {file << "\n";}
        }
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
            for (int i = 0; i < inventory.size(); i++){
                if (inventory[i]->getID() == std::to_string(idDelete)){
                    delete inventory[i];
                    inventory.erase(inventory.begin()+i);
                }
            }
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

bool InventoryWindow::checkInInv(int photoid){
    for (int row = 1; row < ui->dataViewer->rowCount(); ++row) {
        QString id = ui->dataViewer->item(row, 0)->text();
        if (id.toInt() == photoid) {
            return true;
        }
    }
    return false;
}

int InventoryWindow::getIndex(int searchid){
    for (int row = 1; row < ui->dataViewer->rowCount(); ++row) {
        QString id = ui->dataViewer->item(row, 0)->text();
        if (id.toInt() == searchid) {
            return row;
        }
    }
    return 0;
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

void InventoryWindow::on_modifyObjectButton_clicked()
{
    bool idFound = false;

    modifyitemdialog modifyDialog(this);
    if(modifyDialog.exec() == QDialog::Accepted){
        QString inputID = modifyDialog.getEnteredID();
        QString newNote = modifyDialog.getNoteText();

        if (inputID.isEmpty() || newNote.isEmpty()){
            QMessageBox::warning(this, "Incomplete Input", "Make sure ID and note box are filled.");
            return;
        }

        int noteIndex;
        for (int i = 0; i < CSVattributes.size(); i++){
            if (CSVattributes[i] == "Notes"){
                noteIndex = i;
            }
        }

        for (int i = 0; i < dataArray.size(); i++) {
            if(QString::fromStdString(dataArray[i][0]) == inputID){

            dataArray[i][noteIndex] = newNote.toStdString();

            QTableWidgetItem *noteItem = new QTableWidgetItem(newNote);
            ui->dataViewer->setItem(i + 1, noteIndex, noteItem);

            idFound = true;
            break;

            }
        }

        if(!idFound) {
            QMessageBox::warning(this, "ID Not Found", "The inputted ID is not in the current file.");
        }
        else{
            QMessageBox::warning(this, "Item Updated", "The note was added to the selected ID.");
        }

        writeCSV(csvfilepath);
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


void InventoryWindow::on_HTMLGenButton_clicked()
{
    QStringList potentialItems;
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
        generateHTML(selectedInventory, "index.html");
    }
}

void InventoryWindow::on_cutlistButton_clicked()
{
    cutlistdialog cutlistdialog(inventory, this);
    cutlistdialog.exec();
}

void InventoryWindow::on_photoButton_clicked()
{
    photodialog photowindow(this);
    if (photowindow.exec() == QDialog::Accepted){
        if (!photowindow.getImageUpStatus()){
            QMessageBox::information(this, "Error", "No image selected.");
        }
        else if (!checkInInv(photowindow.getIDValue())){
            QMessageBox::information(this, "Error", "ID not in inventory.");
        }
        else{
            int photoindex;
            for (int i = 0; i < CSVattributes.size(); i++){
                if (CSVattributes[i] == "Photo"){
                    photoindex = i;
                }
            }

            for (int i = 0; i < dataArray.size(); i++) {
                if(QString::fromStdString(dataArray[i][0]) == QString::number(photowindow.getIDValue())){

                    dataArray[i][photoindex] = photowindow.getFileName();

                    QTableWidgetItem *photoItem = new QTableWidgetItem(QString::fromStdString(photowindow.getFileName()));
                    ui->dataViewer->setItem(i + 1, photoindex, photoItem);
                    for (int j = 0; j < inventory.size(); j++){
                        if (inventory[j]->getID() == std::to_string(photowindow.getIDValue())){
                            inventory[j]->setPhotoPath(photowindow.getFileName());
                            break;
                        }
                    }
                    break;

                }
            }

            writeCSV(csvfilepath);

            // returns row index
            // write to table and then write to object
        }
    }
}

void InventoryWindow::on_soldButton_clicked()
{
    soldinventoryviewer soldinvviewer(CSVattributes, soldInventory, this);
    if(soldinvviewer.exec() == QDialog::Accepted){
        // if(ui->idEdit->text().isEmpty || ui->quantityEdit->text().isEmpty()) {
        //     QMessageBox::warning(this, "Input Error", "Please enter valid ID or quantity number.");
        //     return;
        // }
        // ^^ use to see if they are empty idk if you need that
    }
}

int getInventoryIndex(int id){
    int index;
    for (int i = 0; i < inventory.size(); i++){
        if (std::stoi(inventory[i]->getID()) == id){
            index = i;
            break;
        }
    }
    return index;
}


void InventoryWindow::on_newSaleButton_clicked()
{
    soldinventorydialog soldinvdialog(this);
    if(soldinvdialog.exec() == QDialog::Accepted){
        if (!checkInInv(soldinvdialog.getIDValue())){
            QMessageBox::information(this, "Error", "ID not in inventory.");
        }
        else if (!soldinvdialog.getQuantityCheckState() && soldinvdialog.getQuantitySold() == 0){
            QMessageBox::information(this, "Error", "No quantity specified.");
        }
        else if (inventory[getInventoryIndex(soldinvdialog.getIDValue())]->getQuantity() < soldinvdialog.getQuantitySold()){
            qDebug() << "NOT ENOUGH QUANTITY";
        }
        else{
            if (!soldInvExists){
                soldInvExists = true;
                soldIndex = CSVattributes.size();
            }
            int invIndex = getInventoryIndex(soldinvdialog.getIDValue());
            int invQuantity = inventory[invIndex]->getQuantity();
            int soldQuantity = soldinvdialog.getQuantitySold();

            if (invQuantity == soldQuantity || soldinvdialog.getQuantityCheckState()){
                deleteRowId(soldinvdialog.getIDValue());
                soldInventory.push_back(inventory[invIndex]);
                soldInventory.back()->setNotes(soldinvdialog.getNoteString());
                inventory.erase(inventory.begin() + invIndex);
                writeCSV(csvfilepath);
            }
            else{
                inventory[invIndex]->setQuantity(invQuantity - soldQuantity);
                Lumber* copy = new Lumber(inventory[invIndex]->getAttributes(), inventory[invIndex]->getAttributeValues());
                copy->setNotes(soldinvdialog.getNoteString());
                copy->setQuantity(soldQuantity);
                soldInventory.push_back(copy);

                int quantityIndex;
                for (int i = 0; i < CSVattributes.size(); i++){
                    if (CSVattributes[i] == "Quantity"){
                        quantityIndex = i;
                        break;
                    }
                }
                for (int i = 0; i < dataArray.size(); i++) {
                    if(dataArray[i][0] == std::to_string(soldinvdialog.getIDValue())){
                        dataArray[i][quantityIndex] = std::to_string(inventory[invIndex]->getQuantity());
                        ui->dataViewer->setItem(i+1, quantityIndex, new QTableWidgetItem(QString::number(inventory[invIndex]->getQuantity())));
                        break;
                    }
                }
                writeCSV(csvfilepath);
            }
        }
    }
}

void InventoryWindow::on_priceButton_clicked()
{
    pricedialog pricedialog(this);
    if (pricedialog.exec() == QDialog::Accepted){
        if (!pricedialog.getSpeciesName().empty() && !(pricedialog.getPricePerCubicInch() == 0)){
            int priceIndex;
            for (int i = 0; i < CSVattributes.size(); i++){
                if (CSVattributes[i] == "Price"){
                    priceIndex = i;
                    break;
                }
            }
            for (int i = 0; i < inventory.size(); i++){
                if (inventory[i]->getSpecies() == pricedialog.getSpeciesName() && inventory[i]->getDescription() == "Bulk"){
                    std::vector<float> dimensions = inventory[i]->getDimensions();
                    float newPrice = 0;

                    for (int j = 0; j < dimensions.size(); j++){
                        newPrice += dimensions[j];
                    }

                    newPrice = newPrice * pricedialog.getPricePerCubicInch();

                    inventory[i]->setBulkPrice(newPrice);

                    for (int k = 0; k < dataArray.size(); k++) {
                        if(dataArray[k][0] == inventory[i]->getID()){
                            dataArray[k][priceIndex] = inventory[i]->getAttributes()[priceIndex];
                            ui->dataViewer->setItem(k+1, priceIndex, new QTableWidgetItem(QString::fromStdString(inventory[i]->getAttributes()[priceIndex])));
                            break;
                        }
                    }
                }
            }
            writeCSV(csvfilepath);
        }
    }
}

