/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QGridLayout *cameraGridLayout;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout_2;
    QLabel *imageLabel;
    QPushButton *startPoseTracking;
    QMenuBar *menubar;
    QMenu *menu3DPoseInput;
    QMenu *menusetting_up;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(911, 638);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        cameraGridLayout = new QGridLayout();
        cameraGridLayout->setObjectName("cameraGridLayout");

        verticalLayout->addLayout(cameraGridLayout);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        imageLabel = new QLabel(centralwidget);
        imageLabel->setObjectName("imageLabel");

        verticalLayout_2->addWidget(imageLabel);

        startPoseTracking = new QPushButton(centralwidget);
        startPoseTracking->setObjectName("startPoseTracking");

        verticalLayout_2->addWidget(startPoseTracking);


        horizontalLayout->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 911, 22));
        menu3DPoseInput = new QMenu(menubar);
        menu3DPoseInput->setObjectName("menu3DPoseInput");
        menusetting_up = new QMenu(menubar);
        menusetting_up->setObjectName("menusetting_up");
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menu3DPoseInput->menuAction());
        menubar->addAction(menusetting_up->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Add Camera Input", nullptr));
        imageLabel->setText(QCoreApplication::translate("MainWindow", "horsing around wit a little ass lable you feel me g", nullptr));
        startPoseTracking->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        menu3DPoseInput->setTitle(QCoreApplication::translate("MainWindow", "3DPoseInput", nullptr));
        menusetting_up->setTitle(QCoreApplication::translate("MainWindow", "setting up...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
