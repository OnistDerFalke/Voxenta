#include "processing_methods.h"

processing_methods::processing_methods(){
    //processing functions pointers
    functions.push_back(&processing_methods::brightness);
    functions.push_back(&processing_methods::contrast);
    functions.push_back(&processing_methods::negative);
    functions.push_back(&processing_methods::gaussian_blur);
};

cv::Mat processing_methods::run_method(cv::Mat image, int index, processing_data data) {
    return (this->*functions[index])(image, data);
}

cv::Mat processing_methods::brightness(cv::Mat image, processing_data data) {
    cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                final_image.at<cv::Vec3b>(y,x)[c] =
                        cv::saturate_cast<uchar>( image.at<cv::Vec3b>(y,x)[c] + data._intVal[0] );
            }
        }
    }
    return final_image;
}

cv::Mat processing_methods::contrast(cv::Mat image, processing_data data) {
    cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                final_image.at<cv::Vec3b>(y,x)[c] =
                        cv::saturate_cast<uchar>( data._floatVal[0] * (float)(image.at<cv::Vec3b>(y,x)[c] - 128) + 128);
            }
        }
    }
    return final_image;
}

cv::Mat processing_methods::negative(cv::Mat image, processing_data data) {
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

cv::Mat processing_methods::gaussian_blur(cv::Mat image, processing_data data) {
    cv::Mat final_image = cv::Mat();
    cv::GaussianBlur(image, final_image,
                     cv::Size(data._intVal[0], data._intVal[1]),
                     data._doubleVal[0],data._doubleVal[1], data._intVal[2]);
    return final_image;
}
