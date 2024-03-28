#ifndef VOXENTA_PROCESSING_UI_H
#define VOXENTA_PROCESSING_UI_H

#include <opencv2/core/mat.hpp>
#include "processing_data.h"

class processing_ui {
public:
    processing_ui();
    processing_data run_method(int index);
private:
    std::vector<processing_data (processing_ui::*)()> functions;
    std::vector<int> _intMem{};
    std::vector<float> _floatMem{};
    std::vector<double> _doubleMem{};
    bool first_frame{};
    int last_index;

    //effect functions
    processing_data brightness();
    processing_data contrast();
    processing_data negative();
    processing_data grayscale();
    processing_data binarization();
    processing_data gaussian_blur();

};


#endif
