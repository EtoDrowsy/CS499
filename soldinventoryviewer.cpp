#include "soldinventoryviewer.h"
#include "ui_soldinventoryviewer.h"

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

    ui->tableWidget->setColumnCount(attributes.size());
    ui->tableWidget->setRowCount(soldinv.size() + 1);

    for (int k = 0; k < attributes.size(); k++) {
        QString att = QString::fromStdString(attributes[k]);
        QTableWidgetItem *attq = new QTableWidgetItem(att);
        ui->tableWidget->setItem(0, k, attq);
    }

    std::vector<std::vector<std::string>> soldarray;
    std::string row;
    std::vector<std::string> rowvec;

    for (int i = 0; i < soldinv.size(); i++){
        row = soldinv[i]->toString();
        row.pop_back();
        rowvec = split(row,";");
        soldarray.push_back(rowvec);
    }

    for (int i = 0; i < soldarray.size(); i++) {
        for (int j = 0; j < soldarray[i].size(); j++) {
            QString qstr = QString::fromStdString(soldarray[i][j]);
            QTableWidgetItem *newitem = new QTableWidgetItem(qstr);
            ui->tableWidget->setItem(i + 1, j, newitem);
        }
    }
}

soldinventoryviewer::~soldinventoryviewer()
{
    delete ui;
}
