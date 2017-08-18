#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include "beta/error.h"
#include "beta/notused.h"
#include "beta/stream.h"
#include "beta/list.h"

int main(int argc, char **argv)
{
    notused(argc);
    notused(argv);
    FILE *in = stdin;
    FILE *out = stdout;
    char w[256] = {0};
    int infd;
    int iseof = 0;
    
    infd = fileno(in);
    while (!stream_read_word(infd, w, strlen(w), &iseof) && !iseof)
        fprintf(out, "%s\n", w);

/*
    char word[WORD_SIZE] = { 0 };
    char line[LINE_SIZE] = { 0 };
    struct tree *tree;
    struct list *list;
    struct list_node *list_node;
    size_t line_length;

    tree_init(&tree, &compare);
    while ((status = get_word(input, word)) == 0 && status != EOF) {
        tree_add(tree, word, strlen(word) + 1);
    }
    tree_print(output, tree);
    tree_free(&tree);

    list_create(&list);
    while ((status = get_line(input, line, LINE_SIZE)) == 0 && status != EOF) {
        list_insert_node(list, line, strlen(line) + 1, &list_node);
    }
    for (list_node = list_head(list); list_node; list_node = list_next_node(list_node))
        fprintf(output, "%s", (char *)list_node_data(list_node, &line_length));
    list_destroy(&list);
*/
    return 0;
}

