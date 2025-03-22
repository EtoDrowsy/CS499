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
    ui->inputTextEdit->setPlaceholderText(QString("Enter a number between 1 and %1").arg(rowCount));
}

int DeleteItemDialog::getInputId() const
{
    bool checkValid = false;
    int value = ui->inputTextEdit->toPlainText().toInt(&checkValid);

    if(checkValid){
        return value;
    }
    else{
        return -1;
    }
}
