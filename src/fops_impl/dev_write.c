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
#include <ebuf.h>
#include <linux/uaccess.h>

ssize_t dev_write(struct file *fp, const char __user *buf, size_t count, loff_t *fpos) {
    struct dev_enigma_usage_line_ctx *ulp;
    int uline;
    const char *bp, *bp_end;
    ssize_t written_bytes = 0;

    uline = *(int *)fp->private_data;

    ulp = dev_uline_ctx(uline);

    if (ulp == NULL) {
        return -EBADF;
    }

    if (!ulp->has_init) {
        return -EINVAL;
    }

    if (!access_ok(VERIFY_READ, (void __user *)buf, count)) {
        return -EFAULT;
    }

    lock_uline(uline);

    bp = buf;
    bp_end = bp + count;

    while (bp != bp_end) {
        libeel_enigma_input(ulp->enigma) = *bp;
        written_bytes += add_char_to_ebuf_ctx(&ulp->ebuf_head, libeel_type(ulp->enigma), ulp->ebuf_tail);
        bp++;
    }

    *fpos += written_bytes;

    unlock_uline(uline);

    return written_bytes;
}
