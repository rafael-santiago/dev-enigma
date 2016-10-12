/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "libc.h"
#if defined(__linux__)
#include <linux/types.h>
#endif

//  INFO(Santiago): This module is used to implement the libc stuff used by Libeel.
//
//                  This "dirty trick" makes the library "thinks" that the things are
//                  still going on under the user-space. Doing it we can use a library
//                  which was written for user-space into kernel-space without changing
//                  anything on Eel, neither exploding things into kernel-space.
//
//                  WARNING: Do not try to implement stupid stubs for malloc(),
//                           free() and use the Libeel's user-space allocation
//                           conveniences. You will got a Boom-Boom Johnny...

static char *g_letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

const unsigned short **__ctype_b_loc(void);

char toupper(const char c) {
    if (!(c >= 'a' && c <= 'z')) {
        return c;
    }

    return g_letter[c - 'a'];
}

char tolower(const char c) {
    if (!(c >= 'A' && c <= 'Z')) {
        return c;
    }

    return g_letter[c + 26];
}

const unsigned short **__ctype_b_loc(void) {
    static const unsigned short *boo = NULL;
    return &boo;
}
