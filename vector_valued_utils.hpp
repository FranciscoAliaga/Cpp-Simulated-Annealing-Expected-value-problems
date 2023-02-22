#include <iomanip>
#include <iostream>
#include <valarray>

#ifndef VECTOR_VALUED_UTILS
#define VECTOR_VALUED_UTILS

namespace sa {
    using real = long double;
    using vecn = std::valarray<real>;
};

std::ostream& operator<<(std::ostream& out, const sa::vecn& x){
    for(size_t i = 0 ; i < x.size(); i++){ out << ' ' << x[i] << ' '; }
    return out;
}


#endif