/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "ebuf.h"
#include <linux/slab.h>

#define new_ebuf_ctx(e) ( (e) = (ebuf_ctx *) kmalloc(sizeof(ebuf_ctx), GFP_KERNEL), (e)->c = 0, (e)->next = NULL )

void del_ebuf_ctx(ebuf_ctx *ebuf) {
    ebuf_ctx *t, *p;
    for (t = p = ebuf; p; p = t) {
        t = p->next;
        p->c = 0;
        kfree(p);
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

