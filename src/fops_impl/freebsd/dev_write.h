/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef DEV_ENIGMA_FOPS_IMPL_FREEBSD_DEV_WRITE_H
#define DEV_ENIGMA_FOPS_IMPL_FREEBSD_DEV_WRITE_H 1

#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/uio.h>

int dev_write(struct cdev *dev, struct uio *uio, int ioflag);

#endif
