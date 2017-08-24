#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include "beta/log.h"
#include "beta/error.h"
#include "beta/notused.h"
#include "beta/stream.h"
#include "beta/list.h"
#include "beta/doc.h"

int main(int argc, char **argv)
{
    notused(argc);
    notused(argv);
    int s = 0;
    int fdin = fileno(stdin);
    struct doc *doc = NULL;
    int l = 0;

    log_init("scan");

    if ((s = doc_create(&doc)))
        return error("failed to create a doc");
    if ((s = doc_read(fdin, doc)))
        return error("failed to parse a doc");

    l = doc_length(doc);
    fprintf(stdout, "doc length: [%d]\n", l);
    if ((s = doc_parse(doc)))
        return error("failed to analyze a doc");

    l = doc_terms(doc);
    fprintf(stdout, "doc terms: [%d]\n", l);

    doc_destroy(&doc);

    log_deinit("scan");
    
    return 0;
}

