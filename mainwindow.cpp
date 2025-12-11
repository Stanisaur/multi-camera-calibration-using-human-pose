#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPoint>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QImageCapture>
#include <QImage>
#include <QFutureSynchronizer>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QPixmap>
#include <QElapsedTimer>
#include <chrono>
#include "utils.h"
#include <QtLogging>
#include <QThread>
#include "opencv2/opencv.hpp"
#include <QMessageBox>
#include <QThreadPool>
#include "PoseEstimation/rtmpose_tracker_onnxruntime.h"
#include "PoseEstimation/rtmpose_utils.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNewCameraCell() {
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    QList<QCameraDevice> availableCameras;
    bool flag = false;

    for (int i = 0; i < cameras.size(); i++){
        flag = false;
        for (int j = 0; j < allCellPtr.size(); j++){
            if (cameras.at(i).id() == allCellPtr[j]->camera->cameraDevice().id()){
                flag = true;
                break;
            }
        }
        if (!flag){
            availableCameras.append(cameras.at(i));
        }
    }
    CameraCell *newCameraCell = new CameraCell(this);
    ui->cameraGridLayout->addWidget(newCameraCell,numCameraCells/3,numCameraCells % 3);
    newCameraCell->setAttribute(Qt::WA_DeleteOnClose,true);
    newCameraCell->cameraCellID = numCameraCells;
    newCameraCell->setCameraOptions(defaultCamera, availableCameras);

    allCellPtr.append(newCameraCell);
    connect(newCameraCell,SIGNAL(closeThisCell(int)),this,SLOT(closeCell(int)));
    // connect(newCameraCell->imageCapture.get(), &QImageCapture::imageCaptured, newCameraCell,
    //         &CameraCell::updateLatestCapture);
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

    std::vector<CameraPoseResult> results(numCameraCells);
    QFutureSynchronizer<void> synchronizer;

    for (int i = 0; i < numCameraCells; i++) {
        allCellPtr[i]->updateLatestCapture();
    }

    for (int i = 0; i < numCameraCells; i++){
        synchronizer.addFuture(QtConcurrent::run(QThreadPool::globalInstance(), [=, &results](){
            results[i] = allCellPtr[i]->extractPose();
        }));
    }

    synchronizer.waitForFinished();

    for (const auto& res : results) {
        allCellPtr[res.cellID]->displayOutput(res.originalFrame);
    }

}

void MainWindow::on_startPoseTracking_clicked()
{
    latencyTimer = new QElapsedTimer();
    mainTimer = new QTimer(this);
    latencyUITimer = new QTimer(this);

    //we are using the cameracells as output for processed images so need to detach them from their respective video capture
    for (int i= 0; i < numCameraCells;i++){
        allCellPtr[i]->swapToPose();
        QThread::msleep(100);
    }

    connect(mainTimer, SIGNAL(timeout()), this, SLOT(capturePose()));
    connect(latencyUITimer, SIGNAL(timeout()), this, SLOT(updateLatency()));


    mainTimer->start(40);
    latencyUITimer->start(1000);
    latencyTimer->start();
}

void MainWindow::capturePose(){
    latencyTimer->restart();
    processAndDisplayImages();
    latency = std::chrono::duration_cast<std::chrono::milliseconds>(latencyTimer->durationElapsed()).count();
}

void MainWindow::updateLatency(){
    QString string = QString::number(latency);
    ui->menulatencyValue->setTitle(string);
}

