CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = db

all: $(TARGET)

$(TARGET): main.o btree.o repl.o
    $(CC) $(CFLAGS) -o $(TARGET) main.o btree.o repl.o

main.o: main.c
    $(CC) $(CFLAGS) -c main.c

btree.o: btree.c
    $(CC) $(CFLAGS) -c btree.c

repl.o: repl.c
    $(CC) $(CFLAGS) -c repl.c

clean:
    rm -f *.o $(TARGET)