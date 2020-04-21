/**
Sequential implementation of Finding primes.
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ff/utils.hpp>

using namespace std;

using ull = unsigned long long;

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
      cerr<<"Usage: "<<argv[0]<<" number1 number2\n";
      return -1;
  }
  ull n1 = std::stoll(argv[1]);
  ull n2 = std::stoll(argv[2]);

  vector<ull> results;
  results.reserve((size_t)(n2-n1)/log(n1));

  ff::ffTime(ff::START_TIME);
  ull prime;
  while((prime=n1++) <= n2)
      if (is_prime(prime)) results.push_back(prime);
  ff::ffTime(ff::STOP_TIME);

  const size_t n = results.size();
  cout<<"Found "<<n<<" primes\n";

  for(size_t i=0;i<n;++i) cout<<results[i] <<" ";
  cout<<"\n\n";

  cout<<"Time: "<<ff::ffTime(ff::GET_TIME)<<" (ms)\n";

  return 0;
}
