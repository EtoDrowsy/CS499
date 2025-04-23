#include "soldinventorydialog.h"
#include "ui_soldinventorydialog.h"
#include <QIntValidator>
#include <QMessageBox>

soldinventorydialog::soldinventorydialog(QWidget *parent, int id)
    : QDialog(parent)
    , ui(new Ui::soldinventorydialog)
{
    ui->setupUi(this);

    saleID = id;

    ui->idEdit->setText(QString::number(saleID));
    ui->quantityEdit->setValidator(new QIntValidator(0,1000000, this));
    ui->dateEdit->setDate(QDate::currentDate());

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}


soldinventorydialog::~soldinventorydialog()
{
    delete ui;
}

bool soldinventorydialog::getQuantityCheckState(){
    return ui->allSoldCheckBox->isChecked();
}

int soldinventorydialog::getQuantitySold(){
    return ui->quantityEdit->text().toInt();
}

std::string soldinventorydialog::getNoteString(){
    std::string retString = ui->notesEdit->toPlainText().toStdString();
    return retString;
}

std::string soldinventorydialog::getDateSold(){
    std::string retString = ui->dateEdit->text().toStdString();
    return retString;
}

int soldinventorydialog::getInvoiceNumber(){
    int retInt = ui->invoiceEdit->text().toInt();
    return retInt;
}
