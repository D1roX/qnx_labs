CC = gcc
CFLAGS = -fPIC -std=c99 -w
LDFLAGS = -shared
SRC = src/logger.c
TARGET = lib/liblogger.so

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -rf lib/