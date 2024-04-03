#include "processing_methods.h"

#include <utility>

processing_methods::processing_methods(){

    //Setting processing functions pointers, effect applied depends on chosen effect index
    functions.push_back(&processing_methods::brightness);
    functions.push_back(&processing_methods::contrast);
    functions.push_back(&processing_methods::negative);
    functions.push_back(&processing_methods::grayscale);
    functions.push_back(&processing_methods::binarization);
}

/* Runs the method assigned to the effect index, returns modified image */
cv::Mat processing_methods::run_method(cv::Mat image, int index, containers::processing_data data) {
    return (this->*functions[index])(std::move(image), std::move(data));
}

/* Example function using OpenCV */
cv::Mat processing_methods::brightness(cv::Mat image, containers::processing_data data) {
    cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                final_image.at<cv::Vec3b>(y,x)[c] =
                        cv::saturate_cast<uchar>( image.at<cv::Vec3b>(y,x)[c] + data.values.ints[0] );
            }
        }
    }
    return final_image;
}

/* Example function using OpenCV */
cv::Mat processing_methods::contrast(cv::Mat image, containers::processing_data data) {
    cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                final_image.at<cv::Vec3b>(y,x)[c] =
                        cv::saturate_cast<uchar>( data.values.floats[0] * (float)(image.at<cv::Vec3b>(y,x)[c] - 128) + 128);
            }
        }
    }
    return final_image;
}

/* Example function using OpenCV */
cv::Mat processing_methods::negative(cv::Mat image, containers::processing_data data) {
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

/* Example function using OpenCV */
cv::Mat processing_methods::grayscale(cv::Mat image, containers::processing_data data) {
    cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                float pixel_value = 0;
                for(int i = 0; i < 3; i++)
                    pixel_value += (float)image.at<cv::Vec3b>(y,x)[i] * data.values.floats[i];
                final_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( (int)pixel_value);
            }
        }
    }
    return final_image;
}

/* Example function using OpenCV */
cv::Mat processing_methods::binarization(cv::Mat image, containers::processing_data data) {
    cv::Mat final_image, grayscale_image;
    cv::cvtColor(image, grayscale_image, cv::COLOR_BGR2GRAY);
    if(data.values.bools[0]) grayscale_image = image;
    int mode = data.values.ints[0];

    if(mode == 0) cv::threshold(grayscale_image, final_image, data.values.ints[1], data.values.ints[2], cv::THRESH_BINARY);
    if(mode == 1) cv::threshold(grayscale_image, final_image, data.values.ints[1], data.values.ints[2], cv::THRESH_BINARY_INV);
    if(mode == 2) cv::threshold(grayscale_image, final_image, data.values.ints[1], 0, cv::THRESH_TOZERO);
    if(mode == 3) cv::threshold(grayscale_image, final_image, data.values.ints[1], 0, cv::THRESH_TOZERO_INV);
    if(mode == 4) cv::threshold(grayscale_image, final_image, data.values.ints[1], 0, cv::THRESH_TRUNC);
    if(mode == 5) cv::threshold(grayscale_image, final_image, 0, data.values.ints[2], cv::THRESH_OTSU);
    if(mode == 6) cv::threshold(grayscale_image, final_image, 0, data.values.ints[2], cv::THRESH_TRIANGLE);
    return final_image;
}
