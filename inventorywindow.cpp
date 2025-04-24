#include "inventorywindow.h"
#include "additemdialog.h"
#include "cutlistdialog.h"
#include "ui_inventorywindow.h"
#include "Lumber.cpp"
#include "CutList.cpp"
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

enum column{
    ID,Location,Quantity,Length,Width,Thickness,Grade,Price,Description,Date,Species,Photo,Notes,Sold,Unknown
};

std::vector<std::string> CSVattributes;
std::vector<column> ColumnNames;
std::vector<std::vector<std::string>> dataArray;

std::string csvfilepath;

std::vector<Lumber*> inventory;

std::vector<Lumber*>soldInventory;
int soldIndex;
bool soldInvExists;

column stringToColumn(std::string readIn){
    if(readIn == "ID"){
        return ID;
    }
    else if(readIn == "Location"){
        return Location;
    }
    else if(readIn == "Quantity"){
        return Quantity;
    }
    else if(readIn == "Length"){
        return Length;
    }
    else if(readIn == "Width"){
        return Width;
    }
    else if(readIn == "Thickness"){
        return Thickness;
    }
    else if(readIn == "Grade"){
        return Grade;
    }
    else if(readIn == "Price"){
        return Price;
    }
    else if(readIn == "Description"){
        return Description;
    }
    else if(readIn == "Date"){
        return Date;
    }
    else if(readIn == "Species"){
        return Species;
    }
    else if(readIn == "Photo"){
        return Photo;
    }
    else if(readIn == "Notes"){
        return Notes;
    }
    else if(readIn == "Sold"){
        return Sold;
    }
    return Unknown;
}

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
        if (substr != "Date Sold" && substr != "Invoice Number"){
            CSVattributes.push_back(substr);
            ColumnNames.push_back(stringToColumn(substr));
        }
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
                Lumber* temp = new Lumber(row,CSVattributes);
                temp->setDateSold(*(row.end()-2));
                temp->setInvoiceNumber(std::stoi(*(row.end()-1)));
                soldInventory.push_back(temp);
            }
            else{
                row.erase(row.cbegin()+soldIndex);
                row.erase(row.end()-1);
                row.erase(row.end()-1);
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

void genHTML(std::string filename){
    std::fstream newhtml;
    newhtml.open(filename);
    if (!newhtml) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }
    newhtml << "<!DOCTYPE html>\n";
    newhtml << "<html lang=\"en\">\n";
    newhtml << "<head>\n";
    newhtml << "    <meta charset=\"UTF-8\">\n";
    newhtml << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    newhtml << "    <title>Your Wood Order</title>\n";
    newhtml << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n";
    newhtml << "</head>\n";
    newhtml << "<body>\n";
    newhtml << "    <div class=\"header-wrapper\">\n";
    newhtml << "        <h1 class=\"page-title\">Sawyer Inventory</h1>\n";
    newhtml << "    </div>\n";
    newhtml << "    <div class=\"grid-container\">\n";
    newhtml << "    </div>\n";
    newhtml << "</body>\n";
    newhtml << "</html>\n";

    newhtml.close();
}

void addToHTML(std::vector<Lumber*> woodList, std::string filename){
    std::ifstream readhtml(filename);
    std::vector<std::string> lines;
    std::string line;
    while(std::getline(readhtml, line)){
        lines.push_back(line);
    }
    readhtml.close();

    lines.pop_back();
    lines.pop_back();
    lines.pop_back();
    for (int i = 0; i < woodList.size(); i++) {
        lines.push_back("        <div class=\"wood-item\">");
        lines.push_back("            <p class=\"item-title-description\">" + woodList[i]->getDescription() + "<br></p>");
        lines.push_back("            <p class=\"item-title-species\">" + woodList[i]->getSpecies() + "<br></p>");
        lines.push_back("            <img src=\"/" + woodList[i]->getPhotoPath() + "\" alt=\"" + woodList[i]->getSpecies() +
                        "\" class=\"wood-image\" onerror=\"this.onerror=null;this.src='images/default.jpg';\">");
        lines.push_back("            <p class=\"item-price\">" + woodList[i]->getPriceDisplay() + "<br></p>");
        lines.push_back("            <p>"
             + woodList[i]->getLengthDisplay() + " x "
             + woodList[i]->getWidthDisplay() + " x "
             + woodList[i]->getThicknessDisplay() + "<br>"
             + std::to_string(woodList[i]->getQuantity()) + " in stock</p>");
        lines.push_back("        </div>");
    }
    lines.push_back("    </div>");
    lines.push_back("</body>");
    lines.push_back("</html>");

    std::ofstream writehtml(filename);
    for(int i = 0; i < lines.size(); i++){
        writehtml << lines[i] + "\n";
    }
    lines.clear();
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

    ui->searchComboBox->clear();

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
        soldInventory.clear();
        soldInvExists = false;

        readCSV(csvfilepath);

        ui->dataViewer->setEditTriggers(QAbstractItemView::NoEditTriggers);

        ui->dataViewer->setColumnCount(CSVattributes.size());
        ui->dataViewer->setRowCount(dataArray.size() + 1);

        for (int k = 0; k < CSVattributes.size(); k++) {
            QString att = QString::fromStdString(CSVattributes[k]);
            QTableWidgetItem *attq = new QTableWidgetItem(att);
            ui->dataViewer->setItem(0, k, attq);
            if(att != "Notes" && att != "Photo"){
                ui->searchComboBox->addItem(att);
            }
        }

        for (int i = 0; i < dataArray.size(); i++) {
            for (int j = 0; j < dataArray[i].size(); j++) {
                QString qstr = QString::fromStdString(dataArray[i][j]);
                QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
                ui->dataViewer->setItem(i + 1, j, newitem);
            }
        }

        csvLoaded = true;

        ui->dataViewer->resizeColumnsToContents();
    }
    return;
}

void InventoryWindow::on_addObjectButton_clicked()
{
    if (csvLoaded){
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
                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(newItem[j]));
                    ui->dataViewer->setItem(newRow, j, item);
                }

                writeCSV(csvfilepath);
            }
        }
    }
}

void InventoryWindow::writeCSV(const std::string &filePath) {
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        QMessageBox::warning(this, "File Error", "Could not open CSV file for writing: " + QString::fromStdString(filePath));
        return;
    }

    for (size_t i = 0; i < CSVattributes.size(); i++) {
        file << CSVattributes[i];
        if (i < CSVattributes.size() - 1) file << ";";
    }
    if (soldInvExists) {file << ";Sold;Date Sold;Invoice Number";}
    file << "\n";

    bool isFirstRow = true;
    for (auto &row : dataArray) {
        if (std::all_of(row.begin(), row.end(), [](const std::string& str) { return str.empty(); })) {
            continue;
        }

        if (!isFirstRow) file << "\n";

        for (size_t j = 0; j < row.size(); j++) {
            std::string value = row[j];

            if (CSVattributes[j] == "Length" && !value.empty()) {
                if (value.back() != '\"') value += "\"";
            }
            else if (CSVattributes[j] == "Width" && !value.empty()) {
                if (value.back() != '\"') value += "\"";
            }
            else if (CSVattributes[j] == "Thickness" && !value.empty()) {
                if (value.back() != '\"') value += "\"";
            }
            else if (CSVattributes[j] == "Price" && !value.empty() && value[0] != '$') {
                value = "$" + value;
            }

            file << value;
            if (j < row.size() - 1) file << ";";
        }
        if (soldInvExists) {file << ";;;";}

        isFirstRow = false;
    }

    if (soldInvExists){
        file << "\n";
        for (int i = 0; i < soldInventory.size(); i++){
            file << soldInventory[i]->toString() << "Yes;"+soldInventory[i]->getDateSold()+";"+std::to_string(soldInventory[i]->getInvoiceNumber());
            if (i != soldInventory.size() - 1) {file << "\n";}
        }
    }

    file.close();
}


void InventoryWindow::on_deleteObjectButton_clicked()
{
    if (csvLoaded){
        if(ui->dataViewer->currentColumn() == 0){
            int idDelete = std::stoi(ui->dataViewer->currentItem()->text().toStdString());
            deleteRowId(idDelete);

            writeCSV(csvfilepath);
            for (int i = 0; i < inventory.size(); i++){
                if (inventory[i]->getID() == std::to_string(idDelete)){
                    delete inventory[i];
                    inventory.erase(inventory.begin()+i);
                }
            }
            QMessageBox::information(this,"Item Deleted","The selected item has been deleted.");
        }
        else {
            QMessageBox::information(this, "Error", "Select an item ID in the inventory.");
        }
    }
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
        newcsv << "ID;Location;Quantity;Length;Width;Thickness;Grade;Price;Description;Date Acquired;Date Cut;Species;Photo;Notes;Sold;Date Sold;Invoice Number";
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
    if(csvLoaded){
        if(ui->dataViewer->currentColumn() == 0){
            int modifyID = std::stoi(ui->dataViewer->currentItem()->text().toStdString());

            modifyitemdialog modifyDialog(inventory,modifyID,this);
            if(modifyDialog.exec() == QDialog::Accepted){
                QString newNote = modifyDialog.getNoteText();

                if (newNote.isEmpty()){
                    QMessageBox::warning(this, "Incomplete Input", "Make sure the note box is filled.");
                    return;
                }

                int noteIndex;
                for (int i = 0; i < CSVattributes.size(); i++){
                    if (CSVattributes[i] == "Notes"){
                        noteIndex = i;
                    }
                }

                for (int i = 0; i < dataArray.size(); i++) {
                    if(QString::fromStdString(dataArray[i][0]) == std::to_string(modifyID)){
                        dataArray[i][noteIndex] = newNote.toStdString();

                        QTableWidgetItem *noteItem = new QTableWidgetItem(newNote);
                        ui->dataViewer->setItem(i + 1, noteIndex, noteItem);

                        break;

                    }
                }
                QMessageBox::warning(this, "Item Updated", "The note was added to the selected ID.");


                writeCSV(csvfilepath);
            }
        }
        else {
            QMessageBox::information(this, "Error", "Select an item ID in the inventory.");
        }
    }
}

void InventoryWindow::on_sortObjectButton_clicked()
{
    if(csvLoaded){
        // Getting current column and the attribute in it
        int columnIndex = ui->dataViewer->currentColumn();
        if (columnIndex != -1){
            std::string sortattr = CSVattributes[columnIndex];

            if (sortattr != "Photo" && sortattr != "Notes"){
                // Sorting based on the attribute
                std::sort(dataArray.begin(), dataArray.end(),
                          [columnIndex, this](const std::vector<std::string>& a, const std::vector<std::string>& b) {
                              if (CSVattributes[columnIndex] == "ID" || CSVattributes[columnIndex] == "Quantity" ||
                                  CSVattributes[columnIndex] == "Length" || CSVattributes[columnIndex] == "Width" || CSVattributes[columnIndex] == "Thickness") {
                                  try {
                                      return std::stoi(a[columnIndex]) < std::stoi(b[columnIndex]);
                                  } catch (...) {
                                      return a[columnIndex] < b[columnIndex];
                                  }
                              } else {
                                  return a[columnIndex] < b[columnIndex];
                              }
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
    }
}


void InventoryWindow::on_HTMLGenButton_clicked()
{
    QString htmlfilepath = QFileDialog::getSaveFileName(this, tr("Save File"),"",tr("HTML Files (*.html)"));

    QFile htmlfile(htmlfilepath);
    htmlfile.open(QIODevice::WriteOnly);
    htmlfile.close();

    std::string newhtmlpath = htmlfilepath.toStdString();

    if(newhtmlpath.empty()){
        QMessageBox::information(this, "File Name Empty", "File must have a name.");
        return;
    }
    else if(newhtmlpath.find(".html") == std::string::npos){
        QMessageBox::information(this, "Wrong File Type", "Incorrect file type, must be .html.");
        return;
    }
    else{
        genHTML(newhtmlpath);
        return;
    }
}

void InventoryWindow::on_cutlistButton_clicked()
{
    if(csvLoaded){
        cutlistdialog cutlistdialog(inventory, this);
        cutlistdialog.exec();
    }
}

void InventoryWindow::on_photoButton_clicked()
{
    if(csvLoaded){
        if(ui->dataViewer->currentColumn() == 0){
            int photoID = std::stoi(ui->dataViewer->currentItem()->text().toStdString());

            photodialog photowindow(this, photoID);
            if (photowindow.exec() == QDialog::Accepted){
                if (!photowindow.getImageUpStatus()){
                    QMessageBox::information(this, "Error", "No image selected.");
                }
                else{
                    int photoindex;
                    for (int i = 0; i < CSVattributes.size(); i++){
                        if (CSVattributes[i] == "Photo"){
                            photoindex = i;
                        }
                    }

                    for (int i = 0; i < dataArray.size(); i++) {
                        if(QString::fromStdString(dataArray[i][0]) == QString::number(photoID)){
                            dataArray[i][photoindex] = photowindow.getFileName();

                            QTableWidgetItem *photoItem = new QTableWidgetItem(QString::fromStdString(photowindow.getFileName()));
                            ui->dataViewer->setItem(i + 1, photoindex, photoItem);
                            for (int j = 0; j < inventory.size(); j++){
                                if (inventory[j]->getID() == std::to_string(photoID)){
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
        else {
            QMessageBox::information(this, "Error", "Select an item ID in the inventory.");
        }
    }
}

void InventoryWindow::on_soldButton_clicked()
{
    if(csvLoaded){
        soldinventoryviewer soldinvviewer(CSVattributes, soldInventory, this);
        soldinvviewer.exec();
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
    if(csvLoaded){
        if(ui->dataViewer->currentColumn() == 0){
            int saleID = std::stoi(ui->dataViewer->currentItem()->text().toStdString());

            soldinventorydialog soldinvdialog(this, saleID);
            if(soldinvdialog.exec() == QDialog::Accepted){
                if (!soldinvdialog.getQuantityCheckState() && soldinvdialog.getQuantitySold() == 0){
                    QMessageBox::information(this, "Error", "No quantity specified.");
                }
                else if (inventory[getInventoryIndex(saleID)]->getQuantity() < soldinvdialog.getQuantitySold()){
                    QMessageBox::information(this, "Error", "Not enough quantity.");
                }
                else{
                    if (!soldInvExists){
                        soldInvExists = true;
                        soldIndex = CSVattributes.size();
                    }
                    int invIndex = getInventoryIndex(saleID);
                    int invQuantity = inventory[invIndex]->getQuantity();
                    int soldQuantity = soldinvdialog.getQuantitySold();

                    if (invQuantity == soldQuantity || soldinvdialog.getQuantityCheckState()){
                        deleteRowId(saleID);
                        soldInventory.push_back(inventory[invIndex]);
                        soldInventory.back()->setNotes(soldinvdialog.getNoteString());
                        soldInventory.back()->setDateSold(soldinvdialog.getDateSold());
                        soldInventory.back()->setInvoiceNumber(soldinvdialog.getInvoiceNumber());
                        inventory.erase(inventory.begin() + invIndex);
                        writeCSV(csvfilepath);
                    }
                    else{
                        inventory[invIndex]->setQuantity(invQuantity - soldQuantity);
                        Lumber* copy = new Lumber(inventory[invIndex]->getAttributes(), inventory[invIndex]->getAttributeValues());
                        copy->setNotes(soldinvdialog.getNoteString());
                        copy->setQuantity(soldQuantity);
                        copy->setDateSold(soldinvdialog.getDateSold());
                        copy->setInvoiceNumber(soldinvdialog.getInvoiceNumber());
                        soldInventory.push_back(copy);

                        int quantityIndex;
                        for (int i = 0; i < CSVattributes.size(); i++){
                            if (CSVattributes[i] == "Quantity"){
                                quantityIndex = i;
                                break;
                            }
                        }
                        for (int i = 0; i < dataArray.size(); i++) {
                            if(dataArray[i][0] == std::to_string(saleID)){
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
        else {
            QMessageBox::information(this, "Error", "Select an item ID in the inventory.");
        }
    }
}

void InventoryWindow::on_priceButton_clicked()
{
    if(csvLoaded){
        std::vector<std::string> uniqueSpecies;
        for(int i = 0; i < inventory.size(); i++){
            auto newSpecies = std::find(uniqueSpecies.begin(), uniqueSpecies.end(), inventory[i]->getSpecies());
            if(newSpecies == uniqueSpecies.end()){
                uniqueSpecies.push_back(inventory[i]->getSpecies());
            }
        }
        pricedialog pricedialog(uniqueSpecies, this);
        if (pricedialog.exec() == QDialog::Accepted){
            if (!pricedialog.getSpeciesName().empty() && !(pricedialog.getPriceChange() == 0) && pricedialog.getPriceChange() > -100){
                int priceIndex;
                for (int i = 0; i < CSVattributes.size(); i++){
                    if (CSVattributes[i] == "Price"){
                        priceIndex = i;
                        break;
                    }
                }
                for (int i = 0; i < inventory.size(); i++){
                    if (inventory[i]->getSpecies() == pricedialog.getSpeciesName() && inventory[i]->getDescription() == "Bulk"){
                        float newPrice = 0;

                        newPrice = inventory[i]->getPrice();

                        newPrice = newPrice * (1 + (pricedialog.getPriceChange()/100));

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
            else{
                QMessageBox::information(this, "Error", "Invalid Price.");
            }
            uniqueSpecies.clear();
        }
    }
}


void InventoryWindow::on_HTMLAddButton_clicked()
{
    if(csvLoaded){
        std::vector<Lumber*> newStoreItems;
        QList<QTableWidgetItem*> selectedItems = ui->dataViewer->selectedItems();
        for(int i = 0; i < selectedItems.size(); i++){
            if(selectedItems[i]->column() == 0){
                newStoreItems.push_back(inventory[getInventoryIndex(std::stoi(selectedItems[i]->text().toStdString()))]);
            }
        }
        if(newStoreItems.size() > 0){
            std::string htmlfilepath = (QFileDialog::getOpenFileName(nullptr, "Select a file", "", "HTML Files (*.html)")).toStdString();

            if(htmlfilepath.empty()){
                QMessageBox::information(this, "No File Selected", "Please select a file.");
                return;
            }
            else if(htmlfilepath.find(".html") == std::string::npos){
                QMessageBox::information(this, "Wrong File Type", "Please select a .html file.");
                return;
            }
            else{
                addToHTML(newStoreItems, htmlfilepath);
            }
        }
        else{
            QMessageBox::information(this, "Error", "Select an item ID in the inventory.");
        }
        newStoreItems.clear();
        selectedItems.clear();
        return;
    }
}


void InventoryWindow::on_searchButton_clicked()
{
    if(csvLoaded){
        int attributeIndex = 0;
        for (int i = 0; i < CSVattributes.size(); i++){
            if (CSVattributes[i] == ui->searchComboBox->currentText().toStdString()){
                attributeIndex = i;
                break;
            }
        }
        std::vector<std::vector<std::string>> searchedDataArray;
        ui->dataViewer->clear();

        ui->dataViewer->setEditTriggers(QAbstractItemView::NoEditTriggers);

        for (int j = 0; j < dataArray.size(); j++){
            if (dataArray[j][attributeIndex] == ui->searchField->text().toStdString()){
                searchedDataArray.push_back(dataArray[j]);
            }
        }

        ui->dataViewer->setColumnCount(CSVattributes.size());
        ui->dataViewer->setRowCount(searchedDataArray.size() + 1);

        for (int k = 0; k < CSVattributes.size(); k++) {
            QString att = QString::fromStdString(CSVattributes[k]);
            QTableWidgetItem *attq = new QTableWidgetItem(att);
            ui->dataViewer->setItem(0, k, attq);
        }

        for (int i = 0; i < searchedDataArray.size(); i++) {
            for (int j = 0; j < searchedDataArray[i].size(); j++) {
                QString qstr = QString::fromStdString(searchedDataArray[i][j]);
                QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
                ui->dataViewer->setItem(i + 1, j, newitem);
            }
        }
        searchedDataArray.clear();
    }
}


void InventoryWindow::on_clearSearchButton_clicked()
{
    if(csvLoaded){
        ui->searchField->setText("");
        ui->dataViewer->clear();
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
    }
}

