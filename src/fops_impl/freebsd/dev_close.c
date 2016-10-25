/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_close.h"

int dev_close(struct cdev *dev, int flags, int devtype, struct thread *td) {
    return 1;
}

