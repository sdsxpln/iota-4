/*

hash routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black> 
This file is part of beta.

*/

#include <math.h>
#include "beta/hash.h"

unsigned hash_string(char *s, int n)
{
    unsigned h;

    for (h = 0; *s != '\0'; s++)
        h = *s + 31 * h;

    return h % n;
}

/* 

(sqrt(5)-1)/2

[0] Donald E. Knuth. Sorting and Searching, volume 3 of The Art of Computer
Programming. Addison-Wesley, 1998. 2nd ed.

[1] Cormen, Thomas H.; Leiserson, Charles E.; Rivest, Ronald L.; Stein,
Clifford. Introduction to Algorithms. The MIT Press, 2009. 3rd ed.

*/

#define A 0.6180339887498949

unsigned hash_integer(int *i, int n)
{
    unsigned h;
    unsigned long w, p, s, r, ra, rb;

    w = sizeof(int) * 8;
    p = (unsigned long)(ceil(log2(n)));
    s = (unsigned long)(A * pow(2, w));
    r = (unsigned)*i * s;
    ra = r >> w;
    rb = r ^ (ra << w);
    h = (unsigned)(rb >> (w-p));

    return h;
}

#undef A

