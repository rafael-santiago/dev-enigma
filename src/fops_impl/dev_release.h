/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef DEV_ENIGMA_FOPS_IMPL_DEV_RELEASE_H
#define DEV_ENIGMA_FOPS_IMPL_DEV_RELEASE_H 1

#include <linux/fs.h>

int dev_release(struct inode *ip, struct file *fp);

#endif
