#include "processing_methods.h"

processing_methods::processing_methods() = default;

cv::Mat processing_methods::run_method(cv::Mat image, int index, processing_data data) {
    if(index == 0) return brightness(image, data._intVal[0]);
    if(index == 1) return contrast(image, data._floatVal[0]);
    if(index == 2) return negative(image);
    if(index == 3) return gaussian_blur(image, data._intVal[0], data._intVal[1], data._floatVal[0], data._floatVal[1], data._intVal[2]);
}

cv::Mat processing_methods::brightness(cv::Mat image, int val) {
    cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                final_image.at<cv::Vec3b>(y,x)[c] =
                        cv::saturate_cast<uchar>( image.at<cv::Vec3b>(y,x)[c] + val );
            }
        }
    }
    return final_image;
}

cv::Mat processing_methods::contrast(cv::Mat image, float val) {
    cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                final_image.at<cv::Vec3b>(y,x)[c] =
                        cv::saturate_cast<uchar>( val * (image.at<cv::Vec3b>(y,x)[c] - 128) + 128);
            }
        }
    }
    return final_image;
}

cv::Mat processing_methods::negative(cv::Mat image) {
    cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                final_image.at<cv::Vec3b>(y,x)[c] =
                        cv::saturate_cast<uchar>( 255 - image.at<cv::Vec3b>(y,x)[c]);
            }
        }
    }
    return final_image;
}

cv::Mat processing_methods::gaussian_blur(cv::Mat image, int kernel_width, int kernel_height, double sigma_x, double sigma_y, int border_type) {
    cv::Mat final_image = cv::Mat();
    cv::GaussianBlur(image, final_image,
                     cv::Size(kernel_width,kernel_height),
                     sigma_x,sigma_y, border_type);
    return final_image;
}
