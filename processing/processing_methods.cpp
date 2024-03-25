#include "processing_methods.h"

processing_methods::processing_methods() = default;

cv::Mat processing_methods::run_method(cv::Mat image, int index, processing_data data) {
    if(index == 0) return brightness(image, data._intVal);
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
