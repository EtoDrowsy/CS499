#include "inventorywindow.h"
#include "additemdialog.h"
#include "deleteitemdialog.h"
#include "sortitemdialog.h"
#include "cutlistdialog.h"
#include "ui_inventorywindow.h"
#include "Lumber.cpp"
#include "htmlgendialog.h"

#include "modifyitemdialog.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <QMessageBox>
#include <QFileDialog>




#include <math.h>

#define SAW_KERF 0.125

class CutListItem : public Lumber {
private:
    std::string name;
    int quantity;
public:
    CutListItem()
        : Lumber() {
        name = "NULL";
        quantity = 0;
    }
    CutListItem(std::string l, std::string w, std::string t, std::string s, int g, std::vector <std::string> a, std::vector <std::string> aV, std::string n, int q)
        : Lumber() {
        length = l;
        width = w;
        thickness = t;
        species = s;
        grade = g;
        attributes = a;
        attributeValues = aV;
        name = n;
        quantity = q;
    }
    int getQuantity() {return quantity;}
};

// Generating a list of CutlistItems that the given Lumber item has matching attributes with (non-exclusive)
std::vector <int> findMatching(Lumber* item, std::vector <CutListItem> cutList) {
    std::vector <int> validItems;
    for (int i = 0; i < cutList.size(); i++) {
        CutListItem c = cutList.at(i);
        std::vector <float> iDimensions = item->getDimensions();
        std::vector <std::string> iAttributes = item->getAttributes();
        std::vector <std::string> iAttributeValues = item->getAttributeValues();

        std::vector <float> cDimensions = c.getDimensions();
        std::vector <std::string> cAttributes = c.getAttributes();
        std::vector <std::string> cAttributeValues = c.getAttributeValues();

        bool sameDims = iDimensions.at(1) == cDimensions.at(1) && iDimensions.at(2) == cDimensions.at(2);
        bool smallEnough = iDimensions[0] >= cDimensions[0];
        bool sameSpecies = item->getSpecies()== c.getSpecies();
        bool gradeMatch = item->getGrade() <= c.getGrade();
        int matchingAttributes = 0;
        for (int k = 0; k < cAttributes.size(); k++) {
            std::string cHeader = cAttributes.at(k);
            std::string cData = cAttributeValues.at(k);

            for (int l = 0; l < iAttributes.size(); l++) {
                std::string iHeader = iAttributes.at(l);
                std::string iData = iAttributeValues.at(l);

                if (iHeader == cHeader && iData == cData)
                    matchingAttributes++;
            }
        }
        if (sameDims && smallEnough && sameSpecies && gradeMatch && matchingAttributes == cAttributes.size()) {
            for(int j = 0; j < cutList.at(i).getQuantity(); j++)
                validItems.push_back(i);
        }
    }
    return validItems;
}

// Generates a list of all possible combinations of CutListItems to be lower or equal to a given sum and accounts for saw width.
// TODO: Add quantity ability for subsets, currently is one of each CutListItem instead of quantity based
std::vector <std::vector <int>> lumberSubset(std::vector <CutListItem> cutList, float sum, std::vector <std::vector <int>> set, int i) {
    if (i == cutList.size()) {
        float setSum = 0;
        for (int j : set.at(0)) {
            setSum += cutList.at(j).getDimensions().at(0);
        }
        setSum += (set.at(0).size() - 1) * SAW_KERF;
        if (setSum <= sum)
            return set;
        std::vector <std::vector <int>> empty;
        return empty;
    }

    std::vector <std::vector <int>> set1 = lumberSubset(cutList, sum, set, i + 1);
    set.at(0).push_back(i);
    std::vector <std::vector <int>> set2 = lumberSubset(cutList, sum, set, i + 1);

    std::vector <std::vector <int>> combinedSet;
    for (std::vector <int> j : set1)
        combinedSet.push_back(j);
    for (std::vector <int> j : set2)
        combinedSet.push_back(j);
    return combinedSet;
}

// Uses a subset-sum algorithm to determine the best CutListItems for any given Lumber item.
std::vector <std::vector <int>> bestFitItems(Lumber* item, std::vector <CutListItem> cutList) {
    std::vector <int> validItemsIndexes = findMatching(item, cutList);
    std::vector <CutListItem> validItems;
    for (int i : validItemsIndexes) {
        validItems.push_back(cutList.at(i));
    }
    float sum = item->getDimensions().at(0);
    std::vector <std::vector <int>> empty;
    std::vector <int> empty2;
    empty.push_back(empty2);
    std::vector <std::vector <int>> possibleSetsSub = lumberSubset(validItems, sum, empty, 0);
    std::vector <std::vector <int>> possibleSets;
    for (std::vector <int> i : possibleSetsSub) {
        std::vector <int> transformed;
        for (int j : i) {
            transformed.push_back(validItemsIndexes.at(j));
        }
        possibleSets.push_back(transformed);
    }
    return possibleSets;
}

// Calculates the price of a given set of cuts to the nearest inch.
float calculateCost(std::vector <CutListItem> cutList, std::vector <int> cutListIndexes, Piece* item) {
    float sum = 0;
    for (int i : cutListIndexes) {
        sum += cutList.at(i).getDimensions().at(0);
    }
    sum += (cutListIndexes.size() - 1) * SAW_KERF;
    if (sum == item->getDimensions().at(0))
        return item->getPrice();
    else
        return item->getPrice() * item->getDimensions().at(0) / ceil(sum + SAW_KERF);
}
float calculateCost(std::vector <CutListItem> cutList, std::vector <int> cutListIndexes, Bulk* item) {
    float sum = 0;
    for (int i : cutListIndexes) {
        sum += cutList.at(i).getDimensions().at(0);
    }
    sum += (cutListIndexes.size() - 1) * SAW_KERF;
    if (sum == item->getDimensions().at(0))
        return item->getPrice();
    else
        return item->getPrice() * item->getDimensions().at(0) / ceil(sum + SAW_KERF);
}
struct knapsack {
    float cost;
    std::vector <int> pieces;
    std::vector <std::vector <int>> bulks;
};
std::vector <std::vector <int>> recursiveBulkQuantity(int i, int count, std::vector <std::vector <int>> set, int quantity, std::vector <std::vector <int>> bulkConfig) {
    if (i == bulkConfig.size()) {
        return set;
    }
    std::vector <std::vector <int>> sets;
    for (int j = 0; count + j <= quantity; j++) {
        std::vector <std::vector <int>> tempSet = set;
        tempSet.at(0).push_back(j);
        std::vector <std::vector <int>> newSet = recursiveBulkQuantity(i + 1, count + j, tempSet, quantity, bulkConfig);
        for (std::vector <int> k : newSet) {
            sets.push_back(k);
        }
    }
    return sets;
}

std::vector <knapsack> recursiveKnapsack(int i, std::vector <int> bulkQuantities, std::vector <std::vector <std::vector <int>>> pieceConfigs, std::vector <std::vector <std::vector <int>>> bulkConfigs, std::vector <std::vector <float>> pieceCost, std::vector <std::vector <float>> bulkCost) {
    if (i >= pieceConfigs.size() + bulkConfigs.size()) {
        std::vector <knapsack> empty;
        knapsack zero;
        zero.cost = 0;
        empty.push_back(zero);
        return empty;
    }
    int newI = i;
    bool bulkSet = false;
    std::vector <knapsack> knapsacks;
    if (newI >= pieceConfigs.size()) {
        newI -= pieceConfigs.size();
        bulkSet = true;
    }
    std::vector <knapsack> prevKnapsacks = recursiveKnapsack(i + 1, bulkQuantities, pieceConfigs, bulkConfigs, pieceCost, bulkCost);
    if (!bulkSet) {
        for (int j = 0; j <= pieceConfigs.at(newI).size(); j++) {
            for (knapsack k : prevKnapsacks) {
                knapsack newKnapsack = k;
                if (j - 1 >= 0)
                    newKnapsack.cost += pieceCost.at(newI).at(j - 1);
                newKnapsack.pieces.push_back(j - 1);
                knapsacks.push_back(newKnapsack);
            }
        }
        return knapsacks;
    }
    else if (bulkSet) {
        std::vector <std::vector <int>> currentConfig = bulkConfigs.at(newI);
        std::vector <std::vector <int>> empty2d;
        std::vector <int> empty1d;
        empty2d.push_back(empty1d);
        std::vector <std::vector <int>> quantities = recursiveBulkQuantity(0, 0, empty2d, bulkQuantities.at(newI), currentConfig);
        for (knapsack k : prevKnapsacks) {
            for(int j = 0; j < quantities.size(); j++) {
                knapsack newKnapsack = k;
                for(int l = 0; l < quantities.at(j).size(); l++) {
                    newKnapsack.cost += bulkCost.at(newI).at(l) * quantities.at(j).at(l);
                }
                newKnapsack.bulks.push_back(quantities.at(j));
                knapsacks.push_back(newKnapsack);
            }
        }
        return knapsacks;
    }
    std::vector <knapsack> empty;
    return empty;
}
struct lumberList {
    std::vector <int> pieces;
    std::vector <std::vector <int>> pieceFulfillment;
    std::vector <int> bulks;
    std::vector <int> bulkQuantities;
    std::vector <std::vector <int>> bulkFulfillment;
};
// Determines the ideal subset of Pieces and Bulks that will fulfill the Cut List
lumberList cutListToLumberList(std::vector <Bulk*> bulkList, std::vector <Piece*> pieceList, std::vector <CutListItem> cutList) {
    std::vector <std::vector <float>> pieceCost;
    std::vector <std::vector <float>> bulkCost;
    std::vector <std::vector <std::vector <int>>> pieceConfigurations;
    std::vector <std::vector <std::vector <int>>> bulkConfigurations;

    for (Piece* i : pieceList) {
        pieceConfigurations.push_back(bestFitItems(i, cutList));
    }
    for (Bulk* i : bulkList) {
        bulkConfigurations.push_back(bestFitItems(i, cutList));
    }

    for (int i = 0; i < pieceConfigurations.size(); i++) {
        std::vector <float> empty;
        pieceCost.push_back(empty);
        for(std::vector <int> j : pieceConfigurations.at(i)) {
            pieceCost.at(i).push_back(calculateCost(cutList, j, pieceList.at(i)));
        }
    }

    for (int i = 0; i < bulkConfigurations.size(); i++) {
        std::vector <float> empty;
        bulkCost.push_back(empty);
        for(std::vector <int> j : bulkConfigurations.at(i)) {
            bulkCost.at(i).push_back(calculateCost(cutList, j, bulkList.at(i)));
        }
    }

    std::vector <int> cutListQuantities;
    std::vector <int> zeroCount;
    for (CutListItem i : cutList) {
        cutListQuantities.push_back(i.getQuantity());
        zeroCount.push_back(0);
    }
    std::vector <int> bulkQuantities;
    for (Bulk* i : bulkList) {
        bulkQuantities.push_back(i->getQuantity());
    }

    knapsack endSack;
    {
        std::vector <knapsack> possibleKnapsacks;
        {
            std::vector <knapsack> allKnapsacks = recursiveKnapsack(0, bulkQuantities, pieceConfigurations, bulkConfigurations, pieceCost, bulkCost);
            for (knapsack k : allKnapsacks) {
                std::vector <int> knapsackCount = zeroCount;
                for (int i = 0; i < k.pieces.size(); i++) {
                    int pieceIndex = k.pieces.size() - i - 1;
                    int configIndex = k.pieces.at(i);
                    if (configIndex == -1) {
                        continue;
                    }
                    for (int j : pieceConfigurations.at(pieceIndex).at(configIndex)) {
                        knapsackCount.at(j) += 1;
                    }
                }
                for (int i = 0; i < k.bulks.size(); i++) {
                    for (int j = 0; j < k.bulks.at(i).size(); j++) {
                        int bulkIndex = k.bulks.size() - i - 1;
                        for (int l = 0; l < bulkConfigurations.at(bulkIndex).at(j).size(); l++) {
                            // knapsack and bulk configs have inverted first index ranges
                            knapsackCount.at(bulkConfigurations.at(bulkIndex).at(j).at(l)) += k.bulks.at(i).at(j);
                        }
                    }
                }
                bool fulfills = true;
                for (int i = 0; i < knapsackCount.size(); i++) {
                    if (knapsackCount.at(i) < cutListQuantities.at(i))
                        fulfills = false;
                }
                if (fulfills)
                    possibleKnapsacks.push_back(k);
            }
        }
        int minIndex = 0;
        for (int i = 0; i < possibleKnapsacks.size(); i++) {
            if (possibleKnapsacks.at(i).cost < possibleKnapsacks.at(minIndex).cost) {
                minIndex = i;
            }
        }

        endSack = possibleKnapsacks.at(minIndex);
    }

    lumberList returnList;
    for (int i = 0; i < endSack.pieces.size(); i++) {
        int pieceIndex = endSack.pieces.size() - i - 1;
        int configIndex = endSack.pieces.at(i);
        if (configIndex == -1) {
            continue;
        }
        else {
            returnList.pieces.push_back(pieceIndex);
            std::vector <int> empty;
            returnList.pieceFulfillment.push_back(empty);
            for (int j : pieceConfigurations.at(pieceIndex).at(configIndex)) {
                returnList.pieceFulfillment.back().push_back(j);
            }
        }
    }
    for (int i = 0; i < endSack.bulks.size(); i++) {
        int bulkIndex = bulkConfigurations.size() - i - 1;
        std::vector <int> empty;
        int count = 0;
        std::vector <int> cutCount = zeroCount;
        for (int j = 0; j < endSack.bulks.at(i).size(); j++) {
            count += endSack.bulks.at(i).at(j);
        }
        if (count > 0) {
            returnList.bulks.push_back(bulkIndex);
            returnList.bulkQuantities.push_back(count);
            returnList.bulkFulfillment.push_back(empty);
            for (int j = 0; j < endSack.bulks.at(i).size(); j++) {
                for (int l = 0; l < endSack.bulks.at(i).at(j); l++) {
                    for (int k : bulkConfigurations.at(bulkIndex).at(j)) {
                        returnList.bulkFulfillment.back().push_back(k);
                    }
                }
            }
        }
    }
    return returnList;
}


















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

void InventoryWindow::on_cutlistButton_clicked()
{
    std::vector <std::string> empty;

    std::vector <CutListItem> cutList;
    CutListItem item1("0'-10\"","0'-5\"","1/1","Cherry",1,empty,empty,"item1",1);
    CutListItem item2("1'-4\"","0'-5\"","1/1","Cherry",1,empty,empty,"item2",1);
    CutListItem item3("0'-10\"","0'-5\"","1/1","Cherry",1,empty,empty,"item3",1);
    CutListItem item4("1'-3\"","0'-5\"","1/1","Cherry",1,empty,empty,"item4",1);
    cutList.push_back(item1);
    cutList.push_back(item2);
    cutList.push_back(item3);
    cutList.push_back(item4);

    std::vector <Piece*> pieceList;
    std::vector <Bulk*> bulkList;

    for(int i = 0; i < inventory.size(); i++){
        if(inventory[i]->getDescription() == "Piece"){
            pieceList.push_back(new Piece(inventory[i]->getAttributes(), inventory[i]->getAttributeValues()));
        }
        else if(inventory[i]->getDescription() == "Bulk"){
            bulkList.push_back(new Bulk(inventory[i]->getAttributes(), inventory[i]->getAttributeValues()));
        }
    }


    // Piece piece1(26,5,1,"cherry",1,empty,empty,5);
    // Piece piece2(16,5,1,"cherry",1,empty,empty,3);
    // pieceList.push_back(piece1);
    // pieceList.push_back(piece2);

    // Bulk bulk1(36,5,1,"cherry",1,empty,empty,1,12);
    // Bulk bulk2(24,5,1,"cherry",1,empty,empty,1,8);
    // bulkList.push_back(bulk1);
    // bulkList.push_back(bulk2);

    lumberList returnList = cutListToLumberList(bulkList, pieceList, cutList);
    for (int i = 0; i < returnList.pieces.size(); i++) {
        std::cout << "Piece #" << returnList.pieces.at(i) + 1 << " fulfills:" << std::endl;
        for (int j : returnList.pieceFulfillment.at(i))  {
            std::cout << "\tCut List Item #" << j + 1 << std::endl;
        }
    }
    for (int i = 0; i < returnList.bulks.size(); i++) {
        std::cout << returnList.bulkQuantities.at(i) << " of Bulk #" << returnList.bulks.at(i) + 1 << " fulfills:" << std::endl;
        for (int j : returnList.bulkFulfillment.at(i))  {
            std::cout << "\tCut List Item #" << j + 1 << std::endl;
        }
    }
}
