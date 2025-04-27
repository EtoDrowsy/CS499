/*
Author: Thomas Pierce
Source file for implementing help ui
*/

#include "../header/help.h"
#include "../ui/ui_help.h"

help::help(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::help)
{
    ui->setupUi(this);
}

help::~help()
{
    delete ui;
}
