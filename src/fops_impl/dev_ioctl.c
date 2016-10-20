/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_ioctl.h"
#include <enigmactl.h>
#include <dev_ctx.h>
#include <eel.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

long dev_ioctl(struct file *fp, unsigned int cmd, unsigned long usr_param) {
    long result = 0;
    libeel_enigma_ctx user_enigma;
    struct dev_enigma_usage_line_ctx *ulp;
    int uline;

    if (fp->private_data == NULL) {
        return -EINVAL;
    }

    uline = *(int *)fp->private_data;

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
            if ((void *)usr_param == NULL ||
                !access_ok(VERIFY_READ, (void __user *)usr_param, _IOC_SIZE(cmd))) {
                return -EFAULT;
            }

            if (!lock_uline(uline)) {
                return -EBUSY;
            }

            if (copy_from_user(&user_enigma, (libeel_enigma_ctx *)usr_param, sizeof(libeel_enigma_ctx)) != 0) {
                unlock_uline(uline);
                return -EFAULT;
            }

            memcpy(ulp->enigma, &user_enigma, sizeof(libeel_enigma_ctx));

            memset(&user_enigma, 0, sizeof(user_enigma));

            if (!(ulp->has_init = libeel_init_machine(ulp->enigma))) {
                result = -EINVAL;
            } else if (ulp->ebuf_head != NULL) {
                del_ebuf_ctx(ulp->ebuf_head);
                ulp->ebuf_head = NULL;
            }

            unlock_uline(uline);
            break;

        case ENIGMA_SET_DEFAULT_SETTING:
            if ((void *)usr_param == NULL ||
                !access_ok(VERIFY_READ, (void __user *)usr_param, _IOC_SIZE(cmd))) {
                return -EFAULT;
            }

            if (copy_from_user(&user_enigma, (libeel_enigma_ctx *)usr_param, sizeof(libeel_enigma_ctx)) != 0) {
                return -EFAULT;
            }

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
            result = -EINVAL;
            break;

    }

    return result;
}
