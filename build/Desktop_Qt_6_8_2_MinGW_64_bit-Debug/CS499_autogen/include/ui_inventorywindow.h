/********************************************************************************
** Form generated from reading UI file 'inventorywindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INVENTORYWINDOW_H
#define UI_INVENTORYWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InventoryWindow
{
public:
    QWidget *centralwidget;
    QPushButton *csvLoadButton;
    QPushButton *addObjectButton;
    QPushButton *deleteObjectButton;
    QTableWidget *dataViewer;
    QPushButton *createCSVButton;
    QMenuBar *menubar;
    QMenu *menuWood_Manager;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *InventoryWindow)
    {
        if (InventoryWindow->objectName().isEmpty())
            InventoryWindow->setObjectName("InventoryWindow");
        InventoryWindow->resize(1032, 600);
        InventoryWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        centralwidget = new QWidget(InventoryWindow);
        centralwidget->setObjectName("centralwidget");
        csvLoadButton = new QPushButton(centralwidget);
        csvLoadButton->setObjectName("csvLoadButton");
        csvLoadButton->setGeometry(QRect(10, 10, 91, 41));
        addObjectButton = new QPushButton(centralwidget);
        addObjectButton->setObjectName("addObjectButton");
        addObjectButton->setGeometry(QRect(120, 10, 91, 41));
        addObjectButton->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        deleteObjectButton = new QPushButton(centralwidget);
        deleteObjectButton->setObjectName("deleteObjectButton");
        deleteObjectButton->setGeometry(QRect(230, 10, 91, 41));
        dataViewer = new QTableWidget(centralwidget);
        dataViewer->setObjectName("dataViewer");
        dataViewer->setGeometry(QRect(10, 60, 1011, 481));
        dataViewer->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        createCSVButton = new QPushButton(centralwidget);
        createCSVButton->setObjectName("createCSVButton");
        createCSVButton->setGeometry(QRect(930, 10, 91, 41));
        InventoryWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(InventoryWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1032, 22));
        menuWood_Manager = new QMenu(menubar);
        menuWood_Manager->setObjectName("menuWood_Manager");
        InventoryWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(InventoryWindow);
        statusbar->setObjectName("statusbar");
        InventoryWindow->setStatusBar(statusbar);

        menubar->addAction(menuWood_Manager->menuAction());

        retranslateUi(InventoryWindow);

        QMetaObject::connectSlotsByName(InventoryWindow);
    } // setupUi

    void retranslateUi(QMainWindow *InventoryWindow)
    {
        InventoryWindow->setWindowTitle(QCoreApplication::translate("InventoryWindow", "InventoryWindow", nullptr));
        csvLoadButton->setText(QCoreApplication::translate("InventoryWindow", "Load CSV", nullptr));
        addObjectButton->setText(QCoreApplication::translate("InventoryWindow", "Add Object", nullptr));
        deleteObjectButton->setText(QCoreApplication::translate("InventoryWindow", "Delete Object", nullptr));
        createCSVButton->setText(QCoreApplication::translate("InventoryWindow", "Create CSV", nullptr));
        menuWood_Manager->setTitle(QCoreApplication::translate("InventoryWindow", "Wood Manager", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InventoryWindow: public Ui_InventoryWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INVENTORYWINDOW_H
