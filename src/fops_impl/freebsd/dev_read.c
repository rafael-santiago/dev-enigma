/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_read.h"
#include <dev_ctx.h>
#include <sys/conf.h>

int dev_read(struct cdev *dev, struct uio *uio, int ioflags) {
    int uline;
    struct dev_enigma_usage_line_ctx *ulp;
    char byte;
    int read_bytes = 0;

    uline = *(int *)dev->si_drv1;

    ulp = dev_uline_ctx(uline);

    if (ulp == NULL) {
        return -EBADF;
    }

    if (!lock_uline(uline)) {
        return -EBUSY;
    }

    while (read_bytes != uio->uio_iov->iov_len && ulp->ebuf_head != NULL) {
        byte = get_char_from_ebuf_ctx(&ulp->ebuf_head);
        if (uiomove(&byte, 1, uio) != 0) {
            read_bytes = -EFAULT;
            goto __dev_read_epilogue;
        }
        read_bytes++;
    }

__dev_read_epilogue:
    unlock_uline(uline);

    return read_bytes;
}
