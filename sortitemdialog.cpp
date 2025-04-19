#include "sortitemdialog.h"
#include "ui_sortitemdialog.h"

sortitemdialog::sortitemdialog(const std::vector<std::string>& attributes,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::sortitemdialog)
    , attNames(attributes)
{
    ui->setupUi(this);

    ui->comboBox->clear();
    for(const std::string& attr : attNames) {
        ui->comboBox->addItem(QString::fromStdString(attr));
    }
}

sortitemdialog::~sortitemdialog()
{
    delete ui;
}

int sortitemdialog::getSelectedSortColumn() const
{
    QString selected = ui->comboBox->currentText();

    auto item = std::find(attNames.begin(), attNames.end(), selected.toStdString());

    if (item != attNames.end()) {
        return static_cast<int>(std::distance(attNames.begin(),item));
    }

    return -1;
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
