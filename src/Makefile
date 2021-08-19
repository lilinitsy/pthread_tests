CXX = clang++
CFLAGS = -std=c++11 -O3 
LDFLAGS = -ldl -lpthread 

all: simple_pthread networked
networked: simple_pthread_server simple_pthread_client

simple_pthread_server: simple_pthread_server.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)

simple_pthread_client: simple_pthread_client.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)

simple_pthread: simple_pthread.o
	$(CXX) $(LDFLAGS) -o $(@) $(^)

.PHONY: test clean

test: simple_pthread
	./simple_pthread

clean:
	rm -f *.o simple_pthread simple_pthread_server simple_pthread_client

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@) $(<)