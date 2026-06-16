CXX = clang++
CXXFLAGS = -std=c++17 -O2 -Wall
TARGET = chess
SRCS = main.cpp board.cpp movegen.cpp eval.cpp search.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean