/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "mod_exit.h"
#include "mod_info.h"
#include <dev_ctx.h>
#include <eel.h>
#include <sys/systm.h>
#include <sys/param.h>
#include <sys/conf.h>

void enigma_exit(void) {
    uprintf("dev/enigma: The /dev/enigma is being unloaded...\n");

    deinit_ulines();

    unset_default_enigma_setting();

    mtx_destroy(&dev_ctx()->lock);

    destroy_dev(dev_ctx()->device);

    uprintf("dev/enigma: Done.\n");
}
