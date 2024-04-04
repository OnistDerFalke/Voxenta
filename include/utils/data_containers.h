#ifndef VOXENTA_DATA_CONTAINERS_H
#define VOXENTA_DATA_CONTAINERS_H

#include <vector>

namespace containers {
    struct value_container {
        std::vector<int> ints{};
        std::vector<float> floats{};
        std::vector<double> doubles{};
        std::vector<bool> bools{};
        int data_size = 10;

        explicit value_container() {
            for (int i = 0; i < data_size; i++) {
                ints.push_back(0);
                floats.push_back(0.0f);
                doubles.push_back(0.0);
                bools.push_back(false);
            }
        }
    };

    struct processing_data {
        char* description{};
        value_container values;
      
        explicit processing_data()
        {
            for(int i=0; i<values.data_size; i++)
            {
                values.ints.push_back(0);
                values.floats.push_back(0.0f);
                values.doubles.push_back(0.0);
                values.bools.push_back(false);
            }
        }
        
        bool compare(processing_data data)
        {
            for(int i = 0; i<values.data_size; i++)
            {
                if(data.values.ints[i] != this->values.ints[i]) return false;
                if(data.values.floats[i] != this->values.floats[i]) return false;
                if(data.values.doubles[i] != this->values.doubles[i]) return false;
                if(data.values.bools[i] != this->values.bools[i]) return false;
            }
            return true;
        }
    };
}

#endif