#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "wn.h"
#include "beta/error.h"
#include "beta/stream.h"
#include "beta/string.h"
#include "beta/tree.h"
#include "beta/hash.h"
#include "beta/map.h"
#include "beta/stopword.h"

#define WORD_MAX 64
const char *STOPWORD_PATH = "STOPWORD_PATH";

struct food {
    int count;
    char *name;
};

int search_word(char *word, int *found, char **token)
{
    int i;
    char *lemma;
    Synset *result, *synset, *sense;

    *found = 0;
    if (!(lemma = morphstr(word, NOUN)))
        lemma = word;
    if (!(result = findtheinfo_ds(lemma, NOUN, -HYPERPTR, ALLSENSES)))
        return error("failed searching for lemma [%s] of a word [%s]\n", lemma, word);
    for (sense = result; sense; sense = sense->nextss)
        for (synset = sense->ptrlist; synset; synset = synset->ptrlist)
            for (i = 0; i != synset->wcount; i++)
                if (strcmp(*(synset->words + i), "food") == 0) {
                    fprintf(stderr, "food: [%s], def: [%s]\n", lemma, synset->defn);
                    *found = 1;
                    *token = lemma;
                    goto finally;
                }
    finally:
    if (result)
        free_synset(result);

    return 0;
}

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

int main(void)
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
    if ((s = wninit())) {
        fprintf(stderr, "failed to init wn db [%d]\n", s);
        exit(EXIT_FAILURE);
    }
    if ((s = morphinit())) {
        fprintf(stderr, "failed to init wn morph [%d]\n", s);
        exit(EXIT_FAILURE);
    }
    for ( ; !stream_read_word(fdi, w, WORD_MAX, &iseof) && !iseof; )
        if (strlen(w) > 1 && !stopword_contains(stopword, w))
            if (!search_word(w, &found, &token) && found) {
                if (!(node = tree_search(foods, token))) {
                    if (!(food = (struct food *)malloc(sizeof(struct food)))) {
                        error("failed to allocate memory for a food");
                        exit(EXIT_FAILURE);
                    }
                    food->name = string_copy(token, WORD_MAX);
                    ++food->count;
                    if (!(node = tree_add(foods, food->name, food)))
                        exit(EXIT_FAILURE);
                } else {
                    if (!(food = (struct food *)tree_object(node)))
                        exit(EXIT_FAILURE);
                    ++food->count;
                }
            }

    tree_write(foods, fileno(stdout));

    return EXIT_SUCCESS;
}

