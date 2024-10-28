#include "cameracell.h"
#include "ui_cameracell.h"
#include <QCameraDevice>
#include <QCamera>
#include <QString>
#include <QVector>
#include <QVariant>
#include <QObject>
#include <QOverload>
#include <QImageCapture>
#include "mainwindow.h"

CameraCell::CameraCell(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CameraCell)
{
    ui->setupUi(this);
}

CameraCell::~CameraCell()
{
    delete ui;
}

void CameraCell::setCameraOptions(QCameraDevice &defaultCamDevice, QVector<QCameraDevice> &list)
{
    for (const QCameraDevice &camDevice : list) {
        ui->camOptionsComboBox->addItem(camDevice.description(), QVariant::fromValue(camDevice));
    }
    //now connect back to main window so that once a new ting is selected, main handler can pass the new cam feed to the algorithm
    // QObject::connect(ui->camOptionsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),)
}

void CameraCell::setCamera()
{
    camera.reset(new QCamera(ui->camOptionsComboBox->currentData().value<QCameraDevice>()));
    currentCameraCaptureSession.setCamera(camera.data());
    currentCameraCaptureSession.setVideoOutput(ui->camFeedWidget);
    imageCapture.reset(new QImageCapture);
    currentCameraCaptureSession.setImageCapture(imageCapture.get());

    camera->start();

}

void CameraCell::on_removeButton_clicked()
{
    emit closeThisCell(cameraCellID);
    this->close();
}

void CameraCell::on_camOptionsComboBox_currentIndexChanged(int index)
{
    //start a capture session now that camera has been selected, so that we can grab screenshots to make pose estimation etc
    setCamera();
}

