#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <regex.h>

#define MAX_LINE_SIZE 200
#define MAX_SCRIPT_LINE_COUNT 40000

struct treenode {
    char *word;
    int count;
    struct treenode *left;
    struct treenode *right;
};

struct scriptline {
    char text[MXLNSZ];
};

struct script {
};

struct tnode *addtree(struct tnode *p, char *w)
{
    int cond;

    if (p == NULL) {
        p = (struct tnode *)malloc(sizeof(struct tnode));
        p->word = (char *)malloc(strlen(w)+1);
        if (p->word != NULL)
            strcpy(p->word, w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++;
    } else if (cond < 0) {
        p->left = addtree(p->left, w);
    } else {
        p->right = addtree(p->right, w);
    }

    return p;
}

void treeprint(
    FILE *stream,
    struct tnode *p
)
{
    if (p != NULL) {
        treeprint(stream, p->left);
        fprintf(stream, "%4d %s\n", p->count, p->word);
        treeprint(stream, p->right);
    }
}

int main(int argc, char **argv)
{
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

    size_t rerr = 0;
    size_t rerrbfsz = 0x80;
    char rerrbf[rerrbfsz];

    struct tnode *root = NULL;

    struct scriptline script[MXNLN];

    if ((e = regcomp(&rext, "[[:digit:]]*[[:blank:]]+EXT[.]", REG_EXTENDED)) != 0) {
        rerr = regerror(e, &rext, rerrbf, rerrbfsz);
        fprintf(serr, "ERROR: %s %lu, line: [%d]\n", rerrbf, rerr, __LINE__);
        exit(1);
    }

    if ((e = regcomp(&rint, "[[:digit:]]*[[:blank:]]+INT[.]", REG_EXTENDED)) != 0) {
        rerr = regerror(e, &rext, rerrbf, rerrbfsz);
        fprintf(serr, "ERROR: %s %lu, line: [%d]\n", rerrbf, rerr, __LINE__);
        exit(1);
    }

    if ((e = regcomp(&rchr, "^[[:blank:]]{16,}([A-Z]+)[[:blank:]]*$", REG_EXTENDED|REG_NEWLINE)) != 0) {
        rerr = regerror(e, &rext, rerrbf, rerrbfsz);
        fprintf(serr, "ERROR: %s %lu, line: [%d]\n", rerrbf, rerr, __LINE__);
        exit(1);
    }

    fprintf(
        sout, "%s\t%s\t%s\t%s\n",
        "ln", "ext", "int", "chr"
    );

    while ((fgets(lnbf, lnbfsz, sin) != NULL)) {

        if ((e = regexec(&rext, lnbf, 0, NULL, REG_NOTBOL|REG_NOTEOL)) == 0)  {
            scnext++;
        } else {
            if (e != REG_NOMATCH) {
                rerr = regerror(e, &rext, rerrbf, rerrbfsz);
                fprintf(serr, "ERROR: [%d] [%s] [%lu], line: [%d]\n", e, rerrbf, rerr, __LINE__);
                exit(1);
            }
        }

        if ((e = regexec(&rint, lnbf, 0, NULL, REG_NOTBOL|REG_NOTEOL)) == 0) {
            scnint++;
        } else {
            if (e != REG_NOMATCH) {
                rerr = regerror(e, &rext, rerrbf, rerrbfsz);
                fprintf(serr, "ERROR: [%d] [%s] [%lu], line: [%d]\n", e, rerrbf, rerr, __LINE__);
                exit(1);
            }
        }

        size_t rnm = 2;
        regmatch_t rm[rnm];
        if ((e = regexec(&rchr, lnbf, rnm, rm, 0)) == 0) {
            scnchr++;
            assert(rm != NULL);
            root = addtree(root, &lnbf[rm[1].rm_so]);
        } else {
            if (e != REG_NOMATCH) {
                rerr = regerror(e, &rext, rerrbf, rerrbfsz);
                fprintf(
                    serr, "ERROR:: errno: [%d], errstr: [%s], \
                        lnbf: [%s], \
                        regerr: [%lu], line: [%d]\n",
                    e, rerrbf, lnbf, rerr, __LINE__
                );
                exit(1);
            }
        }

//        fprintf(serr, "%d %s\n", scln, lnbf);
        memcpy(script[scln].text, lnbf, MXLNSZ-1);
        if (errno != 0) {
            fprintf(serr, "ERROR %d %s\n", errno, strerror(errno));
            exit(1);
        }
        scln++;

    }

    fprintf(
        sout, "%d\t%d\t%d\t%d\n",
        scln, scnext, scnint, scnchr
    );

    treeprint(sout, root);

    regfree(&rext);
    regfree(&rint);
    regfree(&rchr);
    if (lnbf) free(lnbf);
    if (scbf) free(scbf);

    return 0;
}

