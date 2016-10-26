/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "mod_init.h"
#include <mod_info.h>
#include <dev_ctx.h>
#include <eel.h>
#include <dev_open.h>
#include <dev_close.h>
#include <dev_read.h>
#include <dev_write.h>
#include <dev_ioctl.h>
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/lock.h>

static struct cdevsw dev_enigma_cdevsw = {
    .d_version = D_VERSION,
    .d_open = dev_open,
    .d_close = dev_close,
    .d_read = dev_read,
    .d_write = dev_write,
    .d_ioctl = dev_ioctl,
    .d_name = DEVNAME
};

int enigma_init(void) {
    uprintf("dev/enigma: Initializing the /dev/enigma...\n");

    dev_ctx()->default_setting = NULL;
    mtx_init(&dev_ctx()->lock, "DEV_ENIGMA_device_lock", NULL, MTX_DEF);

    init_ulines();

    dev_ctx()->device = make_dev(&dev_enigma_cdevsw, 0, UID_ROOT, GID_WHEEL, 0600, DEVNAME);

    if (dev_ctx()->device == NULL) {
        uprintf("dev/enigma: \tdevice creation fail.\n");
        return 1;
    }

    uprintf("dev/enigma: Done.\n");

    return 0;
}
