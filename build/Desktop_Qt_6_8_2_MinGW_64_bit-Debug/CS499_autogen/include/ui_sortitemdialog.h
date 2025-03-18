/********************************************************************************
** Form generated from reading UI file 'sortitemdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SORTITEMDIALOG_H
#define UI_SORTITEMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_sortitemdialog
{
public:
    QLabel *label;
    QComboBox *comboBox;
    QPushButton *sortPushButton;
    QPushButton *cancelPushButton;

    void setupUi(QDialog *sortitemdialog)
    {
        if (sortitemdialog->objectName().isEmpty())
            sortitemdialog->setObjectName("sortitemdialog");
        sortitemdialog->resize(400, 300);
        label = new QLabel(sortitemdialog);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 10, 351, 41));
        QFont font;
        font.setPointSize(16);
        label->setFont(font);
        comboBox = new QComboBox(sortitemdialog);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(110, 60, 151, 24));
        sortPushButton = new QPushButton(sortitemdialog);
        sortPushButton->setObjectName("sortPushButton");
        sortPushButton->setGeometry(QRect(70, 140, 71, 41));
        cancelPushButton = new QPushButton(sortitemdialog);
        cancelPushButton->setObjectName("cancelPushButton");
        cancelPushButton->setGeometry(QRect(260, 140, 71, 41));

        retranslateUi(sortitemdialog);

        QMetaObject::connectSlotsByName(sortitemdialog);
    } // setupUi

    void retranslateUi(QDialog *sortitemdialog)
    {
        sortitemdialog->setWindowTitle(QCoreApplication::translate("sortitemdialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("sortitemdialog", "How would you like to sort the table?", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("sortitemdialog", "Species", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("sortitemdialog", "Location", nullptr));

        sortPushButton->setText(QCoreApplication::translate("sortitemdialog", "Sort", nullptr));
        cancelPushButton->setText(QCoreApplication::translate("sortitemdialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sortitemdialog: public Ui_sortitemdialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SORTITEMDIALOG_H
