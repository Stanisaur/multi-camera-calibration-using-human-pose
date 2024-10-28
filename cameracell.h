#ifndef CAMERACELL_H
#define CAMERACELL_H

#include <QWidget>
#include <QCameraDevice>
#include <QCamera>
#include <QVector>
#include <QMediaCaptureSession>
#include <QImageCapture>
#include <QScopedPointer>
#include "utils.h"

namespace Ui {
class CameraCell;
}

// struct CamOption
// {
//     CamOption(const QString &text, QCameraDevice &cameraDevice)
//         : name(cameraDevice.description())
//         , cameraDevice(cameraDevice)
//     {
//     }
//     QString name;
//     QCameraDevice cameraDevice;
// };

class CameraCell : public QWidget
{
    Q_OBJECT

public:
    explicit CameraCell(QWidget *parent = nullptr);
    ~CameraCell();
    int cameraCellID;
    QScopedPointer<QCamera> camera;
    QScopedPointer<QImageCapture> imageCapture;
    QMediaCaptureSession currentCameraCaptureSession;
    QImage latestCapture;

    void setCameraOptions(QCameraDevice &defaultCamDevice, QVector<QCameraDevice> &list);
    void updateLatestCapture(int requestID, const QImage&);
    void setCamera();
signals:
    void closeThisCell(int cellNumber);


private slots:
    void on_removeButton_clicked();
    void on_camOptionsComboBox_currentIndexChanged(int index);


private:
    Ui::CameraCell *ui;
};

#endif // CAMERACELL_H
