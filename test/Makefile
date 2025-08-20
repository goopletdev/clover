CC = gcc
CFLAGS = -Wall -Wextra -Wno-override-init -g -std=c99 -I.
COMMON_SOURCES = Unity/src/unity.c \
                 ../clover-chord/clover-chord.c

TESTS = clover-chord-test clover-hash-test clover-json-test

all: $(TESTS)

clover-chord-test: clover-chord-test.c $(COMMON_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clover-hash-test: clover-hash-test.c ../clover-hash/clover-hash.c $(COMMON_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clover-json-test: clover-json-test.c ../clover-json/clover-json.c ../clover-hash/clover-hash.c $(COMMON_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

test: $(TESTS)
	./clover-chord-test
	./clover-hash-test
	./clover-json-test

clean:
	rm -f $(TESTS)

.PHONY: all test clean
