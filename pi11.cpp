#include <iostream>
#include <random>
#include <upcxx/upcxx.hpp>

// we will assume this is always used in all examples
using namespace std;
default_random_engine generator;
uniform_real_distribution<> dist(0.0, 1.0);

int hit() {
  double x = dist(generator);
  double y = dist(generator);
  if (x*x + y*y <= 1.0) {
    return 1;
  } else {
    return 0;
  }
}


int main(int argc, char **argv) {
  upcxx::init();
  int hits, trials = 0;
  double pi;

  if (argc != 2) trials = 1000000;
  else trials = atoi(argv[1]);

  generator.seed(upcxx::rank_me()*17);
  // srand(upcxx::rank_me()*17);
  
  for (int i=0; i < trials; i++) hits += hit();
  pi = 4.0*hits/trials;
  //  cout << "Hits " << hits << " out of " << trials << " trials" << endl;
  cout << "PI estimated to " << pi << endl;
  upcxx::finalize();
}

