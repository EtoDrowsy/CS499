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

    connect(ui->confirmInputBox, &QDialogButtonBox::accepted, this, &modifyitemdialog::accept);
    connect(ui->confirmInputBox, &QDialogButtonBox::rejected, this, &modifyitemdialog::reject);
}

void modifyitemdialog::setIdNoteMap(const QMap<QString, QString> &map)
{
    idNoteMap = map;

    for(auto item = idNoteMap.begin(); item != idNoteMap.end(); item++)
    {
        qDebug() << "Map key:" << item.key() << "note: " << item.value();
    }
}

modifyitemdialog::~modifyitemdialog()
{
    delete ui;
}

QString modifyitemdialog::getNoteText() const
{
    return ui->inputTextEdit->toPlainText().trimmed();
}

void modifyitemdialog::on_viewButton_clicked()
{
    for (int i = 0; i < inventory.size(); i++){
        if (inventory[i]->getID() == getNoteText().toStdString()){
            ui->inputTextEdit->setText(QString::fromStdString(inventory[i]->getNotes()));
        }
    }
}
