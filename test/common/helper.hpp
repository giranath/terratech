#ifndef DEF_COMMON_HELPERS_HPP
#define DEF_COMMON_HELPERS_HPP

namespace helper {

    template<typename T>
    T normalize(T x, T min, T max) {
        const T len = max - min;

        return (x - min) / len;
    }

    template<typename T>
    T clamp(T x, T min, T max) {
        if (max < x) {
            x = max;
        } else if (x < min) {
            x = min;
        }

        return x;
    }

}

#endif
