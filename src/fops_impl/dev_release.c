/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_release.h"
#include <linux/slab.h>
#include <dev_ctx.h>

int dev_release(struct inode *ip, struct file *fp) {
    if (fp->private_data == NULL) {
        return -EBADF;
    }

    release_uline(*(int *)fp->private_data);

    kfree(fp->private_data);

    return 0;
}
