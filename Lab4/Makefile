CC = gcc
CFLAGS = -c -Wall -pedantic
STAND:=  -std=c99
TARGET = final

ifeq ($(OS), Windows_NT)

SOURCES = main.c thread_win.c stack.c
OBJECTS = $(SOURCES:.c=.o)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(STAND) -o $@

.c.o:
	$(CC) $(CFLAGS) $(STAND) $< -o $@

.PHONY: clean
clean:
	del $(TARGET).exe $(OBJECTS)

else

SOURCES = main.c thread_lin.c stack.c
OBJECTS = $(SOURCES:.c=.o)
UNIXFLAG = -D_XOPEN_SOURCE=600
THREADFLAG = -pthread

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(THREADFLAG) $(OBJECTS) $(STAND) -o $@

.c.o:
	$(CC) $(THREADFLAG) $(CFLAGS) $(UNIXFLAG) $(STAND) $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJECTS)

endif
