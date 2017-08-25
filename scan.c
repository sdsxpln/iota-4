#include <stdio.h>
#include "beta/log.h"
#include "beta/error.h"
#include "beta/notused.h"
#include "beta/doc.h"

int main(int argc, char **argv)
{
    notused(argc);
    notused(argv);
    int s = 0;
    int fdi = fileno(stdin);
    int fdo = fileno(stdout);
    struct doc *doc = NULL;
    match_term *term_matcher = NULL;

    log_init("scan");

    if ((s = doc_create(&doc)))
        return error("failed to create a doc");

    if ((s = doc_read(fdi, doc)))
        return error("failed to parse a doc");

    if ((s = film_script_create_term_matchers(&term_matchers)))
        return error("failed to create term matchers");

    if ((s = doc_parse(doc)))
        return error("failed to analyze a doc");

    if ((s = film_script_destroy_term_matchers(&term_matchers)))
        return error("failed to create term matchers");

    if ((s = doc_analyze(fdo, doc)))
        return error("failed to analyze a doc");

    doc_destroy(&doc);

    log_deinit("scan");
    
    return 0;
}

