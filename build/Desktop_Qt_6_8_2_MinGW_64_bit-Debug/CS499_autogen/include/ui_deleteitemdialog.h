/********************************************************************************
** Form generated from reading UI file 'deleteitemdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELETEITEMDIALOG_H
#define UI_DELETEITEMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DeleteItemDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QPlainTextEdit *inputTextEdit;
    QLabel *label_2;

    void setupUi(QDialog *DeleteItemDialog)
    {
        if (DeleteItemDialog->objectName().isEmpty())
            DeleteItemDialog->setObjectName("DeleteItemDialog");
        DeleteItemDialog->resize(622, 337);
        buttonBox = new QDialogButtonBox(DeleteItemDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(230, 290, 341, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        label = new QLabel(DeleteItemDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 10, 391, 21));
        QFont font;
        font.setPointSize(16);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setMargin(0);
        inputTextEdit = new QPlainTextEdit(DeleteItemDialog);
        inputTextEdit->setObjectName("inputTextEdit");
        inputTextEdit->setGeometry(QRect(210, 130, 191, 51));
        label_2 = new QLabel(DeleteItemDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(110, 40, 391, 21));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2->setMargin(0);

        retranslateUi(DeleteItemDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, DeleteItemDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, DeleteItemDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(DeleteItemDialog);
    } // setupUi

    void retranslateUi(QDialog *DeleteItemDialog)
    {
        DeleteItemDialog->setWindowTitle(QCoreApplication::translate("DeleteItemDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("DeleteItemDialog", "Which item ID will be deleted from CSV?", nullptr));
        label_2->setText(QCoreApplication::translate("DeleteItemDialog", "(Must be valid ID number)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeleteItemDialog: public Ui_DeleteItemDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETEITEMDIALOG_H
