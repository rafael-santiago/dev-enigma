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

static void copy_default_enigma_setting(libeel_enigma_ctx *enigma);

struct dev_enigma_ctx *dev_ctx(void) {
    return &g_dev_ctx;
}

int lock_uline(const int uline) {
    if (!(uline >= 0 && uline <= DEV_USAGE_LINES_NR)) {
        return 0;
    }

    if (!mutex_trylock(&g_dev_ctx.ulines[uline].lock)) {
        return 0;
    }

    return 1;
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
                if (g_dev_ctx.default_setting != NULL) {
                    copy_default_enigma_setting(g_dev_ctx.ulines[u].enigma);
                    g_dev_ctx.ulines[u].has_init = libeel_init_machine(g_dev_ctx.ulines[u].enigma);
                }
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
        mutex_destroy(&g_dev_ctx.ulines[u].lock);
    }
}

static void copy_default_enigma_setting(libeel_enigma_ctx *enigma) {
    if (enigma == NULL) {
        return;
    }
    if (mutex_trylock(&g_dev_ctx.lock)) {
        memcpy(enigma, g_dev_ctx.default_setting, sizeof(libeel_enigma_ctx));
        mutex_unlock(&g_dev_ctx.lock);
    }
}

int set_default_enigma_setting(const libeel_enigma_ctx *enigma) {
    int result = 0;

    if (!mutex_trylock(&g_dev_ctx.lock)) {
        return 0;
    }

    if (g_dev_ctx.default_setting != NULL) {
        libeel_del_enigma_ctx(g_dev_ctx.default_setting);
    }

    g_dev_ctx.default_setting = libeel_new_enigma_ctx();

    if (g_dev_ctx.default_setting == NULL) {
        goto ___set_epilogue;
    }

    memcpy(g_dev_ctx.default_setting, enigma, sizeof(libeel_enigma_ctx));

    result = libeel_init_machine(g_dev_ctx.default_setting);

___set_epilogue:
    if (result == 0 && g_dev_ctx.default_setting != NULL) {
        libeel_del_enigma_ctx(g_dev_ctx.default_setting);
        g_dev_ctx.default_setting = NULL;
    }

    mutex_unlock(&g_dev_ctx.lock);

    return result;
}
