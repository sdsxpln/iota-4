#include <stdio.h>
#include "beta/log.h"
#include "beta/error.h"
#include "beta/notused.h"
#include "beta/doc.h"
#include "beta/film-script.h"

int main(int argc, char **argv)
{
    notused(argc);
    notused(argv);
    int s;
    int fdi, fdo;
    struct doc *doc;
    match_term *term_matchers;

    fdi = fileno(stdin);
    fdo = fileno(stdout);
    log_init("scan");
    if ((s = doc_create(&doc)))
        return error("failed to create a doc");
    if ((s = doc_read(fdi, doc)))
        return error("failed to parse a doc");
    if ((s = film_script_init_term_matchers(&term_matchers)))
        return error("failed to get term matchers");
    if ((s = doc_parse(doc, term_matchers)))
        return error("failed to analyze a doc");
    if ((s = doc_analyze(fdo, doc)))
        return error("failed to analyze a doc");
    doc_destroy(&doc);
    log_deinit("scan");
    
    return 0;
}

