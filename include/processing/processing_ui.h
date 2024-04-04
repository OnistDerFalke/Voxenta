#ifndef VOXENTA_PROCESSING_UI_H
#define VOXENTA_PROCESSING_UI_H

#include "utils/data_containers.h"

#include <opencv2/core/mat.hpp>

class processing_ui {
public:
    processing_ui();
    containers::processing_data run_method(int index);
private:
    std::vector<containers::processing_data (processing_ui::*)()> functions;
    containers::value_container ui_state;
    bool first_frame{};
    int last_index;

    //effect functions
    containers::processing_data brightness();
    containers::processing_data contrast();
    containers::processing_data negative();
    containers::processing_data grayscale();
    containers::processing_data binarization();
};

#endif
