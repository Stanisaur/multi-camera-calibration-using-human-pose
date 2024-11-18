#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cameracell.h"
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QVector>
#include <QList>
#include <QCameraDevice>
#include <QMediaDevices>
#include <unordered_map>
#include <QElapsedTimer>
#include "PoseEstimation/rtmpose_tracker_onnxruntime.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int numCameraCells = 0;
    QVector<CameraCell *> allCellPtr;
    QCameraDevice defaultCamera = QMediaDevices::defaultVideoInput();
    std::string rtm_detnano_onnx_path = "../../PoseEstimation/models/rtmdet-389d3a.onnx";
    std::string rtm_pose_onnx_path = "../../PoseEstimation/models/rtmpose-s_simcc-body7_pt-body7_420e.onnx";
    void processAndDisplayImages();
private slots:
    void on_pushButton_clicked();
    void closeCell(int);
    void capturePose();
    void on_startPoseTracking_clicked();
    void updateLatency();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<RTMPoseTrackerOnnxruntime> rtmpose_tracker_onnxruntime;
    cv::VideoCapture cap;
    QTimer *mainTimer;
    QElapsedTimer *latencyTimer;
    long latency;
    QTimer *latencyUITimer;
    void addNewCameraCell();

};
#endif // MAINWINDOW_H
