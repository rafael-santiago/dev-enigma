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

int dev_close(struct cdev *dev, int flags __unused, int devtype __unused, struct thread *td __unused) {
    if (dev->si_drv1 == NULL) {
        return -EBADF;
    }

    return 0;
}

void dev_close_dtor(void *data) {
    if (data == NULL) {
        return;
    }

    release_uline(*(int *)data);

    free(data, M_DEV_OPEN);

    data = NULL;
}

