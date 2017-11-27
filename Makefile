all : samples errors

samples: samples.o stats.o
	g++ -static -s -o samples samples.o stats.o
	
errors: errors.o stats.o
	g++ -static -s -o errors errors.o stats.o

samples.o: samples.cpp stats.hpp
	g++ -O2 -c samples.cpp
	
errors.o: errors.cpp stats.hpp
	g++ -O2 -c errors.cpp

stats.o: stats.cpp stats.hpp
	g++ -O2 -c stats.cpp

clean:
	rm samples && rm errors && rm *.o