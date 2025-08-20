clover-chord-test: clover-chord-test.c Unity/src/unity.c ../clover-chord/clover-chord.c
	gcc -g -o clover-chord-test clover-chord-test.c Unity/src/unity.c ../clover-chord/clover-chord.c -I.

clover-hash-test: clover-hash-test.c Unity/src/unity.c ../clover-hash/clover-hash.c ../clover-chord/clover-chord.c
	gcc -o clover-hash-test clover-hash-test.c Unity/src/unity.c ../clover-hash/clover-hash.c ../clover-chord/clover-chord.c -I.

clover-json-test: clover-json-test.c Unity/src/unity.c ../clover-json/clover-json.c ../clover-hash/clover-hash.c ../clover-chord/clover-chord.c
	gcc -o clover-json-test clover-json-test.c Unity/src/unity.c ../clover-json/clover-json.c ../clover-hash/clover-hash.c ../clover-chord/clover-chord.c -I.