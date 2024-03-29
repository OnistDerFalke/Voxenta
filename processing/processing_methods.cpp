#include "processing_methods.h"

processing_methods::processing_methods(){
    //processing functions pointers
    functions.push_back(&processing_methods::brightness);
    functions.push_back(&processing_methods::contrast);
    functions.push_back(&processing_methods::negative);
    functions.push_back(&processing_methods::grayscale);
    functions.push_back(&processing_methods::binarization);
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

cv::Mat processing_methods::grayscale(cv::Mat image, processing_data data) {
    cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                float pixel_value = 0;
                for(int i = 0; i < 3; i++)
                    pixel_value += (float)image.at<cv::Vec3b>(y,x)[i] * data._floatVal[i];
                final_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( (int)pixel_value);
            }
        }
    }
    return final_image;
}

cv::Mat processing_methods::binarization(cv::Mat image, processing_data data) {
    cv::Mat final_image = cv::Mat();
    cv::Mat grayscale_image = cv::Mat();
    cv::cvtColor(image, grayscale_image, cv::COLOR_BGR2GRAY);
    if(data._intVal[0] == 0)
        cv::threshold(grayscale_image, final_image, data._intVal[1], data._intVal[2], cv::THRESH_BINARY);
    if(data._intVal[0] == 1)
        cv::threshold(grayscale_image, final_image, data._intVal[1], data._intVal[2], cv::THRESH_BINARY_INV);
    if(data._intVal[0] == 2)
        cv::threshold(grayscale_image, final_image, data._intVal[1], 0, cv::THRESH_TOZERO);
    if(data._intVal[0] == 3)
        cv::threshold(grayscale_image, final_image, data._intVal[1], 0, cv::THRESH_TOZERO_INV);
    if(data._intVal[0] == 4)
        cv::threshold(grayscale_image, final_image, data._intVal[1], 0, cv::THRESH_TRUNC);
    if(data._intVal[0] == 5)
        cv::threshold(grayscale_image, final_image, 0, data._intVal[2], cv::THRESH_OTSU);
    if(data._intVal[0] == 6)
        cv::threshold(grayscale_image, final_image, 0, data._intVal[2], cv::THRESH_TRIANGLE);
            return final_image;
}

cv::Mat processing_methods::gaussian_blur(cv::Mat image, processing_data data) {
    cv::Mat final_image = cv::Mat();
    cv::GaussianBlur(image, final_image,
                         cv::Size(data._intVal[0], data._intVal[1]),
                         data._doubleVal[0], data._doubleVal[1], data._intVal[2]);
    return final_image;
}
