# Makefile for numstat

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -lm
TARGET = numstat
SRC = numstat.c

.PHONY: all clean install uninstall test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)

test: $(TARGET)
	@echo "Testing basic functionality..."
	@echo "1 2 3 4 5" | ./$(TARGET)
	@echo "\nTesting JSON output..."
	@echo "1 2 3 4 5" | ./$(TARGET) -j
	@echo "\nTesting file input..."
	@./$(TARGET) data.txt
	@echo "\nAll tests completed!"
