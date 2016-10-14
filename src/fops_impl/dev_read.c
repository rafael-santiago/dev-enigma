/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_read.h"
#include <dev_ctx.h>
#include <ebuf.h>
#include <asm/uaccess.h>

ssize_t dev_read(struct file *fp, char __user *buf, size_t count, loff_t *f_pos) {
    int uline;
    struct dev_enigma_usage_line_ctx *ulp;
    char byte;
    ssize_t read_bytes = 0;

    uline = *(int *)fp->private_data;

    ulp = dev_uline_ctx(uline);

    if (ulp != NULL) {
        return -EBADF;
    }

    lock_uline(uline);

    while (read_bytes != count && ulp->ebuf_head != NULL) {
        byte = get_char_from_ebuf_ctx(&ulp->ebuf_head);
        if (copy_to_user(buf + read_bytes, &byte, 1) != 0) {
            read_bytes = -EFAULT;
            goto __dev_read_epilogue;
        }
        read_bytes++;
    }

    *f_pos += read_bytes;

__dev_read_epilogue:
    unlock_uline(uline);

    return read_bytes;
}
