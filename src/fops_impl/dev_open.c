/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_open.h"
#include <dev_ctx.h>
#include <linux/mutex.h>

int dev_open(struct inode *ip, struct file *fp) {
    mutex_lock(&dev_ctx()->lock);

    if (dev_ctx()->has_open) {
        mutex_unlock(&dev_ctx()->lock);
        return -EBUSY;
    }

    dev_ctx()->has_open = 1;

    mutex_unlock(&dev_ctx()->lock);

    return 0;
}

