CXX = g++
CXXFLAGS = -std=c++11 -I/path/to/pqxx/include
LDFLAGS = -L/path/to/pqxx/lib -lpqxx -lpq

your_program: postgres.cpp
    $(CXX) $(CXXFLAGS) -o postgres postgres.cpp $(LDFLAGS)
