/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef DEV_ENIGMA_FOPS_IMPL_DEV_READ_H
#define DEV_ENIGMA_FOPS_IMPL_DEV_READ_H 1

#include <linux/fs.h>

ssize_t dev_read(struct file *fp, char __user *buf, size_t count, loff_t *f_pos);

#endif
