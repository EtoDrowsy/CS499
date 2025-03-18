#include "sortitemdialog.h"
#include "ui_sortitemdialog.h"

sortitemdialog::sortitemdialog(const std::vector<std::string>& attributes,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::sortitemdialog)
{
    ui->setupUi(this);
}

sortitemdialog::~sortitemdialog()
{
    delete ui;
}

int sortitemdialog::getSelectedSortColumn() const
{
    return ui->comboBox->currentIndex() == 0 ? 3:1;
}

void sortitemdialog::on_sortPushButton_clicked()
{
    accept();
}

void sortitemdialog::on_cancelPushButton_clicked()
{
    reject();
}

QString sortitemdialog::getSelectedColumn() const
{
    return ui->comboBox->currentText();
}
