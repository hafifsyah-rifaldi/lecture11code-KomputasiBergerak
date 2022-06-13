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
  int my_hits, trials, my_trials = 0;
  double pi;

  if (argc != 2) trials = 1000000;
  else trials = atoi(argv[1]);

  my_trials = (trials + upcxx::rank_me()) / upcxx::rank_n();
  
  generator.seed(upcxx::rank_me()*17);

  upcxx::global_ptr<int> hits =
      upcxx::broadcast(upcxx::new_<int>(0), 0).wait();
  
  for (int i=0; i < my_trials; i++) {
    my_hits += hit();
  }
  int old_hits = upcxx::rget(hits).wait();
  upcxx::rput(old_hits + my_hits, hits).wait();

  upcxx::barrier();
  if (upcxx::rank_me() == 0) {
    pi = 4.0*upcxx::rget(hits).wait()/trials;
    cout << "PI estimated to " << pi << endl;
  }
  
  upcxx::finalize();
}

