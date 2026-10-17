CXXFLAGS = ... -MMD -MP
DEPS = $(OBJS:.o=.d)
-include $(DEPS)


CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

TARGET = slm

SRC_DIR = src

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/language_model.cpp $(SRC_DIR)/text_generator.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)


.PHONY: all clean