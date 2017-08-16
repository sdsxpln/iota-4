#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include "beta/error.h"
#include "beta/notused.h"
#include "beta/tree.h"
#include "beta/list.h"

#define LINE_SIZE 4096
#define WORD_SIZE 128

static int get_line(FILE *input, char *line, int size)
{
    if (!fgets(line, size, input))
        return error("failed to read a line from a stream");

    return 0;
}

static int get_word(FILE *input, char *word)
{
    char c;

    while ((c = fgetc(input)) != EOF && !isalpha(c))
        ;
    if (c == EOF)
        return EOF;
    for (*word = c; (c = fgetc(input)) && c != EOF; ) {
        if (!isalpha(c)) {
            ungetc(c, input);
            break;
        }
        *++word= c;
    }
    *++word = 0;

    return 0;
}

static int compare(const void *object1, const void *object2)
{
    return strcmp((const char *)object1, (const char *)object2);
}

int main(int argc, char **argv)
{
    notused(argc);
    notused(argv);
    int status = 0;
    FILE *input = stdin;
    FILE *output = stdout;
    char word[WORD_SIZE] = { 0 };
    char line[LINE_SIZE] = { 0 };
    struct tree *tree;
    struct list *list;
    struct list_node *list_node;
    size_t line_length;

/*
    tree_init(&tree, &compare);
    while ((status = get_word(input, word)) == 0 && status != EOF) {
        tree_add(tree, word, strlen(word) + 1);
    }
    tree_print(output, tree);
    tree_free(&tree);
*/
    list_create(&list);
    while ((status = get_line(input, line, LINE_SIZE)) == 0 && status != EOF) {
        list_insert_node(list, line, strlen(line) + 1, &list_node);
    }
    for (list_node = list_head(list); list_node; list_node = list_next_node(list_node))
        fprintf(output, "%s", (char *)list_node_data(list_node, &line_length));
    list_destroy(&list);

    return 0;
}

