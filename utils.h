#ifndef UTILS_H
#define UTILS_H
#include "opencv2/opencv.hpp"
#include <QImage>

void mat_to_qimage(cv::InputArray image, QImage& out);
QImage Mat2QImage(cv::Mat const& src);
cv::Mat QImage2Mat(QImage const& src);
cv::Mat qimage_to_mat_ref(const QImage &img, int format);
QImage mat_to_qimage_ref(cv::Mat &mat, QImage::Format format);

#endif // UTILS_H
