#ifndef CAMERACELL_H
#define CAMERACELL_H

#include <QWidget>
#include <QCameraDevice>
#include <QCamera>
#include <QVector>
#include <QMediaCaptureSession>
#include <QImageCapture>
#include <QScopedPointer>
#include <QLabel>
#include <QVideoSink>
#include <QVideoFrame>
#include <QMutex>
#include <QQueue>
#include "opencv2/opencv.hpp"
#include "utils.h"

namespace Ui {
class CameraCell;
}

class CameraCell : public QWidget
{
    Q_OBJECT

public:
    explicit CameraCell(QWidget *parent = nullptr);
    ~CameraCell();
    int cameraCellID;
    QScopedPointer<QCamera> camera;
    QScopedPointer<QImageCapture> imageCapture;
    QScopedPointer<QMediaCaptureSession> currentCameraCaptureSession;
    static QMutex imageMutex;

    void setCameraOptions(QCameraDevice &defaultCamDevice, QVector<QCameraDevice> &list);
    void updateLatestCapture();
    void chungus(int requestID, const QImage&);
    QImage getLatestCapture();
    void displayOutput(cv::Mat);
    void setCamera();
    void swapToPose();
    cv::Mat fitImageToCell(cv::Mat);
signals:
    void closeThisCell(int cellNumber);


private slots:
    void on_removeButton_clicked();
    void on_camOptionsComboBox_currentIndexChanged(int index);


private:
    cv::Mat lastProcessedFrame;
    QImage latestCapture;
    QScopedPointer<QVideoSink> videoSink;
    Ui::CameraCell *ui;
    QQueue<QPair<QImage, qint64>> frameQueue;
    static const int MAX_QUEUE_SIZE = 2;  // Adjust as needed
    QTimer* processTimer;

};

#endif // CAMERACELL_H
