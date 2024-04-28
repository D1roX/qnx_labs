CC = gcc
RM = rm -f
LIBRARY_PATH = ../utils/logger/lib
CFLAGS = -I$(LIBRARY_PATH) -std=c99 -w
LDFLAGS = -L$(LIBRARY_PATH) -llogger -Wl,-rpath,$(LIBRARY_PATH)

CLIENT = client
SERVER = server

SRC_CLIENT = $(CLIENT).c
SRC_SERVER = $(SERVER).c

all: $(CLIENT) $(SERVER)

$(CLIENT): $(SRC_CLIENT:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SERVER): $(SRC_SERVER:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.txt $(CLIENT) $(SERVER)
