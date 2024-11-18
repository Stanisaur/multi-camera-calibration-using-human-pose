#include "opencv2/opencv.hpp"
#include <QImage>

void mat_to_qimage(cv::InputArray image, QImage& out)
{
    switch(image.type())
    {
    case CV_8UC4:
    {
        cv::Mat view(image.getMat());
        QImage view2(view.data, view.cols, view.rows, view.step[0], QImage::Format_ARGB32);
        out = view2.copy();
        break;
    }
    case CV_8UC3:
    {
        cv::Mat mat;
        cvtColor(image, mat, cv::COLOR_BGR2BGRA); //COLOR_BGR2RGB doesn't behave so use RGBA
        QImage view(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
        out = view.copy();
        break;
    }
    case CV_8UC1:
    {
        cv::Mat mat;
        cvtColor(image, mat, cv::COLOR_GRAY2BGRA);
        QImage view(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
        out = view.copy();
        break;
    }
    default:
    {
        throw std::invalid_argument("Image format not supported");
        break;
    }
    }
}

QImage mat_to_qimage_ref(cv::Mat &mat, QImage::Format format)
{
    cv::Mat placeholder = mat;
    // mat.copyTo(placeholder);
    return QImage(placeholder.data, placeholder.cols, placeholder.rows,
                  static_cast<int>(placeholder.step), format);
}

cv::Mat qimage_to_mat_ref(const QImage &img, int format)
{
    cv::Mat placeholder(img.height(), img.width(),
                        format, (uchar*)img.constBits(), img.bytesPerLine());
    cv::Mat final;
    // placeholder.copyTo(final);
    return placeholder;
}


QImage Mat2QImage(cv::Mat const& src)
{
    cv::Mat temp; // make the same cv::Mat
    cvtColor(src, temp,cv::COLOR_RGB2BGR); // cvtColor Makes a copt, that what i need
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB32);
    dest.bits(); // enforce deep copy, see documentation
    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return dest;
}

cv::Mat QImage2Mat(QImage const& src)
{
    cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
    cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
    cvtColor(tmp, result,cv::COLOR_BGR2RGB);
    return result;
}
