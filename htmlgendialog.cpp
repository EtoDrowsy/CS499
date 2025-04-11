#include "htmlgendialog.h"
#include "ui_htmlgendialog.h"

htmlgendialog::htmlgendialog(QWidget *parent, QStringList idList)
    : QDialog(parent)
    , ui(new Ui::htmlgendialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, qOverload<>(&QDialog::accept));
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, qOverload<>(&QDialog::reject));

    ui->listWidget->clear();

    ui->listWidget->addItems(idList);
    QListWidgetItem* item = 0;
    for(int i = 0; i < ui->listWidget->count(); ++i){
        item = ui->listWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

htmlgendialog::~htmlgendialog()
{
    delete ui;
}

std::vector<std::string> htmlgendialog::getCheckedItems(){
    std::vector<std::string> selectedItems;
    for(int i = 0; i < ui->listWidget->count(); ++i){
        if(ui->listWidget->item(i)->checkState() == Qt::Checked){
            selectedItems.push_back(ui->listWidget->item(i)->text().toStdString());
        }
    }
    return selectedItems;
}
