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

#define WORD_MAX_LENGTH 256
#define LINE_MAX_LENGTH 4096

int main(int argc, char **argv)
{
    notused(argc);
    notused(argv);
    int fdin = fileno(stdin);
    char l[LINE_MAX_LENGTH] = {0};
    int iseof = 0;
    
    while (!stream_read_line(fdin, l, LINE_MAX_LENGTH, &iseof) && !iseof)
        fprintf(stdout, "%s\n", l);

    return 0;
}

