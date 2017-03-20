CC = gcc
CFLAGS = -c -Wall -pedantic
STAND:=  -std=c99
SERVER = server
CLIENT = client

ifeq ($(OS), Windows_NT)

SOURCES_FOR_SERVER = server.c server_win.c
OBJECTS_FOR_SERVER = $(SOURCES_FOR_SERVER:.c=.o)
SOURCES_FOR_CLIENT = client.c client_win.c
OBJECTS_FOR_CLIENT = $(SOURCES_FOR_CLIENT:.c=.o)

all: $(SERVER) $(CLIENT)

$(SERVER): $(OBJECTS_FOR_SERVER)
	$(CC) $(OBJECTS_FOR_SERVER) $(STAND) -o $@

$(CLIENT): $(OBJECTS_FOR_CLIENT)
	$(CC) $(OBJECTS_FOR_CLIENT) $(STAND) -o $@

.c.o:
	$(CC) $(CFLAGS) $(STAND) $< -o $@

.PHONY: clean
clean:
	del $(OBJECTS_FOR_SERVER) $(OBJECTS_FOR_CLIENT)

else

SOURCES_FOR_SERVER = server.c server_lin.c 
OBJECTS_FOR_SERVER = $(SOURCES_FOR_SERVER:.c=.o)
SOURCES_FOR_CLIENT = client.c client_lin.c
OBJECTS_FOR_CLIENT = $(SOURCES_FOR_CLIENT:.c=.o)

UNIXFLAGS = -D_XOPEN_SOURCE=600

all: $(SERVER) $(CLIENT)

$(SERVER): $(OBJECTS_FOR_SERVER)
	$(CC) $(OBJECTS_FOR_SERVER) $(STAND) -o $@


$(CLIENT): $(OBJECTS_FOR_CLIENT)
	$(CC) $(OBJECTS_FOR_CLIENT) $(STAND) -o $@


.c.o:
	$(CC) $(CFLAGS) $(UNIXFLAGS) $(STAND) $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJECTS_FOR_SERVER) $(OBJECTS_FOR_CLIENT) $(SERVER) $(CLIENT)
	
endif
