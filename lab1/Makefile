CC = gcc
CFLAGS = -Wall -g

LOGGER = logger.c
TEST = test.c

OBJECTS = $(LOGGER:.c=.o) $(TEST:.c=.o)

TARGET = test_logger

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(LOGGER:.c=): $(LOGGER)
	$(CC) -c $(CFLAGS) -o $@.o $^

$(TEST:.c=): $(TEST)
	$(CC) -c $(CFLAGS) -o $@.o $^

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)
