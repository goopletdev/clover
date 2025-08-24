CC = gcc
CFLAGS = -Wall -Wextra -Wno-override-init -g -std=c99 -I.
COMMON_SOURCES = Unity/src/unity.c \
                 ../clover-chord/clover-chord.c

TESTS = chord-test hash-test json-test

all: $(TESTS)

chord-test: chord-test.c $(COMMON_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

hash-test: hash-test.c hash.c $(COMMON_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

json-test: json-test.c json.c hash.c $(COMMON_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

test: $(TESTS)
	./chord-test
	./hash-test
	./json-test

clean:
	rm -f $(TESTS)

.PHONY: all test clean
