CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

TARGET = slm

SRC_DIR = src

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/language_model.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)


.PHONY: all clean