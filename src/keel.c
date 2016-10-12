/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "keel.h"
#include <linux/slab.h>

//  INFO(Santiago): Well, here we are overwriting the user-space conveniences
//                  related with memory allocation in Libeel.

void libeel_clear_enigma_ctx(libeel_enigma_ctx *enigma) {
    if (enigma != NULL) {
        memset(enigma, 0, sizeof(libeel_enigma_ctx));
        libeel_ring(enigma, l) = 1;
        libeel_ring(enigma, m) = 1;
        libeel_ring(enigma, r) = 1;
    }
}

libeel_enigma_ctx *libeel_new_enigma_ctx() {
    libeel_enigma_ctx *p = NULL;
    p = (libeel_enigma_ctx *) kmalloc(sizeof(libeel_enigma_ctx), GFP_KERNEL);
    libeel_clear_enigma_ctx(p);
    return p;
}

void libeel_del_enigma_ctx(libeel_enigma_ctx *p) {
    libeel_clear_enigma_ctx(p);
    kfree(p);
}
