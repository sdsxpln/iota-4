/*
 * stream structures and routines
 * Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
 * This file is part of Beta.
 */

#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "beta/error.h"
#include "beta/stream.h"

#define errors(s, cnbr) ((cnbr == -1) && (s != 0) && (s != EINTR))
#define done(s, cnbr, nbr, nb) !((s == EINTR) && (cnbr < 0 ) && (nbr < nb))

static int stream_read_(int fd, void *b, size_t nb, ssize_t *nbr, int *iseof)
{
    ssize_t cnbr = 0;
    *nbr = 0;
    *iseof = 0;

    while ((cnbr = read(fd, b, nb)) != 0) {
        if (errors(errno, cnbr))
            return error("failed reading from a file descriptor %d", fd);

        b += cnbr;
        *nbr += cnbr;

        if (done(errno, cnbr, *nbr, (ssize_t)nb))
            break;
    }
    
    *iseof = cnbr == 0 ? 1 : 0;

    return 0;
}

int stream_read_word(int fd, char *w, size_t nc, int *iseof)
{
    char c;
    ssize_t nbr;

    while (!stream_read_(fd, &c, 1, &nbr, iseof) && !*iseof && !isalpha(c))
        ;
    if (*iseof)
        return 0;
    for (*w = c; !stream_read_(fd, &c, 1, &nbr, iseof) && !*iseof; *++w = c) {
        if (!isalpha(c)) {
            break;
        }
    }
    *++w = '\0';

    return 0;
}

/*
int stream_read_line(int f, char *l, size_t nc)
{
    if (!fgets(l, nc, f))
        return error("failed to read a line from a stream");

    return 0;
}
*/

int stream_read(int fd, void *b, size_t nb, ssize_t *nbr)
{
    int iseof = 0;
    return stream_read_(fd, b, nb, nbr, &iseof);
}

int stream_write(int fd, const void *b, size_t nb, ssize_t *nbw)
{
/*
    ssize_t cnbw;
    *nbw = 0;

    while ((cnbw = write(fd, b, (nb - (size_t)*nbw))) != 0) {
        if (errors(errno, *nbw))
            return error("failed writing to a file descriptor %d", fd);

        b = (const char *)b + (*nbw ? *nbw : 0);
        *nbw += cnbw;

        if (done(errno, cnbw))
            break;
    }
*/

    return 0;
}

#undef done
#undef errors

