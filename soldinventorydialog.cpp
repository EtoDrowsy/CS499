#include "soldinventorydialog.h"
#include "ui_soldinventorydialog.h"

soldinventorydialog::soldinventorydialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::soldinventorydialog)
{
    ui->setupUi(this);
}

soldinventorydialog::~soldinventorydialog()
{
    delete ui;
}
