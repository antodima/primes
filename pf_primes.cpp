/**
ParallelFor pattern implementation of Finding primes.
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <mutex>
using namespace std;

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include <ff/utils.hpp>
using namespace ff;

using ull = unsigned long long;
std::mutex mtx;

// see http://en.wikipedia.org/wiki/Primality_test
static bool is_prime(ull n) {
  if (n <= 3)  return n > 1; // 1 is not prime !
  if (n % 2 == 0 || n % 3 == 0) return false;
  for (ull i = 5; i * i <= n; i += 6) {
    if (n % i == 0 || n % (i + 2) == 0)
      return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
      cerr<<"Usage: "<<argv[0]<<" number1 number2 nworkers\n";
      return -1;
  }
  ull n1 = std::stoll(argv[1]);
  ull n2 = std::stoll(argv[2]);
  int nw = std::atoi(argv[3]);

  vector<ull> results;
  results.reserve((size_t)(n2-n1)/log(n1));
  const size_t size = n2 - n1;

  ff::ffTime(ff::START_TIME);
  ParallelFor pf(nw);
  pf.parallel_for_idx(0, size, // start, stop indexes
                1, // step size
                0, // chunk size (0=static, >0=dynamic)
                [&](const ull n1, const ull n2, const long thid)  { // body
                    for (ull i=n1; i<n2; ++i) {
                      if (is_prime(i)) {
                        mtx.lock();
                        results.push_back(i);
                        mtx.unlock();
                      }
                    }
                });
  ff::ffTime(ff::STOP_TIME);

  const size_t n = results.size();
  cout<<"Found "<<n<<" primes\n";

  std::sort(results.begin(), results.end());
  for (ull e : results) cout<<e<<" ";
  cout<<endl;

  cout<<"Time: "<<ff::ffTime(ff::GET_TIME)<<" (ms)\n";

  return 0;
}
