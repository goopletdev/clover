CC = gcc
CFLAGS = -Wall -Wextra -Wno-override-init -g -std=c99 -I.
TARGET = clover
SOURCES = clover.c \
          clover-chord/clover-chord.c \
          clover-event-listener/clover-event-listener.c \
          clover-hash/clover-hash.c \
          clover-json/clover-json.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

