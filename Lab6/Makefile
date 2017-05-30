CC = gcc
CFLAGS = -c -Wall -pedantic
STAND:=  -std=c99
TARGET = final

SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)


all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(STAND) -o $@

.c.o:
	$(CC) $(CFLAGS) $(STAND) $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJECTS)
