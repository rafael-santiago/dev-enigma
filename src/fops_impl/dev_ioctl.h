/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef DEV_ENIGMA_FOPS_IMPL_DEV_IOCTL_H
#define DEV_ENIGMA_FOPS_IMPL_DEV_IOCTL_H 1

#include <linux/fs.h>

long dev_ioctl(struct file *fp, unsigned int cmd, unsigned long usr_param);

#endif
