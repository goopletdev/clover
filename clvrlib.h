#ifndef CLVR_HEADER_H
#define CLVR_HEADER_H
//for file handling:
#define MAX_LINE_LEN 256
//steno consts:

extern const unsigned int QWERTY_KEY_VALS[];

typedef struct uint_arr {
    unsigned int *arr;
    int size;
} uint_arr;

void print_bits(unsigned int arr);

char* pretty_chord(unsigned int chord);
char* paper_tape(unsigned int chord);

int steno_index_of(char value, int start);
int compare_chords(unsigned int chord1, unsigned int chord2);
unsigned int parse_chord(char* chord);

dict* parse_dictionary(const char* file_path);

void event_loop(struct config cf);

#endif // CLVR_HEADER_H