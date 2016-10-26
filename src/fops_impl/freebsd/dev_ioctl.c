/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_ioctl.h"
#include <dev_ctx.h>
#include <eel.h>
#include <enigmactl.h>
#include <sys/conf.h>

int dev_ioctl(struct cdev *dev, u_long cmd, caddr_t data, int flag, struct thread *td) {
    long result = 0;
    libeel_enigma_ctx user_enigma;
    struct dev_enigma_usage_line_ctx *ulp;
    int uline;

    if (dev->si_drv1 == NULL) {
        return -EINVAL;
    }

    uline = *(int *)dev->si_drv1;

    ulp = dev_uline_ctx(uline);

    if (ulp == NULL) {
        return -EINVAL;
    }

    switch (cmd) {

        case ENIGMA_RESET:
            if (ulp->has_init) {

                if (!lock_uline(uline)) {
                    return -EBUSY;
                }

                ulp->has_init = libeel_init_machine(ulp->enigma);

                if (!ulp->has_init) {
                    result = -EINVAL;
                } else if (ulp->ebuf_head != NULL) {
                    del_ebuf_ctx(ulp->ebuf_head);
                    ulp->ebuf_head = NULL;
                }

                unlock_uline(uline);

            } else {
                result = -EINVAL;
            }
            break;

        case ENIGMA_SET:
            if (data == NULL) {
                return -EFAULT;
            }

            if (!lock_uline(uline)) {
                return -EBUSY;
            }

            memcpy(ulp->enigma, (libeel_enigma_ctx *)data, sizeof(libeel_enigma_ctx));

            if (!(ulp->has_init = libeel_init_machine(ulp->enigma))) {
                result = -EINVAL;
            } else if (ulp->ebuf_head != NULL) {
                del_ebuf_ctx(ulp->ebuf_head);
                ulp->ebuf_head = NULL;
            }

            unlock_uline(uline);
            break;

        case ENIGMA_SET_DEFAULT_SETTING:
            if (data == NULL) {
                return -EFAULT;
            }

            memcpy(&user_enigma, (libeel_enigma_ctx *)data, sizeof(libeel_enigma_ctx));

            if (!set_default_enigma_setting(&user_enigma)) {
                result = -EINVAL;
            }
            break;

        case ENIGMA_UNSET_DEFAULT_SETTING:
            if (!unset_default_enigma_setting()) {
                result = -EFAULT;
            }
            break;

        default:
            result = -ENOTTY;
            break;

    }

    return result;
}
