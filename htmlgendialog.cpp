#include "htmlgendialog.h"
#include "ui_htmlgendialog.h"

htmlgendialog::htmlgendialog(QWidget *parent, QStringList idList)
    : QDialog(parent)
    , ui(new Ui::htmlgendialog)
{
    ui->setupUi(this);

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
