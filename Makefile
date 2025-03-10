# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++11 -O2

# Target executable name
TARGET = delta_hedging

# Source files
SOURCES = xbs_hedge.cpp bs_hedge.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = bs_hedge.h

# Default target: build and run
all: $(TARGET)
	./$(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
