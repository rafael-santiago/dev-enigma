/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "ebuf.h"

#if defined(__linux__)

#include <linux/slab.h>

#define new_ebuf_ctx(e) ( (e) = (ebuf_ctx *) kmalloc(sizeof(ebuf_ctx), GFP_ATOMIC), (e)->c = 0, (e)->next = NULL )

#elif defined(__FreeBSD__)

#include <sys/param.h>
#include <sys/malloc.h>
#include <sys/kernel.h>

MALLOC_DEFINE(M_EBUF, "DEV_ENIGMA_ebuf", "Allocations related with ebuf");

#define new_ebuf_ctx(e) ( (e) = (ebuf_ctx *) malloc(sizeof(ebuf_ctx), M_EBUF, M_NOWAIT), (e)->c = 0, (e)->next = NULL )

#endif

void del_ebuf_ctx(ebuf_ctx *ebuf) {
    ebuf_ctx *t, *p;
    for (t = p = ebuf; t != NULL; p = t) {
        t = p->next;
        p->c = 0;
#if defined(__linux__)
        kfree(p);
#elif defined(__FreeBSD__)
        free(p, M_EBUF);
#endif
    }
}

int add_char_to_ebuf_ctx(ebuf_ctx **ebuf, const char c, ebuf_ctx *tail) {
    ebuf_ctx *ep;

    if ((*ebuf) == NULL) {
        new_ebuf_ctx((*ebuf));
        ep = (*ebuf);
    } else {
        if (tail != NULL) {
            new_ebuf_ctx(tail->next);
            ep = tail->next;
        } else {
            for (ep = (*ebuf); ep->next != NULL; ep = ep->next);
            new_ebuf_ctx(ep->next);
            ep = ep->next;
        }
    }

    if (ep == NULL) {
        return 0;
    }

    if (tail != NULL) {
        tail = ep;
    }

    ep->c = c;

    return 1;
}

char get_char_from_ebuf_ctx(ebuf_ctx **ebuf) {
    ebuf_ctx *ep;
    char c;

    if (ebuf == NULL) {
        return 0;
    }

    ep = *ebuf;

    if (ep == NULL) {
        return 0;
    }

    c = ep->c;

    *ebuf = ep->next;

    ep->next = NULL;

    del_ebuf_ctx(ep);

    return c;
}

