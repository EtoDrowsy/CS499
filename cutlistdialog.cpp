#include "cutlistdialog.h"
#include "ui_cutlistdialog.h"
#include "CutList.cpp"

cutlistdialog::cutlistdialog(std::vector<Lumber*> invvec, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::cutlistdialog)
{
    ui->setupUi(this);
    inventory = invvec;
}

cutlistdialog::~cutlistdialog()
{
    delete ui;
}

void cutlistdialog::on_rowsetbutton_clicked()
{
    ui->cuttable->setRowCount(ui->numberCutList->text().toInt() + 1);
}

void cutlistdialog::on_genmaterials_clicked()
{
    std::vector<std::vector<std::string>> tableContents;
    std::vector<std::string> row;
    for(int i = 1; i < ui->cuttable->rowCount(); i++){
        for(int j = 0; j < ui->cuttable->columnCount(); j++){
            row.push_back(ui->cuttable->item(i,j)->text().toStdString());
        }
        tableContents.push_back(row);
    }
    std::vector <std::string> empty;

    std::vector <CutListItem*> cutList;

    for(int k = 0; k < tableContents.size(); k++){
        CutListItem* item = new CutListItem(tableContents[k][1],tableContents[k][2],tableContents[k][3],tableContents[k][0],1,empty,empty,"",std::stoi(tableContents[k][4]));
        cutList.push_back(item);
    }

    // CutListItem* item1 = new CutListItem("0'-10\"","0'-5\"","1/1","Cherry",1,empty,empty,"item1",3);
    // CutListItem* item2 = new CutListItem("1'-4\"","0'-5\"","1/1","Cherry",1,empty,empty,"item2",1);
    // CutListItem* item3 = new CutListItem("0'-10\"","0'-5\"","1/1","Cherry",1,empty,empty,"item3",1);
    // CutListItem* item4 = new CutListItem("1'-3\"","0'-5\"","1/1","Cherry",1,empty,empty,"item4",1);

    returnVector = cutListToLumberList(getInventory(), cutList);

    ui->outputTextBrowser->setText(QString::fromStdString(printLumberList(returnVector, inventory, cutList)));
}

std::vector<Lumber*> cutlistdialog::getInventory(){
    return inventory;
}

std::vector<std::vector<int>> cutlistdialog::getReturnVector(){
    return returnVector;
}

void cutlistdialog::setReturnVector(std::vector<std::vector<int>> retVec){
    returnVector = retVec;
}
