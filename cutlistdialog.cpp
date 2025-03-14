#include "cutlistdialog.h"
#include "ui_cutlistdialog.h"

cutlistdialog::cutlistdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::cutlistdialog)
{
    ui->setupUi(this);
}

cutlistdialog::~cutlistdialog()
{
    delete ui;
}

void cutlistdialog::on_rowsetbutton_clicked()
{
    ui->cuttable->setRowCount(ui->numberCutList->text().toInt() + 1);
}
