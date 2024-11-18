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
#include <QMutexLocker>
#include <QThread>
#include <QMutex>
#include <QtMultimedia/qtmultimediaglobal.h>
#include "mainwindow.h"
#include "utils.h"



CameraCell::CameraCell(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CameraCell)
{
    ui->setupUi(this);
    ui->poseOutput->hide();
}

CameraCell::~CameraCell()
{
    delete ui;
}

QMutex CameraCell::imageMutex;

void CameraCell::setCameraOptions(QCameraDevice &defaultCamDevice, QVector<QCameraDevice> &list)
{
    for (const QCameraDevice &camDevice : list) {
        ui->camOptionsComboBox->addItem(camDevice.description(), QVariant::fromValue(camDevice));
    }

}

void CameraCell::setCamera()
{
    camera.reset(new QCamera(this));
    camera.get()->setCameraDevice(ui->camOptionsComboBox->currentData().value<QCameraDevice>());
    currentCameraCaptureSession.reset(new QMediaCaptureSession(this));
    currentCameraCaptureSession->setCamera(camera.data());
    currentCameraCaptureSession->setVideoOutput(ui->camFeedWidget);

    camera->start();

}

void CameraCell::swapToPose(){
    QMutexLocker locker(&imageMutex);

    if (!videoSink.isNull()){
        disconnect(videoSink.get(), &QVideoSink::videoFrameChanged, this,
                &CameraCell::updateLatestCapture);
    }

    videoSink.reset(new QVideoSink(this));
    //connect our processing function to videosink
    connect(ui->camFeedWidget->videoSink(), &QVideoSink::videoFrameChanged, videoSink.get(),
            &QVideoSink::setVideoFrame);
    connect(videoSink.get(), &QVideoSink::videoFrameChanged, this,
            &CameraCell::updateLatestCapture);
}

void CameraCell::updateLatestCapture(){
    QMutexLocker locker(&imageMutex);


    const QVideoFrame placeholder = ui->camFeedWidget->videoSink()->videoFrame();
    if (!placeholder.isValid()){
        return;
    }

    this->latestCapture = placeholder.toImage();

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

cv::Mat CameraCell::fitImageToCell(cv::Mat frame){
    double aspectRatio = frame.cols / (double)frame.rows;
    int targetWidth= ui->camFeedWidget->width();
    int targetHeight = static_cast<int>(targetWidth / aspectRatio);
    if (!frame.empty()){
        cv::resize(frame, frame, cv::Size(targetWidth, targetHeight));
    }
    return frame;
}

QImage CameraCell::getLatestCapture(){
    QMutexLocker locker(&imageMutex);
    return this->latestCapture;
}

void CameraCell::displayOutput(cv::Mat frame){
    cv::Mat resized_frame = fitImageToCell(frame);
    QImage output_frame = mat_to_qimage_ref(resized_frame, QImage::Format_RGB32);
    ui->poseOutput->setPixmap(QPixmap::fromImage(output_frame));
}
