#include "modifyitemdialog.h"
#include "ui_modifyitemdialog.h"

modifyitemdialog::modifyitemdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::modifyitemdialog)
{
    ui->setupUi(this);

    connect(ui->idNum, &QLineEdit::textChanged, this, &modifyitemdialog::onIdChanged);
    connect(ui->confirmInputBox, &QDialogButtonBox::accepted, this, &modifyitemdialog::accept);
    connect(ui->confirmInputBox, &QDialogButtonBox::rejected, this, &modifyitemdialog::reject);
}

void modifyitemdialog::onIdChanged(const QString &text)
{
    qDebug() << "Id: " << text;
    QString trimId = text.trimmed();
    if (idNoteMap.contains(trimId)) {
        ui->inputTextEdit->setPlainText(idNoteMap[trimId]);
        qDebug() << "Note: " << idNoteMap[trimId];
    }
    else {
        ui->inputTextEdit->clear();
        qDebug() << "no note";
    }
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

QString modifyitemdialog::getEnteredID() const
{
    return ui->idNum->text().trimmed();
}

QString modifyitemdialog::getNoteText() const
{
    return ui->inputTextEdit->toPlainText().trimmed();
}
