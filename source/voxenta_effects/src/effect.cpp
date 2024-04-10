#include "voxenta/effects/effect.h"

std::vector<effect*> g_effects;

effect::effect() {
    g_effects.emplace_back(this);
}

void effect::convert_to_rgb(cv::Mat* img) {
    int image_type = img->type();
    if (image_type == CV_8UC1) {
        cv::Mat converted_image;
        cv::cvtColor(*img, converted_image, cv::COLOR_GRAY2BGR);
        *img = converted_image;
    }
    else if (image_type == CV_8UC3) {
        cv::Mat converted_image;
        cv::cvtColor(*img, converted_image, cv::COLOR_BGR2RGB);
        *img = converted_image;
    }
    else if (image_type > CV_8UC3) {

    }
    else {
        fprintf(stderr, "unable to convert image \n");
    }
}

extern "C" {
    __attribute__((visibility("default")))
    std::vector<effect*>* voxenta_effects() {
        return &g_effects;
    }
}
