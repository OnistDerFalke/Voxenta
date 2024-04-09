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
};

inline std::vector<effect*> g_effects;

inline effect::effect() {
    g_effects.emplace_back(this);
}

#define REGISTER_EFFECT(effect_name) \
    namespace detail { static inline effect_name g_##effect_name; }

#endif
