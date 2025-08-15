clover: clover.c parse-to-hash.c hash-table.c qwerty-chord.c clover-lib.c
	gcc -o clover clover.c parse-to-hash.c hash-table.c qwerty-chord.c clover-lib.c -I.
parse-debug: parse-to-hash.c hash-table.c clover-lib.c
	gcc -g -o parse-debug parse-to-hash.c hash-table.c clover-lib.c -I.

qwerty-debug: qwerty-chord.c clover-lib.c
	gcc -g -o qwerty-debug qwerty-chord.c clover-lib.c -I.