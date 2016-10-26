/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_close.h"
#include "dev_open.h"
#include <dev_ctx.h>
#include <sys/conf.h>
#include <sys/malloc.h>

int dev_close(struct cdev *dev, int flags, int devtype, struct thread *td) {
    if (dev->si_drv1 == NULL) {
        return -EBADF;
    }

    release_uline(*(int *)dev->si_drv1);

    free(dev->si_drv1, M_DEV_OPEN);
    dev->si_drv1 = NULL;

    return 0;
}

