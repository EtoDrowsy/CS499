#include "../header/soldinventoryviewer.h"
#include "../ui/ui_soldinventoryviewer.h"

std::vector<std::string> split (std::string row, std::string delim){
    std::vector<std::string> v;
    size_t x = row.find(delim);
    while (x!= std::string::npos) {
        v.push_back(row.substr(0,x));
        row = row.substr(x);
        row.erase(row.begin());
        x = row.find(delim);
    }
    v.push_back(row);
    return v;
}

soldinventoryviewer::soldinventoryviewer(std::vector<std::string> attributes, std::vector<Lumber*> soldinv, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::soldinventoryviewer)
{
    ui->setupUi(this);
    ui->tableWidget->clear();
    ui->searchComboBox->clear();
    soldarray.clear();

    attributes.push_back("Date Sold");
    attributes.push_back("Invoice Number");

    ui->tableWidget->setColumnCount(attributes.size());
    ui->tableWidget->setRowCount(soldinv.size() + 1);

    tableAttributes = attributes;

    for (int k = 0; k < attributes.size(); k++) {
        QString att = QString::fromStdString(attributes[k]);
        QTableWidgetItem *attq = new QTableWidgetItem(att);
        ui->tableWidget->setItem(0, k, attq);
    }

    std::string row;
    std::vector<std::string> rowvec;

    for (int i = 0; i < soldinv.size(); i++){
        row = soldinv[i]->toString();
        row.pop_back();
        rowvec = split(row,";");
        rowvec.push_back(soldinv[i]->getDateSold());
        rowvec.push_back(std::to_string(soldinv[i]->getInvoiceNumber()));
        soldarray.push_back(rowvec);
    }

    for (int i = 0; i < soldarray.size(); i++) {
        for (int j = 0; j < soldarray[i].size(); j++) {
            QString qstr = QString::fromStdString(soldarray[i][j]);
            QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
            ui->tableWidget->setItem(i + 1, j, newitem);
        }
    }

    for (int i = 0; i < tableAttributes.size(); i++){
        if (tableAttributes[i] != "Notes" && tableAttributes[i] != "Photo"){
            QString att = QString::fromStdString(tableAttributes[i]);
            ui->searchComboBox->addItem(att);
        }
    }

    ui->tableWidget->resizeColumnsToContents();
}

soldinventoryviewer::~soldinventoryviewer()
{
    delete ui;
}

void soldinventoryviewer::on_sortButton_clicked()
{
    int columnIndex = ui->tableWidget->currentColumn();
    std::string sortattr = tableAttributes[columnIndex];

    if (sortattr != "Photo" && sortattr != "Notes"){
        // Sorting based on the attribute
        std::sort(soldarray.begin(), soldarray.end(),
                  [columnIndex, this](const std::vector<std::string>& a, const std::vector<std::string>& b) {
                      if (tableAttributes[columnIndex] == "ID" || tableAttributes[columnIndex] == "Quantity" ||
                          tableAttributes[columnIndex] == "Length" || tableAttributes[columnIndex] == "Width" || tableAttributes[columnIndex] == "Thickness") {
                          try {
                              return std::stoi(a[columnIndex]) < std::stoi(b[columnIndex]);
                          } catch (...) {
                              return a[columnIndex] < b[columnIndex];
                          }
                      } else {
                          return a[columnIndex] < b[columnIndex];
                      }
                  });

        ui->tableWidget->setRowCount(soldarray.size() + 1);
        for (int i = 0; i < soldarray.size(); i++) {
            for (int j = 0; j < soldarray[i].size(); j++) {
                QString qstr = QString::fromStdString(soldarray[i][j]);
                QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
                ui->tableWidget->setItem(i + 1, j, newitem);
            }
        }
    }
}

void soldinventoryviewer::on_searchButton_clicked()
{
    int attributeIndex = 0;
    for (int i = 0; i < tableAttributes.size(); i++){
        if (tableAttributes[i] == ui->searchComboBox->currentText().toStdString()){
            attributeIndex = i;
            break;
        }
    }
    std::vector<std::vector<std::string>> searchedDataArray;
    ui->tableWidget->clear();

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int j = 0; j < soldarray.size(); j++){
        if (soldarray[j][attributeIndex] == ui->searchField->text().toStdString()){
            searchedDataArray.push_back(soldarray[j]);
        }
    }

    ui->tableWidget->setColumnCount(tableAttributes.size());
    ui->tableWidget->setRowCount(searchedDataArray.size() + 1);

    for (int k = 0; k < tableAttributes.size(); k++) {
        QString att = QString::fromStdString(tableAttributes[k]);
        QTableWidgetItem *attq = new QTableWidgetItem(att);
        ui->tableWidget->setItem(0, k, attq);
    }

    for (int i = 0; i < searchedDataArray.size(); i++) {
        for (int j = 0; j < searchedDataArray[i].size(); j++) {
            QString qstr = QString::fromStdString(searchedDataArray[i][j]);
            QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
            ui->tableWidget->setItem(i + 1, j, newitem);
        }
    }
    ui->tableWidget->resizeColumnsToContents();
    searchedDataArray.clear();
}


void soldinventoryviewer::on_clearSearchButton_clicked()
{
    ui->searchField->setText("");
    ui->tableWidget->clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->setColumnCount(tableAttributes.size());
    ui->tableWidget->setRowCount(soldarray.size() + 1);

    for (int k = 0; k < tableAttributes.size(); k++) {
        QString att = QString::fromStdString(tableAttributes[k]);
        QTableWidgetItem *attq = new QTableWidgetItem(att);
        ui->tableWidget->setItem(0, k, attq);
    }

    for (int i = 0; i < soldarray.size(); i++) {
        for (int j = 0; j < soldarray[i].size(); j++) {
            QString qstr = QString::fromStdString(soldarray[i][j]);
            QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
            ui->tableWidget->setItem(i + 1, j, newitem);
        }
    }
    ui->tableWidget->resizeColumnsToContents();
}

