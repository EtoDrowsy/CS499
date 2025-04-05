#include "additemdialog.h"
#include "ui_additemdialog.h"
#include <QLineEdit>
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

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [this]() {
        if (validate()) {
            this->accept();
        }
    });

    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    for (int i = 0; i < attributes.size(); i++) {
        QString att = QString::fromStdString(attributes[i]);
        QTableWidgetItem *attq = new QTableWidgetItem(att);
        ui->attributetable->setItem(i, 0, attq);

        // Input field widget
        QWidget *container = new QWidget(this);

        if (att.toLower() == "id" || att.toLower() == "quantity") {
            QLineEdit *lineEdit = new QLineEdit(container);
            lineEdit->setValidator(new QIntValidator(0, 9999999, this));
            lineEdit->setPlaceholderText(att);
            lineEdit->setGeometry(0, 0, 80, 25);
            container->setFixedSize(80, 25);
            ui->inputtable->setCellWidget(i, 0, lineEdit);
        }
        else if (att.toLower() == "location") {
            QLineEdit *lineEdit = new QLineEdit(container);
            QRegularExpression regex("[A-Za-z ]+");
            QValidator *validator = new QRegularExpressionValidator(regex, this);
            lineEdit->setValidator(validator);
            lineEdit->setPlaceholderText("Location");
            lineEdit->setGeometry(0, 0, 100, 25);
            container->setFixedSize(100, 25);
            ui->inputtable->setCellWidget(i, 0, lineEdit);
        }
        else if (att.toLower() == "length") {
            QLineEdit *feetLineEdit = new QLineEdit(container);
            feetLineEdit->setValidator(new QIntValidator(0, 999, this));
            feetLineEdit->setPlaceholderText("Feet");
            feetLineEdit->setGeometry(0, 0, 40, 25);

            QLabel *separator = new QLabel("-", container);
            separator->setGeometry(45, 5, 10, 15);

            QLineEdit *inchesLineEdit = new QLineEdit(container);
            inchesLineEdit->setValidator(new QIntValidator(0, 11, this));
            inchesLineEdit->setPlaceholderText("Inches");
            inchesLineEdit->setGeometry(60, 0, 40, 25);

            container->setFixedSize(100, 25);
            ui->inputtable->setCellWidget(i, 0, container);
        }
        else if (att.toLower() == "width") {
            QLineEdit *wholeNumberEdit = new QLineEdit(container);
            wholeNumberEdit->setValidator(new QIntValidator(0, 999, this));
            wholeNumberEdit->setPlaceholderText("Feet");
            wholeNumberEdit->setGeometry(0, 0, 40, 25);

            QLabel *separator = new QLabel("-", container);
            separator->setGeometry(45, 5, 10, 15);

            QLineEdit *fractionEdit = new QLineEdit(container);

            QRegularExpression fractionRegex("[0-9\\s/]*");
            fractionEdit->setValidator(new QRegularExpressionValidator(fractionRegex, this));
            fractionEdit->setPlaceholderText("Inches");
            fractionEdit->setGeometry(60, 0, 40, 25);

            container->setFixedSize(100, 25);
            ui->inputtable->setCellWidget(i, 0, container);
        }
        else if (att.toLower() == "thickness") {
            QLineEdit *lineEdit = new QLineEdit(container);
            lineEdit->setPlaceholderText("In 1/4s");

            QRegularExpression thicknessRegex("\\d+/\\d+|\\d+");
            lineEdit->setValidator(new QRegularExpressionValidator(thicknessRegex,this));

            lineEdit->setGeometry(0, 0, 80, 25);
            container->setFixedSize(80, 25);
            ui->inputtable->setCellWidget(i, 0, lineEdit);

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

            lineEdit->setGeometry(0, 0, 80, 25);
            container->setFixedSize(80, 25);
            ui->inputtable->setCellWidget(i, 0, lineEdit);
        }
        else {
            QLineEdit *lineEdit = new QLineEdit(container);
            lineEdit->setPlaceholderText(att);
            lineEdit->setGeometry(0, 0, 100, 25);
            container->setFixedSize(100, 25);
            ui->inputtable->setCellWidget(i, 0, lineEdit);
        }
    }
}

std::vector<std::string> AddItemDialog::getNewItemData() const {
    std::vector<std::string> newItem;
    qDebug() << "Fetching data from inputtable. Rows:" << ui->inputtable->rowCount();

    for (int i = 0; i < ui->inputtable->rowCount(); i++) {
        QString attribute = ui->attributetable->item(i, 0)->text().toLower();
        QWidget *widget = ui->inputtable->cellWidget(i, 0);

        if (!widget) {
            newItem.push_back("");
            continue;
        }

        if (attribute == "length") {
            QList<QLineEdit *> lineEdits = widget->findChildren<QLineEdit *>();
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
            QList<QLineEdit*> widthEdits = widget->findChildren<QLineEdit*>();
            if (widthEdits.size() >= 2) {
                QString wholePart = widthEdits[0]->text();
                QString fractionPart = widthEdits[1]->text().trimmed();

                QString widthStr;
                if (!wholePart.isEmpty()) {
                    widthStr = wholePart;
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
        else {
            QLineEdit *lineEdit = nullptr;

            lineEdit = widget->findChild<QLineEdit *>();

            if (!lineEdit) {
                lineEdit = qobject_cast<QLineEdit *>(widget);
            }

            if (lineEdit) {
                newItem.push_back(lineEdit->text().toStdString());
            } else {
                newItem.push_back("");
            }
        }
    }

    qDebug() << "Final newItem size:" << newItem.size();
    return newItem;
}

bool AddItemDialog::validate() {
    for (int i = 0; i < ui->inputtable->rowCount(); i++) {
        QWidget* widget = ui->inputtable->cellWidget(i, 0);
        if (!widget) continue;

        QList<QLineEdit*> edits = widget->findChildren<QLineEdit*>();
        if (edits.size() >= 2) {
            if (edits[0]->text().trimmed().isEmpty()) {
                QMessageBox::warning(this, "Error", "Please fill in all fields to add a valid item");
                return false;
            }
        }

        else {
            QLineEdit* lineEdit = widget->findChild<QLineEdit*>();
            if (lineEdit) {
                QString value = lineEdit->text().trimmed();
                bool isPrice = lineEdit->property("isPrice").toBool();

                if (value.isEmpty() || (isPrice && value == "$")) {
                    QMessageBox::warning(this, "Error", "Please fill in all fields to add a valid item");
                    return false;
                }
            }
        }
    }
    return true;
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
