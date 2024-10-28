/********************************************************************************
** Form generated from reading UI file 'cameracell.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERACELL_H
#define UI_CAMERACELL_H

#include <QtCore/QVariant>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CameraCell
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QComboBox *camOptionsComboBox;
    QVideoWidget *camFeedWidget;
    QPushButton *removeButton;

    void setupUi(QWidget *CameraCell)
    {
        if (CameraCell->objectName().isEmpty())
            CameraCell->setObjectName("CameraCell");
        CameraCell->resize(275, 260);
        horizontalLayout = new QHBoxLayout(CameraCell);
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        camOptionsComboBox = new QComboBox(CameraCell);
        camOptionsComboBox->setObjectName("camOptionsComboBox");

        verticalLayout->addWidget(camOptionsComboBox);

        camFeedWidget = new QVideoWidget(CameraCell);
        camFeedWidget->setObjectName("camFeedWidget");

        verticalLayout->addWidget(camFeedWidget);

        removeButton = new QPushButton(CameraCell);
        removeButton->setObjectName("removeButton");

        verticalLayout->addWidget(removeButton);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(CameraCell);

        QMetaObject::connectSlotsByName(CameraCell);
    } // setupUi

    void retranslateUi(QWidget *CameraCell)
    {
        CameraCell->setWindowTitle(QCoreApplication::translate("CameraCell", "Form", nullptr));
        removeButton->setText(QCoreApplication::translate("CameraCell", "remove cam", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CameraCell: public Ui_CameraCell {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERACELL_H
