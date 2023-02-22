
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

template <typename F>
struct solverAnswer{
    bool insideDomain = false;
    sa::real value    = sa::infinity;
    F    state;

    friend std::ostream& operator<<(std::ostream& out, const solverAnswer& result)
    {
        out << "Solution is inside domain: " << (result.insideDomain ? "true" : "false") << '\n'
            << "State: " << result.state << '\n'
            << std::setprecision(8)
            << "Value: " << result.value << std::endl;
        return out;
    }
};

namespace sa {
    // F is for deterministic decision, G for random variate.
    template <typename F,typename G> 
    class ExpectedValueProblem {
        public:
            virtual real valueFunction(const F& x, const G& z) = 0;
            virtual bool domainChecker(const F& x) = 0;
            virtual G    RVSampler() = 0;
            
            virtual ~ExpectedValueProblem(){};
    };

    template <typename F, typename G>
    class EVPSolver {
        private:
            const F x0;
            F m_x;      
            real m_value = infinity;

            std::uniform_real_distribution<real> U{0., 1.};
            std::mt19937 generator{};
        public:
            ExpectedValueProblem<F,G>& P;
            const integer batchSize = 1;

            virtual F    neighborHoodExplorer(const F& x) = 0;
            virtual real temperature (const integer n) {
                return -static_cast<real>(n);
            }
        
            EVPSolver(
                ExpectedValueProblem<F,G>& P,
                const F& x0,
                integer batchSize,
                uint_fast32_t seed = 0)  :
                x0 {x0}, m_x {x0}, P{P}, batchSize{batchSize}
                {
                    if (!P.domainChecker(x0)){
                        throw std::domain_error(
                            "EVPSolver: The starting value of class ExpectedValueProblem is not within it's own defined domain.");
                    }
                    generator.seed(seed); // initialize seed to mersenne twister
                }

            virtual ~EVPSolver(){};

            solverAnswer<F> run(integer iterations){

                // setup
                real current_value = eval(m_x);
                real best_value    = current_value;

                F best_x = m_x;
                F x      = m_x;

                // algorithm
                for(integer t = 0 ; t < iterations; t++){
                    // generate new candidate
                    F new_x = neighborHoodExplorer(x);
                    // if not on domain, try new iteration
                    if (!(P.domainChecker(new_x))) continue;
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
                bool is_in_domain = P.domainChecker(m_x);
                return {is_in_domain, best_value, m_x};
            }

            real eval(const F& x){
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