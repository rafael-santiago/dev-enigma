/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_open.h"
#include "dev_close.h"
#include <dev_ctx.h>
#include <sys/conf.h>

MALLOC_DEFINE(M_DEV_OPEN, "DEV_ENIGMA_dev_open", "Allocations related with dev_open");

int dev_open(struct cdev *dev, int flags __unused, int devtype __unused, struct thread *td __unused) {
    int uline = new_uline();

    if (uline == -1) {
        return -EBUSY;
    }

    if (!lock_uline(uline)) {
        return -EBUSY;
    }

    dev->si_drv1 = malloc(sizeof(int), M_DEV_OPEN, M_NOWAIT);

    if (dev->si_drv1 != NULL) {
        *((int *)dev->si_drv1) = uline;
        devfs_set_cdevpriv(dev->si_drv1, dev_close_dtor);
    }

    unlock_uline(uline);

    return (dev->si_drv1 != NULL) ? 0 : -ENOMEM;
}
