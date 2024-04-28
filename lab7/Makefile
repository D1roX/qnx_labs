CC = gcc
RM = rm -f
LIBRARY_PATH = ../utils/logger/lib
CFLAGS = -I$(LIBRARY_PATH) -std=c99 -w
LDFLAGS = -L$(LIBRARY_PATH) -llogger -Wl,-rpath,$(LIBRARY_PATH)

TARGET = lab7

SRC_LAB = $(TARGET).c

all: $(TARGET)

$(TARGET): $(SRC_LAB:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.txt $(TARGET) ourmemory


# CC = gcc
# RM = rm -f
# CFLAGS = -std=c99 -w

# TARGET = lab7

# SRC_LAB = $(TARGET).c

# all: $(TARGET)

# $(TARGET): $(SRC_LAB:.c=.o) logger.o
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# %.o: %.c
# 	$(CC) -c $(CFLAGS) -o $@ $^

# clean:
# 	$(RM) *.o *.txt $(TARGET) ourmemory
