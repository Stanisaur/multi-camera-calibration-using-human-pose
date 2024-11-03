#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPoint>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QImageCapture>
#include <QImage>
#include <QPixmap>
#include <QElapsedTimer>
#include <chrono>
#include "utils.h"
#include <QtLogging>
#include "opencv2/opencv.hpp"
#include "PoseEstimation/rtmpose_tracker_onnxruntime.h"
#include "PoseEstimation/rtmpose_utils.h"
#include <QMessageBox>

std::vector<std::pair<int, int>> coco_17_joint_links = {
    {0,1},{0,2},{1,3},{2,4},{5,7},{7,9},{6,8},{8,10},{5,6},{5,11},{6,12},{11,12},{11,13},{13,15},{12,14},{14,16}
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    rtmpose_tracker_onnxruntime = std::make_unique<RTMPoseTrackerOnnxruntime>(rtm_detnano_onnx_path, rtm_pose_onnx_path, 40);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNewCameraCell() {
    CameraCell *newCameraCell = new CameraCell(this);
    ui->cameraGridLayout->addWidget(newCameraCell,numCameraCells/3,numCameraCells % 3);
    newCameraCell->setAttribute(Qt::WA_DeleteOnClose,true);
    newCameraCell->cameraCellID = numCameraCells;
    newCameraCell->setCameraOptions(defaultCamera, availableCameras);

    allCellPtr.append(newCameraCell);
    connect(newCameraCell,SIGNAL(closeThisCell(int)),this,SLOT(closeCell(int)));
    // connect(newCameraCell->imageCapture.get(), &QImageCapture::imageCaptured, this,
    //         &MainWindow::processAndDisplayImage);
    connect(newCameraCell->imageCapture.get(), &QImageCapture::imageCaptured, newCameraCell,
            &CameraCell::updateLatestCapture);
    numCameraCells++;
}

void MainWindow::on_pushButton_clicked()
{
    addNewCameraCell();
}

void MainWindow::closeCell(int cellNumber){
    allCellPtr[cellNumber]->close();
    for (int i=cellNumber+1;i<allCellPtr.length();i++){
        allCellPtr[i]->cameraCellID--;
        //have to remove and add from layout because qt doesnt let you move widgets within layout
        ui->cameraGridLayout->removeWidget(allCellPtr[i]);
        ui->cameraGridLayout->addWidget(allCellPtr[i], allCellPtr[i]->cameraCellID/3,allCellPtr[i]->cameraCellID%3);
    }
    numCameraCells--;
    allCellPtr.remove(cellNumber);
}

void MainWindow::processAndDisplayImages(){
    cv::Mat frame;

    for (int i = 0; i < numCameraCells; i++){
        QImage &input = allCellPtr[i]->latestCapture;
        //need to use CV_8UC4 to be compatible with Qimage format
        frame = qimage_to_mat_ref(input, CV_8UC4);
        if (frame.empty()){
            continue;
        }
        cv::resize(frame, frame, cv::Size(), 0.4, 0.4);
        std::pair<DetectBox, std::vector<PosePoint>> inference_box= rtmpose_tracker_onnxruntime->Inference(frame);
        DetectBox detect_box = inference_box.first;
        std::vector<PosePoint> pose_result = inference_box.second;

        cv::rectangle(
            frame,
            cv::Point(detect_box.left, detect_box.top),
            cv::Point(detect_box.right, detect_box.bottom),
            cv::Scalar{ 255, 0, 0 },
            2);

        if (!pose_result.empty()){
            for (int i = 0; i < pose_result.size(); ++i)
            {
                cv::circle(frame, cv::Point(pose_result[i].x, pose_result[i].y), 1, cv::Scalar{ 0, 0, 255 }, 5, cv::LINE_AA);
            }

            for (int i = 0; i < coco_17_joint_links.size(); ++i)
            {
                std::pair<int, int> joint_links = coco_17_joint_links[i];
                cv::line(
                    frame,
                    cv::Point(pose_result[joint_links.first].x, pose_result[joint_links.first].y),
                    cv::Point(pose_result[joint_links.second].x, pose_result[joint_links.second].y),
                    cv::Scalar{ 0, 255, 0 },
                    2,
                    cv::LINE_AA);
            }
        }





        cv::Mat resized_frame;
        QImage output_frame = mat_to_qimage_ref(frame, QImage::Format_RGB32);
        ui->imageLabel->setPixmap(QPixmap::fromImage(output_frame));
    }

}

void MainWindow::on_startPoseTracking_clicked()
{
    latencyTimer = new QElapsedTimer();
    mainTimer = new QTimer(this);
    latencyUITimer = new QTimer(this);
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(capturePose()));
    connect(latencyUITimer, SIGNAL(timeout()), this, SLOT(updateLatency()));
    mainTimer->start(20);
    latencyUITimer->start(1000);
    latencyTimer->start();
}

void MainWindow::capturePose(){
    latencyTimer->restart();

    for (int i = 0;i < numCameraCells; i++){
        if (allCellPtr[i]->imageCapture->isReadyForCapture()){
            allCellPtr[i]->imageCapture->capture();
        }
    }

    processAndDisplayImages();
    latency = std::chrono::duration_cast<std::chrono::milliseconds>(latencyTimer->durationElapsed()).count();
}

void MainWindow::updateLatency(){
    QString string = QString::number(latency);
    ui->menulatencyValue->setTitle(string);
}

