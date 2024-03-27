#ifndef VOXENTA_PROCESSING_DATA_H
#define VOXENTA_PROCESSING_DATA_H

#include <vector>

class processing_data {
public:
    processing_data();
    std::vector<float> _floatVal;
    std::vector<int> _intVal;
    std::vector<double> _doubleVal;
    char* description{};
    int slots_per_type;
    bool compare(processing_data data);
private:
};


#endif
