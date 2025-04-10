#include "modifyitemdialog.h"
#include "ui_modifyitemdialog.h"

modifyitemdialog::modifyitemdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::modifyitemdialog)
{
    ui->setupUi(this);

    connect(ui->confirmInputBox, &QDialogButtonBox::accepted, this, &modifyitemdialog::accept);
    connect(ui->confirmInputBox, &QDialogButtonBox::rejected, this, &modifyitemdialog::reject);
}

modifyitemdialog::~modifyitemdialog()
{
    delete ui;
}

QString modifyitemdialog::getEnteredID() const
{
    return ui->idNum->text().trimmed();
}

QString modifyitemdialog::getNoteText() const
{
    return ui->inputTextEdit->toPlainText().trimmed();
}
