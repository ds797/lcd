CC = g++
CVER = -std=c++23
CFLAGS = -Wall -g $(CVER)

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

TARGET = lcd

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.cpp tui.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)