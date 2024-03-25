#ifndef VOXENTA_PROCESSING_UI_H
#define VOXENTA_PROCESSING_UI_H

#include <opencv2/core/mat.hpp>
#include "processing_data.h"

class processing_ui {
public:
    processing_ui();
    processing_data  run_method(int index);
private:
    processing_data  brightness();
    int _intMem;
};


#endif
