#ifndef VOXENTA_PROCESSING_UI_H
#define VOXENTA_PROCESSING_UI_H

#include "utils/data_containers.h"

#include <opencv2/core/mat.hpp>
#include <variant>
#include <functional>

class processing_ui {
public:
    processing_ui();
    containers::processing_data run_method(int index);
private:
    std::vector<std::function<containers::processing_data(bool*, containers::value_container*)>> functions;
    containers::value_container ui_state;
    bool first_frame{};
    int last_index;
};

#endif
