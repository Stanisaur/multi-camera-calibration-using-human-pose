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
#include <QQueue>
#include "opencv2/opencv.hpp"
#include "utils.h"
#include "PoseEstimation/rtmpose_tracker_onnxruntime.h"
#include "PoseEstimation/rtmpose_utils.h"

namespace Ui {
class CameraCell;
}

struct CameraPoseResult {
    int cellID;                 // To know which camera this result belongs to
    cv::Mat originalFrame;      // The frame (cloned)
    DetectBox box;              // The bounding box found
    std::vector<PosePoint> keypoints; // The skeleton points
};

class CameraCell : public QWidget
{
    Q_OBJECT

public:
    explicit CameraCell(QWidget *parent = nullptr);
    ~CameraCell();
    int cameraCellID;
    QScopedPointer<QCamera> camera;
    QScopedPointer<QMediaCaptureSession> currentCameraCaptureSession;
    std::unique_ptr<RTMPoseTrackerOnnxruntime> rtmpose_tracker_onnxruntime;
    std::string rtm_detnano_onnx_path = "../../PoseEstimation/models/rtmdet-389d3a.onnx";
    std::string rtm_pose_onnx_path = "../../PoseEstimation/models/rtmpose-s_simcc-body7_pt-body7_420e.onnx";

    void setCameraOptions(QCameraDevice &defaultCamDevice, QVector<QCameraDevice> &list);
    void updateLatestCapture();
    void chungus(int requestID, const QImage&);
    QImage getLatestCapture();
    void setCamera();
    void swapToPose();
    cv::Mat fitImageToCell(cv::Mat);
    CameraPoseResult extractPose();
signals:
    void closeThisCell(int cellNumber);

public slots:
    void displayOutput(cv::Mat);

private slots:
    void on_removeButton_clicked();
    void on_camOptionsComboBox_currentIndexChanged(int index);

private:
    cv::Mat lastProcessedFrame;
    cv::Mat latestCapture;
    QScopedPointer<QVideoSink> m_captureSink;
    Ui::CameraCell *ui;
    QTimer* processTimer;

};

#endif // CAMERACELL_H
