CXX = g++
CXXFLAGS = -std=c++17 -O2 -I Public -I /usr/include/eigen3

TARGET = a.out
SRCS = main.cpp $(wildcard Private/*.cpp)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)