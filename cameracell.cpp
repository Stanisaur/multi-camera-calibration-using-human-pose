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
#include <QThread>
#include <QtMultimedia/qtmultimediaglobal.h>
#include "mainwindow.h"
#include "PoseEstimation/rtmpose_tracker_onnxruntime.h"
#include "PoseEstimation/rtmpose_utils.h"
#include "utils.h"

std::vector<std::pair<int, int>> coco_17_joint_links = {
    {0,1},{0,2},{1,3},{2,4},{5,7},{7,9},{6,8},{8,10},{5,6},{5,11},{6,12},{11,12},{11,13},{13,15},{12,14},{14,16}
};

CameraCell::CameraCell(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CameraCell)
{
    ui->setupUi(this);
    rtmpose_tracker_onnxruntime = std::make_unique<RTMPoseTrackerOnnxruntime>(rtm_detnano_onnx_path, rtm_pose_onnx_path, 40);
    ui->poseOutput->hide();
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
}

void CameraCell::setCamera()
{
    camera.reset(new QCamera(this));
    camera.get()->setCameraDevice(ui->camOptionsComboBox->currentData().value<QCameraDevice>());
    currentCameraCaptureSession.reset(new QMediaCaptureSession(this));
    currentCameraCaptureSession->setCamera(camera.data());
    currentCameraCaptureSession->setVideoOutput(ui->camFeedWidget);

    QCameraDevice device = ui->camOptionsComboBox->currentData().value<QCameraDevice>();

    std::cout << "Cell ID:" << cameraCellID << " connected to Camera:" << device.description().toStdString() << " ID:" << device.id().toStdString() << std::endl;

    camera->start();

}

void CameraCell::swapToPose(){
    // 1. Initialize our private sink
    m_captureSink.reset(new QVideoSink(this));

    // 2. DETACH from the UI Widget (Stops the raw video flicker)
    currentCameraCaptureSession->setVideoOutput(static_cast<QObject*>(nullptr));

    // 3. ATTACH to our internal sink (We now own the data)
    currentCameraCaptureSession->setVideoOutput(m_captureSink.data());

    ui->camFeedWidget->hide();
    ui->poseOutput->show();
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

void CameraCell::updateLatestCapture(){

    QImage frameImg = m_captureSink->videoFrame().toImage();

    if (frameImg.format() != QImage::Format_ARGB32 && frameImg.format() != QImage::Format_RGB32) {
        frameImg = frameImg.convertToFormat(QImage::Format_ARGB32);
    }

    latestCapture = qimage_to_mat_ref(frameImg, CV_8UC4).clone();
}

void CameraCell::displayOutput(cv::Mat frame){
    cv::Mat resized_frame = fitImageToCell(frame);
    QImage output_frame = mat_to_qimage_ref(resized_frame, QImage::Format_RGB32);
    ui->poseOutput->setPixmap(QPixmap::fromImage(output_frame));
}

CameraPoseResult CameraCell::extractPose() {
    CameraPoseResult result;

    auto output = rtmpose_tracker_onnxruntime->Inference(latestCapture);

    DetectBox box = output.first;
    auto points = output.second;
    auto frame = latestCapture.clone();

    cv::rectangle(frame, cv::Point(box.left, box.top),
                  cv::Point(box.right, box.bottom), cv::Scalar(255, 0, 0), 2);

    for (const auto& p : points) {
        if (p.score > 0.4){
            cv::circle(frame, cv::Point(p.x, p.y), 1, cv::Scalar(0, 0, 255), 5, cv::LINE_AA);
        }
    }

    for (int i = 0; i < coco_17_joint_links.size(); ++i)
    {
        std::pair<int, int> joint_links = coco_17_joint_links[i];

        if (points[joint_links.first].score > 0.4 & points[joint_links.second].score > 0.4){
            cv::line(
                frame,
                cv::Point(points[joint_links.first].x, points[joint_links.first].y),
                cv::Point(points[joint_links.second].x, points[joint_links.second].y),
                cv::Scalar{ 0, 255, 0 },
                2,
                cv::LINE_AA);
        }

    }

    result.cellID = cameraCellID;
    result.box = box;
    result.keypoints = points;
    result.originalFrame = frame;

    return result;
}
