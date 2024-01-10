#include "utils.h"

namespace utils {
    std::string padString(std::string const &s, int const size, char const padder = '-') {
        int size_dif = size - s.size();
        std::string padding = "";
        if (size_dif > 0) {
            for (int i=0; i<size_dif; i++) {
                padding.push_back(padder);
            }
        }
        return s+padding;
    }

    unsigned long long getNextBitPermutation(unsigned long long const& bitsAsULL) {
        unsigned long long tmp = (bitsAsULL | (bitsAsULL - 1)) + 1;  
        return tmp | ((((tmp & -tmp) / (bitsAsULL & -bitsAsULL)) >> 1) - 1);
    }

    mutex_holder::mutex_holder():mutex(){}
    mutex_holder::mutex_holder(mutex_holder const& other):mutex(){}
}