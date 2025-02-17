/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *paradaButton;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_LedRed;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_LedRed;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_LedBlue;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_LedBlue;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_LedGreen;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_LedGreen;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(348, 289);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_8, 3, 2, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 3, 0, 1, 1);

        paradaButton = new QPushButton(centralwidget);
        paradaButton->setObjectName("paradaButton");

        gridLayout_2->addWidget(paradaButton, 3, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 4, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_4, 8, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_LedRed = new QHBoxLayout();
        horizontalLayout_LedRed->setObjectName("horizontalLayout_LedRed");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_LedRed->addItem(horizontalSpacer_3);

        pushButton_LedRed = new QPushButton(centralwidget);
        pushButton_LedRed->setObjectName("pushButton_LedRed");
        pushButton_LedRed->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_LedRed->addWidget(pushButton_LedRed);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_LedRed->addItem(horizontalSpacer_4);


        gridLayout->addLayout(horizontalLayout_LedRed, 1, 0, 1, 1);

        horizontalLayout_LedBlue = new QHBoxLayout();
        horizontalLayout_LedBlue->setObjectName("horizontalLayout_LedBlue");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_LedBlue->addItem(horizontalSpacer_5);

        pushButton_LedBlue = new QPushButton(centralwidget);
        pushButton_LedBlue->setObjectName("pushButton_LedBlue");
        pushButton_LedBlue->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_LedBlue->addWidget(pushButton_LedBlue);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_LedBlue->addItem(horizontalSpacer_6);


        gridLayout->addLayout(horizontalLayout_LedBlue, 2, 0, 1, 1);

        horizontalLayout_LedGreen = new QHBoxLayout();
        horizontalLayout_LedGreen->setObjectName("horizontalLayout_LedGreen");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_LedGreen->addItem(horizontalSpacer);

        pushButton_LedGreen = new QPushButton(centralwidget);
        pushButton_LedGreen->setObjectName("pushButton_LedGreen");
        pushButton_LedGreen->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_LedGreen->addWidget(pushButton_LedGreen);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_LedGreen->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_LedGreen, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 7, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 348, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        paradaButton->setText(QCoreApplication::translate("MainWindow", "PARADA DE EMERGENCIA", nullptr));
        pushButton_LedRed->setText(QCoreApplication::translate("MainWindow", "LAVADO RAPIDO", nullptr));
        pushButton_LedBlue->setText(QCoreApplication::translate("MainWindow", "CENTRIFUGADO", nullptr));
        pushButton_LedGreen->setText(QCoreApplication::translate("MainWindow", "LAVADO NORMAL", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
