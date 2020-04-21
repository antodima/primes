# https://github.com/fastflow/fastflow
FF_ROOT		= /usr/local/include/fastflow

CXX = g++ -std=c++17
INCLUDES = -I $(FF_ROOT)
CXXFLAGS = -g # -DNO_DEFAULT_MAPPING -DBLOCKING_MODE -DFF_BOUNDED_BUFFER

LDFLAGS = -pthread
OPTFLAGS = -O3 -finline-functions -DNDEBUG

TARGETS = primes

.PHONY: all clean cleanall
.SUFFIXES: .cpp

%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

all : $(TARGETS)
clean :
	rm -f $(TARGETS)
cleanall : clean
	\rm -f *.o *~
