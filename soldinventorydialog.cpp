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
}


soldinventorydialog::~soldinventorydialog()
{
    delete ui;
}
