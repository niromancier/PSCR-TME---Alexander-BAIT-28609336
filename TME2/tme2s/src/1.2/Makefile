TARGET	= a.out
CC	= g++
FLAGS	= --std=c++17 -Wall -Werror -Wextra -c -I.
SRCS	= $(wildcard *.cpp)
OBJS	= $(patsubst %.cpp,%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(FLAGS) -c $<

clean:
	@rm -rf $(TARGET) $(OBJS)

fmt:
	@clang-format -i --style=google $(SRCS) *.hpp
	
.PHONY: all clean fmt
