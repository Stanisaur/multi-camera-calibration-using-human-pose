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
    QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    QCameraDevice defaultCamera = QMediaDevices::defaultVideoInput();
    std::string rtm_detnano_onnx_path = "/home/stanisaur/Documents/qtprojects/3DPoseInput/PoseEstimation/models/rtmdet-389d3a.onnx";
    std::string rtm_pose_onnx_path = "/home/stanisaur/Documents/qtprojects/3DPoseInput/PoseEstimation/models/rtmpose-s_simcc-body7_pt-body7_420e.onnx";
    void processAndDisplayImage(int, const QImage&);
private slots:
    void on_pushButton_clicked();
    void closeCell(int);
    void capturePose();
    void on_startPoseTracking_clicked();

private:
    Ui::MainWindow *ui;
    //just keeps track of whether or not camera is being used at a given time, uses device id
    //for simplification and to ensure no room for error with Qt's sus as shiet librarys that may be unpredictable
    //0: not being used, 1: being used
    std::unordered_map<int, int> cameraUsage;
    std::unique_ptr<RTMPoseTrackerOnnxruntime> rtmpose_tracker_onnxruntime;
    cv::VideoCapture cap;
    QTimer *timer;
    void addNewCameraCell();

};
#endif // MAINWINDOW_H
