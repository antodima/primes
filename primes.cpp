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
        cerr<<"use: "<<argv[0]<<" number1 number2 [print=off|on]\n";
        return -1;
    }
    ull n1 = std::stoll(argv[1]);
    ull n2 = std::stoll(argv[2]);
    bool print_primes = false;
    if (argc >= 4)  print_primes = (std::string(argv[3]) == "on");

    std::vector<ull> results_seq;

    // Sequential
    ff::ffTime(ff::START_TIME);
    results_seq.reserve((size_t)(n2-n1)/log(n1));
    ull prime_seq;
    while((prime_seq=n1++) <= n2)
        if (is_prime(prime_seq)) results_seq.push_back(prime_seq);
    const size_t n = results_seq.size();
    cout<<"(Sequential) Found "<<n<<" primes\n";
    ff::ffTime(ff::STOP_TIME);
    if (print_primes) {
        for(size_t i=0;i<n;++i)
            cout<<results_seq[i] <<" ";
        cout<<"\n\n";
    }
    cout<<"(Sequential) Time: "<<ff::ffTime(ff::GET_TIME)<<" (ms)\n";

    // Parallel: Master-Worker Farm pattern
    

    // Parallel: ParallelFor pattern

    return 0;
}
