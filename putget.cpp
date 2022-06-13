#include <iostream>
#include <random>
#include <upcxx/upcxx.hpp>

// we will assume this is always used in all examples
using namespace std;

int main(int argc, char **argv) {
  upcxx::init();

  //  upcxx::global_ptr<int> gptr = upcxx::new_<int>(upcxx::rank_me());
  // upcxx::broadcast(&gptr, 0);
  upcxx::global_ptr<int> gptr = upcxx::broadcast(upcxx::new_<int>(24),0).wait();


  cout << "From " << upcxx::rank_me() << " gptr points to ";
  cout << upcxx::rget(gptr).wait() << endl;
  // long hand version
  // upcxx::future<int> temp = upcxx::rget(gptr);
  // cout << temp.wait() << endl;
  upcxx::barrier();

  if (upcxx::rank_me()==0) upcxx::rput(19, gptr).wait();

  upcxx::barrier();

  cout << "From " << upcxx::rank_me() << " gptr points to ";
  cout << upcxx::rget(gptr).wait() << endl;
  upcxx::finalize();
}

