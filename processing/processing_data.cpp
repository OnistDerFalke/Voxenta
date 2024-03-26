#include "processing_data.h"

bool processing_data::compare(processing_data data) {
    if(data._intVal != this->_intVal) return false;
    if(data._floatVal != this->_floatVal) return false;
    if(data._doubleVal != this->_doubleVal) return false;
    return true;
}
