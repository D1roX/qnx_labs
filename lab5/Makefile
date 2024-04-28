CC = gcc
RM = rm -f
LIBRARY_PATH = ../utils/logger/lib
CFLAGS = -I$(LIBRARY_PATH)
LDFLAGS = -L$(LIBRARY_PATH) -llogger -Wl,-rpath,$(LIBRARY_PATH)

TARGET = lab5

SRC_LAB = $(TARGET).c

all: $(TARGET)

$(TARGET): $(SRC_LAB:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.txt $(TARGET)
