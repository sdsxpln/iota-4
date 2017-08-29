#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "wn.h"
#include "beta/error.h"
#include "beta/stream.h"

#define WORD_MAX 64

int search_word(char *word)
{
    int i;
    char *lemma;
    Synset *result, *synset, *sense;

    if (!(lemma = morphstr(word, NOUN)))
        lemma = word;
    if (!(result = findtheinfo_ds(lemma, NOUN, -HYPERPTR, ALLSENSES)))
        return error("failed searching for lemma [%s] of a word [%s]\n", lemma, word);
    for (sense = result; sense; sense = sense->nextss)
        for (synset = sense->ptrlist; synset; synset = synset->ptrlist)
            for (i = 0; i != synset->wcount; i++)
                if (strcmp(*(synset->words + i), "food") == 0) {
                    fprintf(stdout, "food: [%s], def: [%s]\n", lemma, synset->defn);
                    goto found;
                }
    found:
    if (result)
        free_synset(result);

    return 0;
}

int main(void)
{
    int s, iseof, fdi;
    char w[WORD_MAX];

    fdi = fileno(stdin);
    if ((s = wninit())) {
        fprintf(stderr, "failed to init wn db [%d]\n", errno);
        exit(EXIT_FAILURE);
    }
    if ((s = morphinit())) {
        fprintf(stderr, "failed to init wn morph [%d]\n", errno);
        exit(EXIT_FAILURE);
    }
    for ( ; !stream_read_word(fdi, w, WORD_MAX, &iseof) && !iseof; )
        search_word(w);

    return EXIT_SUCCESS;
}

