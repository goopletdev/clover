#ifndef CLVR_HEADER_H
#define CLVR_HEADER_H
//for file handling:
#define MAX_LINE_LEN 256
//steno consts:
#define STENO_ORDER "#STKPWHRAO*EUFRPBLGTSDZ"
#define MIDDLE_START 8
#define RIGHT_START 13

extern const int STENO_ORDER_SIZE;
extern const unsigned int L_KEY_MASK;
extern const unsigned int M_KEY_MASK;
extern const unsigned int R_KEY_MASK;
extern const unsigned int CANCEL_MASK;
extern const unsigned int SEND_MASK;
extern const unsigned int STENO_MASK;
extern const unsigned int QWERTY_KEY_VALS[];

struct dictT {
    unsigned int id;
    char* translation;
    struct dictT* parent;
    struct dictmapT* children;
};

struct dictmapT {
    int size;
    int capacity;
    struct dictT** dicts;
};

typedef struct dictT dict;
typedef struct dictmapT dictmap;

typedef struct uint_arr {
    unsigned int *arr;
    int size;
} uint_arr;

struct config {
    dict* dictionary;
    char** listeners;
};

struct stroke {
    unsigned int chord;
    unsigned int was_deleted;
    char* translation;
};

int has(dictmap* m, unsigned int id);
int hash_index_of(dictmap* m, unsigned int id);
dictmap* scale_up_dictmap(dictmap* old);
dict* init_dict(unsigned int id, char* translation, dict* parent);
dictmap* init_dictmap(int capacity);
dict* get(dictmap* m, unsigned int id);
dictmap* add(dictmap* m, dict* element);
dictmap* scale_up_dictmap(dictmap* old);
dict* push_entry(dict* d, uint_arr id, int id_pos, char *translation);

void check_memory_allocation(void* ptr, int exit_code);
void print_bits(unsigned int arr);

char* pretty_chord(unsigned int chord);
char* paper_tape(unsigned int chord);

int steno_index_of(char value, int start);
int compare_chords(unsigned int chord1, unsigned int chord2);
unsigned int parse_chord(char* chord);

dict* parse_dictionary(const char* file_path);

void event_loop(struct config cf);

#endif // CLVR_HEADER_H