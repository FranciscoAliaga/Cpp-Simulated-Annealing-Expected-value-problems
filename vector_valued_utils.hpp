#include <iomanip>
#include <iostream>
#include <valarray>

#ifndef VECTOR_VALUED_UTILS
#define VECTOR_VALUED_UTILS

// commmon mathematical types
namespace sa {
    using integer = unsigned int;
    using real = long double;
    using vecn = std::valarray<real>;
    using size_t_sequence = std::valarray<size_t>;
};

// needed in order to print out vecn using << operator
std::ostream& operator<<(std::ostream& out, const sa::vecn& x){
    for(size_t i = 0 ; i < x.size(); i++){ out << ' ' << x[i] << ' '; }
    return out;
}

// needed in order to print out vecn using << operator
std::ostream& operator<<(std::ostream& out, const sa::size_t_sequence& x){
    for(size_t i = 0 ; i < x.size(); i++){ out << ' ' << x[i] << ' '; }
    return out;
}


#endif