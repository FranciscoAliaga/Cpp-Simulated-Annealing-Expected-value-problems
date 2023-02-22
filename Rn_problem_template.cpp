#include <iostream>
#include <iomanip>
#include <cmath>
#include <valarray>

// defines sa::real as long double (most precise floating point type in standard c++).
// defines sa::vecn "math vectors" (std::valarrays<real>). they can be element-wise operated with +, -, =, etc.
// provides stream out functionality (<<) for vecn.
#include "vector_valued_utils.hpp" 

// problem/solver templates
#include "sa.hpp"                  

using real = sa::real;
using vecn = sa::vecn;

class VecProblem : public sa::ExpectedValueProblem<vecn,vecn> {
    public: 
        virtual real valueFunction(const vecn& x, const vecn& z) {
            return x[0]*x[0] + x[1]*x[1] + 0.1 * z[0];  // <x,x> + random error
        }
        virtual bool domainChecker(const vecn& x){
            return
            (-50 <= x[0] && x[0] <= 50) &&              // domain is [-50, 50] x [-30, 30]
            (-30 <= x[1] && x[0] <= 30);                //
        };
        virtual vecn RVSampler(){
            return {gaussian(mt)};                      // random error is standard normal
        };

    private:
        std::normal_distribution<real> gaussian {0.,1.}; // provides randomness functionality
        std::mt19937 mt{};                               //

};

class VecSolver : public sa::EVPSolver<vecn,vecn> {
    public:
        virtual vecn neighborHoodExplorer(const vecn& x){
            return x + vecn{gaussian(mt),gaussian(mt)}; // searches in direction (N1,N2) with Ni being a standard normal
        }

        // Needed
        using sa::EVPSolver<vecn,vecn>::EVPSolver; // IMPORTANT! defaults to base constructor, otherwise cannot be constructed.

    private:
        std::normal_distribution<real> gaussian {0.,1.}; // provides randomness functionality
        std::mt19937 mt{};                               //         
};

int main(){
    unsigned int seed = 10;

    vecn x0 {10., 10.}; // starting state

    VecProblem P{};     // initialize problem class

    VecSolver S(        // initialize solver class
        P,              // with P as the target problem
        x0,             // starting on x0
        20,             // using 20 values per each estimation of expected value at x
        seed);          // and defined seed

    auto result = S.run(100);        // run the solver for 100 iterations
    std::cout << result;

    // variables provided by the solver:
    auto inside = result.insideDomain; // bool, checks if the result is within domain. (should always be true)
    auto value  = result.value;        // real, returns an estimation for the expected value problem.
    auto state  = result.state;        // X, is the value found by the simulated annealing method.

    std::cout << "within domain: " << inside << " ; value: " << value << " ; x: " << state << std::endl;

    return 0;
}
