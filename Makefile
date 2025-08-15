parse-chord: parse-chord.c clover-lib.c
	gcc -o parse-chord parse-chord.c clover-lib.c -I.

parse-to-hash: parse-to-hash.c hash-table.c clover-lib.c
	gcc -o parse-to-hash parse-to-hash.c hash-table.c clover-lib.c -I.

parse-debug: parse-to-hash.c hash-table.c clover-lib.c
	gcc -g -o parse-debug parse-to-hash.c hash-table.c clover-lib.c -I.