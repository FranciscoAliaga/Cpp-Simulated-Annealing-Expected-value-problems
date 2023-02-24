
#include <algorithm> // reduce/generate parallel algorithms
#include <cmath>     // exponential and min
#include <exception>
#include <execution> // parallel execution policy
#include <iomanip>   // for ostream& overloading
#include <iostream>  // cout
#include <limits>    // infinity
#include <random>    // PNRG
#include <stdexcept>

#ifndef SIMULATED_ANNEALING
#define SIMULATED_ANNEALING

namespace sa {
    using real     = long double;
    using integer  = unsigned int;
    const real infinity = std::numeric_limits<real>::infinity();
};

namespace sa{
template <typename X>
struct solverAnswer{
    bool insideDomain = false;
    sa::real value    = sa::infinity;
    X    state;

    friend std::ostream& operator<<(std::ostream& out, const solverAnswer& result)
    {
        out << "Solution is inside domain: " << (result.insideDomain ? "true" : "false") << '\n'
            << "State: " << result.state << '\n'
            << std::setprecision(8)
            << "Value: " << result.value << std::endl;
        return out;
    }
};

};
namespace sa {
    // X is for deterministic decision, Z for random variate.
    template <typename X,typename Z> 
    class ExpectedValueProblem {
        public:
            virtual real valueFunction(const X& x, const Z& z) = 0;
            virtual bool insideDomain(const X& x) = 0;
            virtual Z    RVSampler() = 0;
            
            virtual ~ExpectedValueProblem(){};
    };

    template <typename X, typename Z>
    class EVPSolver {
        private:
            const X x0;
            X m_x;      
            real m_value = infinity;

            std::uniform_real_distribution<real> U{0., 1.};
            std::mt19937 generator{};
        public:
            ExpectedValueProblem<X,Z>& P;
            const integer batchSize = 1;

            virtual X    neighborHoodExplorer(const X& x) = 0;
            virtual real temperature (integer n) {
                return -static_cast<real>(n);
            }
        
            EVPSolver(
                ExpectedValueProblem<X,Z>& P,
                const X& x0,
                integer batchSize,
                uint_fast32_t acceptance_probability_seed = 0)  :
                x0 {x0}, m_x {x0}, P{P}, batchSize{batchSize}
                {
                    if (!P.insideDomain(x0)){
                        throw std::domain_error(
                            "EVPSolver: The starting value provided to the solver is not within ExpectedValueProblem defined domain (insideDomain(x0) returns false).");
                    }
                    generator.seed(acceptance_probability_seed); // initialize seed to mersenne twister
                }

            virtual ~EVPSolver(){};

            solverAnswer<X> run(integer iterations){

                // setup
                real current_value = eval(m_x);
                real best_value    = current_value;

                X best_x = m_x;
                X x      = m_x;

                // algorithm
                for(integer t = 0 ; t < iterations; t++){
                    // generate new candidate
                    X new_x = neighborHoodExplorer(x);
                    // if not on domain, try new iteration
                    if (!(P.insideDomain(new_x))) continue;
                    real new_val = eval(new_x);

                    // compare to previous solutions
                    if (new_val < best_value){ // best possible scenario
                        // update value and x
                        best_value = new_val; current_value = new_val;
                        best_x = new_x ; x = new_x;
                    } else if (new_val < current_value) { // found new candidate
                        current_value = new_val;
                        x           = new_x;
                    } else {
                        // new_val >= current_val,
                        // we only proceed with probability.
                        integer time = t+1; // (time cannot be 0, solves div0)
                        real p = std::min(static_cast<real>(1.),
                            std::exp(1./temperature(time)));
                        real q = U(generator);
                        if (q>p){
                            // update the current x with a _worse_ candidate
                            // note the probability of executing
                            // this block goes to 0 as time -> infinity.
                            current_value = new_val;
                            x           = new_x;
                        }
                    }
                }

                // clean up and finalization
                m_x = best_x;
                bool is_in_domain = P.insideDomain(m_x);
                return {is_in_domain, best_value, m_x};
            }

            real eval(const X& x){
                integer n = batchSize;
                std::vector<real> samples(n);
                std::generate(std::execution::par_unseq, samples.begin(), samples.end(),
                [&]() -> real {
                    return P.valueFunction(x,P.RVSampler());
                });
                real ans = std::reduce(std::execution::par_unseq, samples.begin(),samples.end());
                ans /= static_cast<real>(n);
                return ans;
            }

            
    };

};


#endif