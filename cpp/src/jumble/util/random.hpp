#ifndef JUMBLE_UTIL_RANDOM_HPP_INCLUDED
#define JUMBLE_UTIL_RANDOM_HPP_INCLUDED

#include "jumble/util/common.hpp"
#include <random>
#include <ctime>
#include <utility>

JUMBLE_NAMESPACE_BEGIN

class Random {
public:
    static Random* getInstance() {
        static Random instance;
        return &instance;
    }

    // Return a random real number in [min, max)
    template<typename RealType = double>
    RealType nextReal(const RealType min = 0.0, const RealType max = 1.0) {
        return std::uniform_real_distribution<RealType>(min, max)(engine);
    }

    // Return a random integer in [min, max]
    template<typename IntType = int>
    IntType nextInt(const IntType min, const IntType max) {
        return std::uniform_int_distribution<IntType>(min, max)(engine);
    }

    /*
    Randomly shuffle the contents in a given iterator range.
    Reference: https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle.
    */
    template<typename RandIt>
    void shuffle(RandIt beg, RandIt end) {
        if (end - beg > 1) {
            auto i = end - beg - 1;
            for (RandIt it = end - 1; it != beg; --it, --i) {
                std::swap(*it, *(beg + nextInt<decltype(i)>(0, i)));
            }
        }
    }

private:
    Random() : engine((unsigned)std::time(nullptr)) {}

private:
    std::default_random_engine engine;
};

JUMBLE_NAMESPACE_END

#endif
