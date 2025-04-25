#include "../header/additemdialog.h"
#include "../ui/ui_additemdialog.h"
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QIntValidator>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

AddItemDialog::AddItemDialog(std::vector<std::string> attributes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);
    ui->attributetable->setRowCount(attributes.size());
    ui->attributetable->setColumnCount(1);
    ui->inputtable->setRowCount(attributes.size());
    ui->inputtable->setColumnCount(1);
    ui->inputtable->horizontalHeader()->setStretchLastSection(true);
    ui->attributetable->horizontalHeader()->setStretchLastSection(true);

    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        disconnect(okButton, nullptr, nullptr, nullptr); // remove any default connections
        connect(okButton, &QPushButton::clicked, this, &AddItemDialog::handleDialogAccept);
    }



    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    int rowIndex = 0;

    for (int i = 0; i < attributes.size(); i++) {
        QString att = QString::fromStdString(attributes[i]);

        QTableWidgetItem *attq = new QTableWidgetItem(att);
        ui->attributetable->setItem(rowIndex, 0, attq);

        QWidget *container = new QWidget(this);

        if (att.toLower() == "id" || att.toLower() == "quantity") {
            QLineEdit *lineEdit = new QLineEdit(container);
            lineEdit->setValidator(new QIntValidator(0, 9999999, this));
            lineEdit->setPlaceholderText(att);
            lineEdit->setGeometry(0, 0, 100, 25);
            container->setFixedSize(400, 25);
            ui->inputtable->setCellWidget(rowIndex, 0, lineEdit);
        }
        else if (att.toLower() == "location") {
            QLineEdit *lineEdit = new QLineEdit(container);
            QRegularExpression regex("[A-Za-z ]+");
            QValidator *validator = new QRegularExpressionValidator(regex, this);
            lineEdit->setValidator(validator);
            lineEdit->setPlaceholderText("Location");
            lineEdit->setGeometry(0, 0, 400, 25);
            container->setFixedSize(400, 25);
            ui->inputtable->setCellWidget(rowIndex, 0, lineEdit);
        }
        else if (att.toLower() == "length") {
            QLineEdit *feetLineEdit = new QLineEdit(container);
            feetLineEdit->setValidator(new QIntValidator(0, 999, this));
            feetLineEdit->setPlaceholderText("Feet");
            feetLineEdit->setGeometry(0, 0, 40, 30);

            QLabel *separator = new QLabel("-", container);
            separator->setGeometry(41, 5, 10, 15);

            QLineEdit *inchesLineEdit = new QLineEdit(container);
            inchesLineEdit->setValidator(new QIntValidator(0, 11, this));
            inchesLineEdit->setPlaceholderText("Inches");
            inchesLineEdit->setGeometry(49, 0, 50, 30);

            container->setFixedSize(400, 30);
            ui->inputtable->setCellWidget(rowIndex, 0, container);
        }
        else if (att.toLower() == "width") {
            QLineEdit *wholeNumberEdit = new QLineEdit(container);
            wholeNumberEdit->setValidator(new QIntValidator(0, 999, this));
            wholeNumberEdit->setPlaceholderText("Feet");
            wholeNumberEdit->setGeometry(0, 0, 40, 30);

            QLabel *separator = new QLabel("-", container);
            separator->setGeometry(41, 5, 10, 15);

            QLineEdit *fractionEdit = new QLineEdit(container);

            QRegularExpression fractionRegex("[0-9\\s/]*");
            fractionEdit->setValidator(new QRegularExpressionValidator(fractionRegex, this));
            fractionEdit->setPlaceholderText("Inches");
            fractionEdit->setGeometry(49, 0, 50, 30);

            container->setFixedSize(400, 30);
            ui->inputtable->setCellWidget(rowIndex, 0, container);
        }
        else if (att.toLower() == "thickness") {
            QLineEdit *lineEdit = new QLineEdit(container);
            lineEdit->setPlaceholderText("In 1/4s");

            QRegularExpression thicknessRegex("\\d+/\\d+|\\d+");
            lineEdit->setValidator(new QRegularExpressionValidator(thicknessRegex,this));

            lineEdit->setGeometry(0, 0, 400, 25);
            container->setFixedSize(400, 25);
            ui->inputtable->setCellWidget(rowIndex, 0, lineEdit);

        }
        else if (att.toLower() == "grade") {
            QLineEdit *lineEdit = new QLineEdit(container);
            lineEdit->setPlaceholderText("Grade 1 - 4");

            QRegularExpression gradeRegex("^[1-4]$");
            lineEdit->setValidator(new QRegularExpressionValidator(gradeRegex,this));

            lineEdit->setGeometry(0, 0, 400, 25);
            container->setFixedSize(400, 25);
            ui->inputtable->setCellWidget(rowIndex, 0, lineEdit);

        }
        else if (att.toLower() == "price") {
            QLineEdit *lineEdit = new QLineEdit(container);
            lineEdit->setPlaceholderText("Price $");

            QRegularExpression priceRegex("\\d*\\.?\\d*");
            lineEdit->setValidator(new QRegularExpressionValidator(priceRegex, this));

            connect(lineEdit, &QLineEdit::editingFinished, [lineEdit]() {
                QString text = lineEdit->text();
                if (!text.isEmpty() && !text.startsWith("$")){
                    lineEdit->setText("$" + text);
                }
            });

            lineEdit->setGeometry(0, 0, 400, 25);
            container->setFixedSize(400, 25);
            ui->inputtable->setCellWidget(rowIndex, 0, lineEdit);
        }
        else if (att.toLower() == "date acquired") {
            QDateEdit *dateEdit = new QDateEdit(container);
            dateEdit->setCalendarPopup(true);
            dateEdit->setDisplayFormat("yyyy-MM-dd");
            dateEdit->setDate(QDate::currentDate());

            dateEdit->setGeometry(0, 0, 400, 25);
            container->setFixedSize(400, 25);
            ui->inputtable->setCellWidget(rowIndex, 0, dateEdit);
        }
        else if (att.toLower() == "date cut") {
            QDateEdit *dateEdit = new QDateEdit(container);
            dateEdit->setCalendarPopup(true);
            dateEdit->setDisplayFormat("yyyy-MM-dd");
            dateEdit->setDate(QDate::currentDate());

            dateEdit->setGeometry(0, 0, 400, 25);
            container->setFixedSize(400, 25);
            ui->inputtable->setCellWidget(rowIndex, 0, dateEdit);
        }
        else if (att.toLower() == "photo") {
            QTableWidgetItem *photoItem = new QTableWidgetItem("N/A");
            photoItem->setFlags(photoItem->flags() & ~Qt::ItemIsEditable);
            ui->attributetable->setItem(rowIndex, 0, new QTableWidgetItem(att));
            ui->inputtable->setItem(rowIndex, 0, photoItem);
            rowIndex++;
            continue;
        }
        else if (att.toLower() == "notes") {
            QTableWidgetItem *notesItem = new QTableWidgetItem("N/A");
            notesItem->setFlags(notesItem->flags() & ~Qt::ItemIsEditable);
            ui->inputtable->setItem(rowIndex, 0, notesItem);

            QTableWidgetItem *attq = new QTableWidgetItem(att);
            ui->attributetable->setItem(rowIndex, 0, attq);

            rowIndex++;
            continue;
        }
        else if (att.toLower() == "name") {
            QLineEdit *lineEdit = new QLineEdit(container);
            QRegularExpression regex("[A-Za-z ]+");
            QValidator *validator = new QRegularExpressionValidator(regex, this);
            lineEdit->setValidator(validator);
            lineEdit->setPlaceholderText("Name");
            lineEdit->setGeometry(0, 0, 400, 25);
            container->setFixedSize(400, 25);
            ui->inputtable->setCellWidget(rowIndex, 0, lineEdit);
        }
        else {
            QLineEdit *lineEdit = new QLineEdit(container);
            lineEdit->setPlaceholderText(att);
            lineEdit->setGeometry(0, 0, 400, 25);
            container->setFixedSize(400, 25);
            ui->inputtable->setCellWidget(rowIndex, 0, lineEdit);
        }

        rowIndex++;
    }
}

std::vector<std::string> AddItemDialog::getNewItemData() const {
    std::vector<std::string> newItem;
    qDebug() << "Fetching data from inputtable. Rows:" << ui->inputtable->rowCount();

    for (int i = 0; i < ui->inputtable->rowCount(); i++) {
        QTableWidgetItem* item = ui->attributetable->item(i, 0);
        if (!item) {
            qDebug() << "Warning: attribute table missing item at row" << i;
            newItem.push_back("");
            continue;
        }

        QString attribute = item->text().toLower();
        QWidget *widget = ui->inputtable->cellWidget(i, 0);

        if (attribute == "length") {
            QList<QLineEdit *> lineEdits = widget ? widget->findChildren<QLineEdit *>() : QList<QLineEdit *>();
            if (lineEdits.size() == 2) {
                QString feetText = lineEdits[0]->text();
                QString inchesText = lineEdits[1]->text();
                QString lengthStr;

                if (!feetText.isEmpty()) {
                    lengthStr = feetText + "'";
                    if (!inchesText.isEmpty()) {
                        lengthStr += "-" + inchesText + "\"";
                    }
                }
                newItem.push_back(lengthStr.toStdString());
            } else {
                newItem.push_back("");
            }
        }
        else if (attribute == "width") {
            QList<QLineEdit*> widthEdits = widget ? widget->findChildren<QLineEdit*>() : QList<QLineEdit*>();
            if (widthEdits.size() >= 2) {
                QString wholePart = widthEdits[0]->text();
                QString fractionPart = widthEdits[1]->text().trimmed();

                QString widthStr;
                if (!wholePart.isEmpty()) {
                    widthStr = wholePart + "'";
                    if (!fractionPart.isEmpty()) {
                        widthStr += "-" + fractionPart;
                    }
                    widthStr += "\"";
                }
                newItem.push_back(widthStr.toStdString());
            } else {
                newItem.push_back("");
            }
        }
        else if (attribute == "thickness") {
            QString value;

            if (widget) {
                QLineEdit *lineEdit = widget->findChild<QLineEdit *>();
                if (!lineEdit) {
                    lineEdit = qobject_cast<QLineEdit *>(widget);
                }

                if (lineEdit) {
                    value = lineEdit->text().trimmed();
                }
            }

            if (!value.isEmpty()) {
                if (!value.endsWith("\"")) {
                    value += "\"";
                }
            } else {
                value = "N/A";
            }

            newItem.push_back(value.toStdString());
        }
        else if (attribute == "date") {
            QDateEdit *dateEdit = qobject_cast<QDateEdit *>(widget);
            if (dateEdit) {
                QString dateStr = dateEdit->date().toString("yyyy-MM-dd");
                newItem.push_back(dateStr.toStdString());
            }
            else {
                newItem.push_back("N/A");
            }
        }
        else {
            QString value;

            if (widget) {
                QLineEdit *lineEdit = widget->findChild<QLineEdit *>();
                if (!lineEdit) {
                    lineEdit = qobject_cast<QLineEdit *>(widget);
                }

                if (lineEdit) {
                    value = lineEdit->text();
                }
            }

            if (value.isEmpty()) {
                QTableWidgetItem *cellItem = ui->inputtable->item(i, 0);
                if (cellItem) {
                    value = cellItem->text();
                }
            }

            if (value.isEmpty()) {
                value = "N/A";
            }

            newItem.push_back(value.toStdString());
        }
    }


    qDebug() << "Final newItem size:" << newItem.size();
    return newItem;
}

bool AddItemDialog::validate() {
    std::set<QString> seenIDs;

    for (int i = 0; i < ui->inputtable->rowCount(); ++i) {
        QWidget* widget = ui->inputtable->cellWidget(i, 0); // Column 0: ID
        if (!widget) continue;

        QList<QLineEdit*> edits = widget->findChildren<QLineEdit*>();
        if (!edits.isEmpty()) {
            for (QLineEdit* lineEdit : edits) {
                QString value = lineEdit->text().trimmed();
                bool isPrice = lineEdit->property("isPrice").toBool();

                if (value.isEmpty() || (isPrice && value == "$")) {
                    lineEdit->setStyleSheet("border: 1px solid red");
                    QMessageBox::warning(this, "Error", "Please fill in all fields to add a valid item");
                    return false;
                } else {
                    lineEdit->setStyleSheet("");
                }
            }

            QString idValue = edits[0]->text().trimmed();
            if (seenIDs.find(idValue) != seenIDs.end()) {
                edits[0]->setStyleSheet("border: 1px solid red");
                QMessageBox::warning(this, "Error", "Duplicate ID found. Each item must have a unique ID.");
                return false;
            }
            seenIDs.insert(idValue);
        }
    }

    return true;
}

void AddItemDialog::handleDialogAccept() {
    if(validate()){
        accept();
    }
}

void AddItemDialog::resetStyles() {
    for (int i = 0; i < ui->attributetable->rowCount(); i++) {
        ui->attributetable->item(i, 0)->setBackground(Qt::white);
        if (auto widget = ui->inputtable->cellWidget(i, 0)) {
            widget->setStyleSheet("");
        }
    }
}

AddItemDialog::~AddItemDialog()
{
    delete ui;
}
