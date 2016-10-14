/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef DEV_ENIGMA_DEV_CTX_H
#define DEV_ENIGMA_DEV_CTX_H 1

#include <eel.h>
#include <ebuf.h>
#include <linux/mutex.h>

//  INFO(Santiago): DEV_USAGE_LINES_NR specifies the number of users
//                  which can be simultaneously hung in this device.

#define DEV_USAGE_LINES_NR 10

struct dev_enigma_usage_line_ctx {
    libeel_enigma_ctx *enigma;
    ebuf_ctx *ebuf_head, *ebuf_tail;
    struct mutex lock;
    int has_init;
};

struct dev_enigma_ctx {
    struct dev_enigma_usage_line_ctx ulines[DEV_USAGE_LINES_NR];
    int major_nr;
    struct class *device_class;
    struct device *device;
};

struct dev_enigma_ctx *dev_ctx(void);

libeel_enigma_ctx *dev_enigma(const int uline);

void lock_uline(const int uline);

void unlock_uline(const int uline);

int new_uline(void);

void release_uline(const int uline);

struct dev_enigma_usage_line_ctx *dev_uline_ctx(const int uline);

void init_ulines(void);

void deinit_ulines(void);

#endif
