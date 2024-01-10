#ifndef INCLUDED_UTILS
#define INCLUDED_UTILS

#include<string>
#include<bitset>
#include<mutex>

namespace utils {
    std::string padString(std::string const &s, int const size, char const padder);
    unsigned long long getNextBitPermutation(unsigned long long const& bitsAsULL);
    struct mutex_holder {
        std::mutex mutex;
        mutex_holder();
        mutex_holder(mutex_holder const& other);
    };
}

#endif