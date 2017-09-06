#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "beta/debug.h"
#include "beta/trace.h"
#include "beta/error.h"
#include "beta/stream.h"
#include "beta/string.h"
#include "beta/tree.h"
#include "beta/hash.h"
#include "beta/map.h"
#include "beta/stopword.h"

#define WORD_MAX 64
#define LINE_MAX 4096
#define WORD_SIZE 128

const char *STOPWORDS_PATH = "STOPWORDS_PATH";
const char *MORPH_EXCEPTIONS_PATH = "MORPH_EXCEPTIONS_PATH";
const char *WORDS_PATH = "WORDS_PATH";

struct map *morph_exceptions;
struct map *words_map;

struct food {
    int count;
    char *name;
};

struct word {
    int index;
    char *string;
};

void write_food(const void *object, int fd)
{
    const struct food *food;

    if (object) {
        food = (const struct food *)object;
        fprintf(stdout, "%s\t%d\n", food->name, food->count);
    }
}

int compare_food(const void *k1, const void *k2)
{
    return strcmp((char *)k1, (char *)k2);
}

int compare_word(const void *w1, const void *w2)
{
    return (*(struct word **)w1)->index - (*(struct word **)w2)->index;
}

int _search_word(char *word)
{
    if (map_contains(words_map, word))
        return 1;
    return 0;
}

char *word_morph(char *w)
{
    struct inflection {
        char *suffix;
        char *ending;
    };
    
    struct inflection f[] = {
        { "ches", "ch" },
        { "shes", "sh" },
        { "men", "man" },
        { "ies", "y" },
        { "ses", "s" },
        { "xes", "x" },
        { "zes", "z" },
        { "s", "" },
    };

    if (map_contains(morph_exceptions, w))
        return NULL;

    int ic = sizeof f / sizeof(struct inflection);
    for (int i = 0; i < ic; i++) {
        int sl = strlen(f[i].suffix);
        int el = strlen(f[i].ending);
        int wl = strlen(w);
        if (memcmp(w + (wl - sl), f[i].suffix, sl) == 0) {
            char *r = string_copy(w, wl - (sl - el) + 1);
            return r;
        }
    }

    return NULL;
}

int tokenize()
{
    int s, iseof, fdi;
    char l[LINE_MAX];
    char *p;
    char w[WORD_MAX];
    char *stopword_path;
    FILE *stopword_stream;
    struct stopword *stopword;
    int found;
    struct food *food;
    struct tree_node *node;
    struct tree *foods;
    char *token;
    int wi;
    char *word;

    char *morph_exceptions_path;
    int morph_exceptions_stream;

    char *words_path;
    int words_stream;

    if (!(stopword_path = getenv(STOPWORDS_PATH)))
        return error("env var STOPWORD_PATH not set");
    if (!(morph_exceptions_path = getenv(MORPH_EXCEPTIONS_PATH)))
        return error("env var MORPH_EXCEPTIONS_PATH not set");
    if (!(words_path = getenv(WORDS_PATH)))
        return error("env var WORDS_PATH not set");

    stopword_stream = fopen(stopword_path, "r");
    stopword = stopword_create(fileno(stopword_stream));

    morph_exceptions_stream = fileno(fopen(morph_exceptions_path, "r"));
    if (!(morph_exceptions = map_create(8, &hash_string, &strcmp)))
        return error("failed to allocate memory for a morph exceptions internal objects");
    for (; !stream_read_word(morph_exceptions_stream, w, WORD_SIZE, &iseof) && !iseof; ) {
        if (!(word = string_copy(w, WORD_SIZE)))
            return error("failed to copy a word");
        if (map_install(morph_exceptions, word, word))
            return error("failed to initialize a morph exc map");
    }

    words_stream = fileno(fopen(words_path, "r"));
    if (!(words_map = map_create(20001, &hash_string, &strcmp)))
        return error("failed to allocate memory for a word internal objects");
    for (; !stream_read_word(words_stream, w, WORD_SIZE, &iseof) && !iseof; ) {
        if (!(word = string_copy(w, WORD_SIZE)))
            return error("failed to copy a word");
        if (map_install(words_map, word, word))
            return error("failed to initialize a words map");
    }

    foods = tree_create(&compare_food, &write_food);

    fdi = fileno(stdin);

    for ( ; !stream_read_line(fdi, l, LINE_MAX, &iseof) && !iseof; ) {
        struct map *m = map_create(31, hash_string, strcmp);
        int key;
        int x;
        for (x = 0, p = l, wi = 0; string_word(l, &p, w) && p; x++) {
            if (x == 0) {
                key = atoi(w);
                continue;
            }
            if (strlen(w) > 1 && !stopword_contains(stopword, w)) {
                char *r = word_morph(w);
                char *lemma = r ? r : w;
                if (_search_word(lemma) == 1) {
                    token = lemma;
                    if (!map_contains(m, token)) {
                        struct word *word1 = malloc(sizeof(struct word));
                        word1->index = wi;
                        word1->string = string_copy(token, strlen(token)+1);
                        map_install(m, word1->string, word1);
                        wi++;
                    }
                }
            }
        }
        size_t wl;
        struct word **words = map_to_array(m, &wl);
        if (words) {
            qsort(words, wl, sizeof(void *), &compare_word);
            int i, j;
            for (i = j = 0; i < wl; i++) {
                if (j++ == 0)
                    fprintf(stdout, "%d\t%s", key, (*(words+i))->string);
                else
                    fprintf(stdout, " %s", (*(words+i))->string);
            }
            if (j) fprintf(stdout, "\n");
        }
        free(words);
        map_destroy(&m);
    }

    return 0;
}

/*
int analyze_frequency()
{
    int s, iseof, fdi;
    char w[WORD_MAX];
    char *stopword_path;
    FILE *stopword_stream;
    struct stopword *stopword;
    int found;
    struct food *food;
    struct tree_node *node;
    struct tree *foods;
    char *token;

    stopword_path = getenv(STOPWORD_PATH);
    stopword_stream = fopen(stopword_path, "r");
    stopword = stopword_create(fileno(stopword_stream));
    foods = tree_create(&compare_food, &write_food);
    fdi = fileno(stdin);
    for ( ; !stream_read_word(fdi, w, WORD_MAX, &iseof) && !iseof; )
        if (strlen(w) > 1 && !stopword_contains(stopword, w))
            if (!search_word(w, &found, &token) && found) {
                if (!(node = tree_search(foods, token))) {
                    if (!(food = (struct food *)malloc(sizeof(struct food))))
                        return error("failed to allocate memory for a food");
                    food->name = string_copy(token, WORD_MAX);
                    ++food->count;
                    if (!(node = tree_add(foods, food->name, food)))
                        return -1;
                } else {
                    if (!(food = (struct food *)tree_object(node)))
                        return -1;
                    ++food->count;
                }
            }
    tree_write(foods, fileno(stdout));

    return 0;
}
*/

int main(void)
{
    if (tokenize())
        exit(0);
    exit(1);
}

