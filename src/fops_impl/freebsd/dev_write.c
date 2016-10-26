/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_write.h"
#include <dev_ctx.h>
#include <eel.h>
#include <sys/malloc.h>
#include <sys/conf.h>

MALLOC_DEFINE(M_DEV_WRITE, "DEV_ENIGMA_dev_write", "Allocations related with dev_write");

int dev_write(struct cdev *dev, struct uio *uio, int ioflags) {
    struct dev_enigma_usage_line_ctx *ulp;
    int uline;
    char *temp_buf = NULL;
    char *bp, *bp_end;
    ssize_t written_bytes = 0;
    size_t temp_buf_size = 0;

    uline = *(int *)dev->si_drv1;

    ulp = dev_uline_ctx(uline);

    if (ulp == NULL) {
        return -EBADF;
    }

    if (!ulp->has_init) {
        return -EINVAL;
    }

    temp_buf_size = uio->uio_iov->iov_len;
    temp_buf = (char *) malloc(temp_buf_size, M_DEV_WRITE, M_NOWAIT);

    if (temp_buf == NULL) {
        return -ENOMEM;
    }

    if (!lock_uline(uline)) {
        return -EBUSY;
    }

    if (uiomove(temp_buf, temp_buf_size, uio) != 0) {
        free(temp_buf, M_DEV_WRITE);
        unlock_uline(uline);
        return -EFAULT;
    }

    bp = temp_buf;
    bp_end = bp + temp_buf_size;

    while (bp != bp_end) {
        libeel_enigma_input(ulp->enigma) = *bp;
        written_bytes += add_char_to_ebuf_ctx(&ulp->ebuf_head, libeel_type(ulp->enigma), ulp->ebuf_tail);
        bp++;
    }

    free(temp_buf, M_DEV_WRITE);

    unlock_uline(uline);

    return (written_bytes == temp_buf_size) ? 0 : EFAULT;
}

