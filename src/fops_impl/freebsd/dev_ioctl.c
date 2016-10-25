/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_ioctl.h"

int dev_ioctl(struct cdev *dev, u_long cmd, caddr_t data, int flag, struct thread *td) {
    return -ENOTTY;
}
