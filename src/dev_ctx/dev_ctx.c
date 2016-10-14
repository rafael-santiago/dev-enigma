/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_ctx.h"
#include <eel.h>
#include <ebuf.h>
#include <linux/mutex.h>

static struct dev_enigma_ctx g_dev_ctx = { 0 };

struct dev_enigma_ctx *dev_ctx(void) {
    return &g_dev_ctx;
}

void lock_uline(const int uline) {
    if (!(uline >= 0 && uline <= DEV_USAGE_LINES_NR)) {
        return;
    }
    mutex_lock(&g_dev_ctx.ulines[uline].lock);
}

void unlock_uline(const int uline) {
    if (!(uline >= 0 && uline <= DEV_USAGE_LINES_NR)) {
        return;
    }
    mutex_unlock(&g_dev_ctx.ulines[uline].lock);
}

int new_uline(void) {
    int u;
    int uline = -1;

    for (u = 0; u < DEV_USAGE_LINES_NR && uline == -1; u++) {
        lock_uline(u);
        if (g_dev_ctx.ulines[u].enigma == NULL) {
            g_dev_ctx.ulines[u].enigma = libeel_new_enigma_ctx();
            if (g_dev_ctx.ulines[u].enigma != NULL) {
                uline = u;
            }
        }
        unlock_uline(u);
    }

    return uline;
}

void release_uline(const int uline) {
    if (!(uline >= 0 && uline <= DEV_USAGE_LINES_NR)) {
        return;
    }

    lock_uline(uline);

    if (g_dev_ctx.ulines[uline].enigma != NULL) {
        libeel_del_enigma_ctx(g_dev_ctx.ulines[uline].enigma);
        g_dev_ctx.ulines[uline].enigma = NULL;
    }

    if (g_dev_ctx.ulines[uline].ebuf_head != NULL) {
        del_ebuf_ctx(g_dev_ctx.ulines[uline].ebuf_head);
        g_dev_ctx.ulines[uline].ebuf_head = NULL;
    }

    unlock_uline(uline);
}

struct dev_enigma_usage_line_ctx *dev_uline_ctx(const int uline) {
    if (!(uline >= 0 && uline <= DEV_USAGE_LINES_NR)) {
        return NULL;
    }

    if (g_dev_ctx.ulines[uline].enigma == NULL) {
        return NULL;
    }

    return &g_dev_ctx.ulines[uline];
}

void init_ulines(void) {
    int u;
    for (u = 0; u < DEV_USAGE_LINES_NR; u++) {
        g_dev_ctx.ulines[u].enigma = NULL;
        g_dev_ctx.ulines[u].ebuf_head = NULL;
        mutex_init(&g_dev_ctx.ulines[u].lock);
        g_dev_ctx.ulines[u].has_init = 0;
    }
}

void deinit_ulines(void) {
    int u;
    for (u = 0; u < DEV_USAGE_LINES_NR; u++) {
        release_uline(u);
    }
}
