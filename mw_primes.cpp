/**
Master-Worker pattern implementation of Finding primes.
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;

#include <ff/ff.hpp>
#include <ff/farm.hpp>
#include <ff/utils.hpp>
using namespace ff;

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

struct Master : ff_node_t<ull> {
  Master(const ull n1, const ull n2) : length(n2-n1), n1(n1), n2(n2) {
    results.reserve((ull)(n2-n1)/log(n1));
  }

  ull* svc(ull *task) {
    if (task == nullptr) {
        for (size_t i=n1; i<n2; i++) {
          ff_send_out((ull*)i);
        }
        return GO_ON;
    }
    ull &t = *task;
    cout<<"Master received "<<t<<endl;
    results.push_back(t);
    delete task;
    if (++ntasks == length) return EOS;
    return GO_ON;
  }

  void svc_end() {
    const size_t n = results.size();
    cout<<"Found "<<n<<" primes\n";
    std::sort(results.begin(), results.end());
    for (ull e : results) cout<<e<<" ";
    cout<<endl;
  }

  const size_t length;
  size_t ntasks = 0;
  vector<ull> results;
  const ull n1, n2;
};

struct Worker: ff_node_t<ull> {
  ull* svc(ull *task) {
    cout<<"Worker svc"<<endl;
    ull &t = *task;
    cout<<"worker received "<<t<<"\n";
    if (is_prime((ull)t)) {
      return task;
    }
    else {
      cout<<"Pass"<<endl;
      return GO_ON;
    }
  }
};


int main(int argc, char *argv[]) {
  if (argc < 3) {
      cerr<<"Usage: "<<argv[0]<<" number1 number2 nworkers\n";
      return -1;
  }
  ull n1 = std::stoll(argv[1]);
  ull n2 = std::stoll(argv[2]);
  int nw  = std::atoi(argv[3]);

  Master master(n1, n2);
  vector<std::unique_ptr<ff_node>> workers;
  for(size_t i=0;i<nw;++i) workers.push_back(std::make_unique<Worker>());
  ff_Farm<ull> farm(std::move(workers), master);
  farm.remove_collector(); // needed because the collector is present by default in the ff_Farm
  farm.wrap_around(); // this call creates feedbacks from Workers to the Emitter
  //farm.set_scheduling_ondemand(); // optional

  ff::ffTime(ff::START_TIME);
  if (farm.run_and_wait_end() < 0) {
      error("running farm");
      return -1;
  }
  ff::ffTime(ff::STOP_TIME);
  cout<<"Time: "<<ff::ffTime(ff::GET_TIME)<<"\n";

  return 0;
}
