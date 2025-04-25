#include "modifyitemdialog.h"
#include "ui_modifyitemdialog.h"

modifyitemdialog::modifyitemdialog(std::vector<Lumber*> inv, int id, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::modifyitemdialog)
{
    ui->setupUi(this);

    inventory = inv;
    modifyID = id;

    ui->idNum->setText(QString::number(modifyID));

    for(int i = 0; i < inventory.size(); i++) {
        if(inventory[i]->getID() == std::to_string(modifyID)) {
            QString displayString = QString::fromStdString(inventory[i]->getNotes());
            displayString.replace("<return>","\n");
            displayString.replace("<semicolon>",";");
            ui->inputTextEdit->setText(displayString);
            break;
        }
    }
    connect(ui->confirmInputBox, &QDialogButtonBox::accepted, this, &modifyitemdialog::accept);
    connect(ui->confirmInputBox, &QDialogButtonBox::rejected, this, &modifyitemdialog::reject);
}

modifyitemdialog::~modifyitemdialog()
{
    delete ui;
}

QString modifyitemdialog::getNoteText() const
{
    QString retString = ui->inputTextEdit->toPlainText().trimmed();
    retString.replace(";","<semicolon>");
    retString.replace("\n", "<return>");

    return retString;
}

