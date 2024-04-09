#include "processing/processing_methods.h"

#include "effects/effect_brightness.h"
#include "effects/effect_contrast.h"
#include "effects/effect_negative.h"
#include "effects/effect_grayscale.h"
#include "effects/effect_binarization.h"

#include <utility>

processing_methods::processing_methods() {
    std::vector<effect*> effects;
    effects.push_back(new effect_brightness());
    effects.push_back(new effect_contrast());
    effects.push_back(new effect_negative());
    effects.push_back(new effect_grayscale());
    effects.push_back(new effect_binarization());

    // Setting processing functions pointers, effect applied depends on chosen effect index
    for (auto& effect : effects) {
        functions.emplace_back([effect](cv::Mat img, containers::processing_data data) {
            return effect->pop_image(std::move(img), std::move(data));
        });
    }
}

/* Runs the method assigned to the effect index, returns modified image */
cv::Mat processing_methods::run_method(cv::Mat image, int index, containers::processing_data data) {
    auto& function_ptr = std::get<std::function<cv::Mat(cv::Mat, containers::processing_data)>>(functions[index]);
    return function_ptr(std::move(image), std::move(data));
}
