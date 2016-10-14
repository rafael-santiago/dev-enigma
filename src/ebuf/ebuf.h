/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef DEV_ENIGMA_EBUF_H
#define DEV_ENIGMA_EBUF_H 1

typedef struct _ebuf_ctx {
    char c;
    struct _ebuf_ctx *next;
}ebuf_ctx;

int add_char_to_ebuf_ctx(ebuf_ctx **ebuf, const char c, ebuf_ctx *tail);

char get_char_from_ebuf_ctx(ebuf_ctx **ebuf);

void del_ebuf_ctx(ebuf_ctx *ebuf);

#endif
