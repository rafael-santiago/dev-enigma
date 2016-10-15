/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_open.h"
#include <dev_ctx.h>
#include <linux/slab.h>

int dev_open(struct inode *ip, struct file *fp) {
    int uline = new_uline();

    if (uline == -1) {
        return -EBUSY;
    }

    lock_uline(uline);

    fp->private_data = kmalloc(sizeof(int), GFP_KERNEL);

    if (fp->private_data != NULL) {
        *((int *)fp->private_data) = uline;
    }

    unlock_uline(uline);

    return (fp->private_data != NULL) ? 0 : -ENOMEM;
}

