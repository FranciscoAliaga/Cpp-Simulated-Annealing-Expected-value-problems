#include <cmath>
#include "sa.hpp"

class SellerProblem : public sa::ExpectedOptimizationProblem<sa::real,sa::real> {
    public: 
        using real = sa::real;
        virtual real valueFunction(const real& x, const real& z) {
            real iv = 5000.;
            real il = 1000.;
            real cc = 2000.;
            real sales = z > 0 ? std::min(x,z) : 0;
            real remaining = x - sales;
            real n_profits = 
                cc * x - iv * sales - il * remaining;
            return n_profits; 
        }

        virtual bool domainChecker(const real& x){return x > 0;};
        virtual real RVSampler(){return gaussian(mt);};
    
    private:
        real pv = 100.;
        real dv = 30.;
        std::normal_distribution<real> gaussian {100.,30.};
        std::mt19937 mt{};
};

class SellerSolver : public sa::EOPSolver<sa::real,sa::real> {
    using real = sa::real;
    private:
        std::normal_distribution<real> gaussian {0.,10.};
        std::mt19937 mt{};
    public:
        virtual real neighborHoodExplorer(const real& x){
            return x + gaussian(mt); }

        // defaults to base constructor, important! 
        using sa::EOPSolver<real,real>::EOPSolver;
};

int main(){
    using real = sa::real;
    real x0 = 100.;

    SellerProblem Q{};
    SellerSolver S(Q,x0,1000,12);

    auto result = S.run(10000);
    std::cout << result << "and that's all folks!";

    return 0;
}
