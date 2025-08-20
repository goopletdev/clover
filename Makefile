CC = gcc
CFLAGS = -Wall -Wextra -Wno-override-init -g -std=c99 -I.
TARGET = clover
PKGS = libevdev
SOURCES = clover.c \
          clover-chord/clover-chord.c \
          clover-event-listener/clover-event-listener.c \
          clover-event-emulator/clover-event-emulator.c \
          clover-hash/clover-hash.c \
          clover-json/clover-json.c

CFLAGS += $(shell pkg-config --cflags $(PKGS))
LDFLAGS += $(shell pkg-config --libs $(PKGS))

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean