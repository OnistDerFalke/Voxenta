#include "processing_data.h"

bool processing_data::compare(processing_data data) {
    for(int i = 0; i<slots_per_type; i++)
    {
        if(data._intVal[i] != this->_intVal[i]) return false;
        if(data._floatVal[i] != this->_floatVal[i]) return false;
        if(data._doubleVal[i] != this->_doubleVal[i]) return false;
    }
    return true;
}

processing_data::processing_data()
{
    slots_per_type = 10;
    for(int i=0; i<slots_per_type; i++)
    {
        _intVal.push_back(0);
        _floatVal.push_back(0.0f);
        _doubleVal.push_back(0.0);
    }
    //description = nullptr;
}
