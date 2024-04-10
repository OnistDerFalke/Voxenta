#ifndef VOXENTA_EFFECT_H
#define VOXENTA_EFFECT_H

#include <vector>
#include <opencv2/opencv.hpp>

class effect {
public:
    virtual ~effect() = default;
    effect();

    virtual const char* get_name() = 0;
    virtual const char* get_description() = 0;
    virtual bool run_ui() = 0;
    virtual cv::Mat run(cv::Mat image) = 0;

protected:
    static void convert_to_rgb(cv::Mat* img);
};

extern "C" {
    std::vector<effect*>* voxenta_effects();
}

#endif
