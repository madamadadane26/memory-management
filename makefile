CC = g++
CFLAGS = -std=c++11

EXEC = sim

.PHONY: all clean

all: $(EXEC)

$(EXEC): main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(EXEC)

clean:
	rm -f $(EXEC)
