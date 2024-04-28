CC = gcc
RM = rm -f
LIBRARY_PATH = ../utils/logger/lib
CFLAGS = -I$(LIBRARY_PATH) -std=c99 -w
LDFLAGS = -L$(LIBRARY_PATH) -llogger -Wl,-rpath,$(LIBRARY_PATH)

PROCESS = process
THREAD = thread

SRC_PROCESS = $(PROCESS).c
SRC_THREAD = $(THREAD).c

all: $(PROCESS) $(THREAD)

$(PROCESS): $(SRC_PROCESS:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(THREAD): $(SRC_THREAD:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.txt $(PROCESS) $(THREAD)
