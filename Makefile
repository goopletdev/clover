parse-chord: parse-chord.c clover-lib.c
	gcc -o parse-chord parse-chord.c clover-lib.c -I.
