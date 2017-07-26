#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static int MXLNSZ = 0x80;
static int MXNLN = 0x8e3;

struct tnode {
    char *word;
    int count;
    struct tnode *left;
    struct tnode *right;
};

struct tnode *addtree(struct tnode *p, char *w) {

    int cond;

    if (p == NULL) {
        p = (struct tnode *)malloc(sizeof(struct tnode));
        p->word = (char *)malloc(strlen(w)+1);
        if (p->word != NULL)
            strcpy(p->word, w);
        p->count = 1;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++;
    } else if (cond < 0) {
        p->left = addtree(p->left, w);
    } else {
        p->right = addtree(p->right, w);
    }

    return p;
}

int main(int argc, char **argv) {

    int e = 0;
    FILE *sin = stdin;
    FILE *sout = stdout;
    FILE *serr = stderr;

    int lnbfsz = MXLNSZ;
    char *lnbf = (char *)malloc(sizeof(char)*lnbfsz);

    int scbfsz = MXLNSZ*MXNLN;
    char *scbf = (char *)malloc(sizeof(char)*scbfsz);

    int scln = 1;
    int scnext = 0;
    int scnint = 0;
    int scnchr = 0;

    regex_t rext;
    regex_t rint;
    regex_t rchr;

    regmatch_t rm;
    size_t rnm = 0;
    size_t rerr = 0;
    size_t rerrbfsz = 0x80;
    char rerrbf[rerrbfsz];

    struct tnode *root = NULL;

    if ((e = regcomp(&rext, "[[:digit:]]*[[:blank:]]+EXT[.]", REG_EXTENDED | REG_NEWLINE)) != 0) {
        rerr = regerror(e, &rext, rerrbf, rerrbfsz);
        fprintf(serr, "ERROR: %s %lu, line: [%d]\n", rerrbf, rerr, __LINE__);
        exit(1);
    }

    if ((e = regcomp(&rint, "[[:digit:]]*[[:blank:]]+INT[.]", REG_EXTENDED | REG_NEWLINE)) != 0) {
        rerr = regerror(e, &rext, rerrbf, rerrbfsz);
        fprintf(serr, "ERROR: %s %lu, line: [%d]\n", rerrbf, rerr, __LINE__);
        exit(1);
    }

    if ((e = regcomp(&rchr, "^[[:blank:]]+([A-Z]+)$", REG_EXTENDED | REG_NEWLINE)) != 0) {
        rerr = regerror(e, &rext, rerrbf, rerrbfsz);
        fprintf(serr, "ERROR: %s %lu, line: [%d]\n", rerrbf, rerr, __LINE__);
        exit(1);
    }

    fprintf(
        sout, "%s\t%s\t%s\t%s\n",
        "ln", "ext", "int", "chr"
    );

    while ((fgets(lnbf, lnbfsz, sin) != NULL)) {

        ;
        if ((e = regexec(&rext, lnbf, rnm, &rm, REG_NOTEOL) == 0))  {
            scnext++;
        } else {
            if (e != REG_NOMATCH) {
                rerr = regerror(e, &rext, rerrbf, rerrbfsz);
                fprintf(serr, "ERROR: [%d] [%s] [%lu], line: [%d]\n", e, rerrbf, rerr, __LINE__);
                exit(1);
            }
        }

        if ((e = regexec(&rint, lnbf, rnm, &rm, REG_NOTEOL) == 0)) {
            scnint++;
        } else {
            rerr = regerror(e, &rext, rerrbf, rerrbfsz);
            fprintf(serr, "ERROR: %s %lu, line: [%d]\n", rerrbf, rerr, __LINE__);
            exit(1);
        }

        if ((e = regexec(&rchr, lnbf, rnm, &rm, REG_NOTEOL)) == 0) {
            scnchr++;
            root = addtree(root, &lnbf[rm.rm_so]);
        } else {
            rerr = regerror(e, &rext, rerrbf, rerrbfsz);
            fprintf(serr, "ERROR: %s %lu, line: [%d]\n", rerrbf, rerr, __LINE__);
            exit(1);
        }

        scln++;
//        if ((scln % scrlnprt == 0)) {
//            fprintf(
//                sout, "%d\t%d\t%d\t%d\n",
//                scrprti, scrnext, scrnint, scrnchr
//            );
//            scrnext = 0;
//            scrnint = 0;
//            scrnchr = 0;
//            scrprti++;
//        }

    }

    fprintf(
        sout, "%d\t%d\t%d\t%d\n",
        scln, scnext, scnint, scnchr
    );

    regfree(&rext);
    regfree(&rint);
    regfree(&rchr);
    if (lnbf) free(lnbf);
    if (scbf) free(scbf);

    return 0;
}

