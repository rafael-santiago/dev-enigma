/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef DEV_ENIGMA_KEEL_H
#define DEV_ENIGMA_KEEL_H 1

#include <eel.h>

void libeel_clear_enigma_ctx(libeel_enigma_ctx *enigma);

libeel_enigma_ctx *libeel_new_enigma_ctx(void);

void libeel_del_enigma_ctx(libeel_enigma_ctx *p);

#endif
