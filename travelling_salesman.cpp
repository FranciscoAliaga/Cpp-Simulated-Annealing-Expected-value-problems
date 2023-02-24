#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>  // read file .csv
#include <limits>
#include <string>
#include <valarray>

#include "vector_valued_utils.hpp" 
#include "sa.hpp"                  

using integer = sa::integer;
using real = sa::real;

using tour   = sa::size_t_sequence;
using matrix = std::vector<std::vector<real>>;

class TSP : public sa::ExpectedValueProblem<tour,real> {
    public: 
        virtual real valueFunction(const tour& x, const real& z) {
            real tour_value = 0. + z;
                for(size_t i = 0; i < x.size(); i++){
                    size_t now  = i;
                    size_t next = i+1 < x.size() ? i+1 : 0;
                    tour_value += distance_matrix[x[now]][x[next]];
                }
            return  tour_value;
            }
        virtual bool insideDomain(const tour& x){ // ignore warning, we will make sure domain is satisfied on the solver neighbor exploration.
            return true; 
        };
        virtual real RVSampler(){
            return 0.; // not even random
        };
    
    private:
        matrix distance_matrix;
    
    public:
        TSP(matrix distance_matrix) : distance_matrix{distance_matrix} {};

};

class TSPSolver : public sa::EVPSolver<tour,real> {
    public:
        virtual tour neighborHoodExplorer(const tour& x){
            tour y = x; // copy
            // randomly select two values to swap
            size_t first  = u(mt)%y.size();
            size_t second = u(mt)%(y.size()-1);
            if (second >= first) second+=1;
            y[first]  = x[second];
            y[second] = x[first];
            return y;
        }

        virtual real temperature(integer t){ 
            return -std::sqrt(static_cast<real>(t)); // negative, and should never be zero for t greater than or equal to 0.
        }

        // Needed
        using sa::EVPSolver<tour,real>::EVPSolver; // IMPORTANT! defaults to base constructor, otherwise cannot be constructed.

        void set_neighborhood_explorer_seed(unsigned int seed){
            mt.seed(seed);
        }


    private:
        std::uniform_int_distribution<unsigned long> u{0, std::numeric_limits<unsigned long>::max()}; // provides randomness functionality
        std::mt19937 mt{};                       //         
};

int main(int argc, char* argv[]){
    unsigned int acceptance_seed = 0;
    unsigned int exploration_seed = 0;
    if (argc >= 3){
        acceptance_seed = static_cast<unsigned int>(std::stoi(argv[1]));
        exploration_seed = static_cast<unsigned int>(std::stoi(argv[2]));
        }

    std::cout << "random acceptance seed:  " << acceptance_seed <<  std::endl ;
    std::cout << "random exploration seed: " << exploration_seed << '\n' << std::endl ;

    size_t cities = 8;
    tour starting_tour(cities); for(size_t i = 0; i < cities; i++) starting_tour[i] = i; // traverse in order.
    matrix distance_matrix = std::vector<std::vector<real>>(cities,std::vector<real>(cities,0.));

    // matrix taken from Jamal Benhra,
    // https://www.researchgate.net/publication/275947104_New_Genetic_Operator_Jump_Crossover_for_the_Traveling_Salesman_Problem

    std::ifstream csv_file("./tsp8cities.csv");
    std::string line;

    size_t row = 0;
    real val;
    while(std::getline(csv_file,line)){
        std::stringstream ss(line);
        size_t col = 0;
        while(ss >> val){
            distance_matrix[row][col] = val;
            if(ss.peek()==',') ss.ignore();
            col++;
        }
        row++;
    }

    std::cout << "distance matrix is : " << std::endl;

    for(size_t i = 0 ; i < cities ; i++){
        for(size_t j = 0 ; j < cities ; j++){
            std::cout << distance_matrix[i][j] << ' ';
        } std::cout << "\n";
    } std::cout << "\n" << std::endl;;

    /* Problem Declaration and solver run */

    TSP P{distance_matrix}; // initialize problem class

    TSPSolver S(        // initialize solver class
        P,              // with P as the target problem
        starting_tour,  // starting on x0
        1,              // using 20 values per each estimation of expected value at x
        acceptance_seed);          // and defined seed
    
    S.set_neighborhood_explorer_seed(exploration_seed);

    auto result = S.run(10000); // run the solver for several iterations.
    //auto result = S.run(1e6); // still < 1 sec on my machine
    //auto result = S.run(1e7); // takes ~2 secs on my machine
    std::cout << result;

    /* End  */

    tour answer = result.state;

    std::cout << "\ndistance steps: \n" ;
    for(size_t node = 0; node < answer.size();node++){
        size_t next = node + 1 != answer.size() ? node + 1 : 0;
        std::cout << '(' << answer[node] << " -> " << answer[next] << ") : " << distance_matrix[answer[node]][answer[next]] << "\n";
    }
    std::cout << "\n" << " total :  " << result.value << std::endl;

    return 0;
}
