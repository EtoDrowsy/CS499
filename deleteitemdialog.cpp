#include "deleteitemdialog.h"
#include "ui_deleteitemdialog.h"

DeleteItemDialog::DeleteItemDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteItemDialog)
{
    ui->setupUi(this);
}

DeleteItemDialog::~DeleteItemDialog()
{
    delete ui;
}

void DeleteItemDialog::updateDeleteSpinBox(int rowCount)
{
    ui->inputBox->setRange(1, rowCount);  // The valid range is from 1 to rowCount
}

int DeleteItemDialog::getInputId() const
{
    return ui->inputBox->value();  // Return the value selected in the spin box
}
