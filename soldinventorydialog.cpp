#include "soldinventorydialog.h"
#include "ui_soldinventorydialog.h"
#include <QIntValidator>
#include <QMessageBox>

soldinventorydialog::soldinventorydialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::soldinventorydialog)
{
    ui->setupUi(this);

    ui->idEdit->setValidator(new QIntValidator(0, 999999, this));
    ui->quantityEdit->setValidator(new QIntValidator(0,1000000, this));

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

int soldinventorydialog::getIDValue(){
    return ui->idEdit->text().toInt();
}

int soldinventorydialog::getQuantitySold(){
    return ui->quantityEdit->text().toInt();
}

std::string soldinventorydialog::getNoteString(){
    return ui->notesEdit->toPlainText().toStdString();
}
