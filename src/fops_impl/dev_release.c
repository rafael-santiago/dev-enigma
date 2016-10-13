/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_release.h"
#include <dev_ctx.h>

int dev_release(struct inode *ip, struct file *fp) {
    int result = 0;
    mutex_lock(&dev_ctx()->lock);

    if (dev_ctx()->has_open) {
        dev_ctx()->has_open = 0;
    } else {
        result = -EBADF;
    }

    mutex_unlock(&dev_ctx()->lock);

    return result;
}
