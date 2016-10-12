/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef DEV_ENIGMA_LIBC_H
#define DEV_ENIGMA_LIBC_H 1

#if defined(__linux__)
#include <linux/types.h>
#endif

char toupper(const char c);

char tolower(const char c);

void *malloc(size_t ssize);

void free(void *p);

#endif
