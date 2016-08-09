CXX ?= g++
CXXFLAGS = -I. -std=c++11 -DNDEBUG
TARGET = test

$(TARGET):
	$(CXX) *.cpp -o $(TARGET) $(CXXFLAGS)

data:
	./genData.tcl

clean:
	@rm -rf test
