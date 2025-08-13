#ifndef CLVR_HEADER_H
#define CLVR_HEADER_H

#define STENO_ORDER "#STKPWHRAO*EUFRPBLGTSDZ"
#define MIDDLE_START 8
#define RIGHT_START 13

extern const int STENO_ORDER_SIZE;
extern const unsigned int M_KEY_MASK;
extern const unsigned int R_KEY_MASK;
extern const unsigned int CANCEL_MASK;
extern const unsigned int SEND_MASK;
extern const unsigned int QWERTY_KEY_VALS[];

struct dictT {
    unsigned int id;
    char* translation;
    struct dictionaryTT* parent;
    struct dict_listT children;
};

struct dictmapT {
    int size;
    int capacity;
    struct dictT** dicts;
};

typedef struct dictT dict;
typedef struct dictmapT dictmap;

typedef struct u_int_array {
    unsigned int *arr;
    int size;
} u_int_array;

typedef struct json_entry {
    u_int_array key;
    char* value;
} json_entry;

void check_memory_allocation(void* ptr, int exit_code);
void print_bits(unsigned int arr);

char* pretty_chord(unsigned int chord);
char* paper_tape(unsigned int chord);

int steno_index_of(char value, int start);
int compare_chords(unsigned int chord1, unsigned int chord2);
u_int_array parse_pretty_chord(char *chord, int *start, char terminator);

#endif // CLVR_HEADER_H

