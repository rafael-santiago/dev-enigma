/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "dev_ioctl.h"
#include "enigmactl.h"
#include <dev_ctx.h>
#include <eel.h>
#include <asm/uaccess.h>

long dev_ioctl(struct file *fp, unsigned int cmd, unsigned long usr_param) {
    int result = 0;
    libeel_enigma_ctx user_enigma;
    struct dev_enigma_usage_line_ctx *ulp;

    if (fp->private_data == NULL) {
        return -EINVAL;
    }

    ulp = dev_uline_ctx(*(int *)fp->private_data);

    switch (cmd) {

        case ENIGMA_RESET:
            if (ulp->has_init) {

                ulp->has_init = libeel_init_machine(ulp->enigma);

                if (!ulp->has_init) {
                    result = -EINVAL;
                } else if (ulp->ebuf_head != NULL) {
                    del_ebuf_ctx(ulp->ebuf_head);
                    ulp->ebuf_head = NULL;
                }

            } else {
                result = -EINVAL;
            }
            break;

        case ENIGMA_SET:
            if (!access_ok(VERIFY_READ, (void __user *)usr_param, _IOC_SIZE(cmd))) {
                return -EFAULT;
            }

            user_enigma = *((libeel_enigma_ctx *)usr_param);

            ulp->enigma->left_rotor = user_enigma.left_rotor;
            ulp->enigma->middle_rotor = user_enigma.middle_rotor;
            ulp->enigma->right_rotor = user_enigma.right_rotor;

            libeel_rotor_at(ulp->enigma, l) = libeel_rotor_at(&user_enigma, l);
            libeel_rotor_at(ulp->enigma, m) = libeel_rotor_at(&user_enigma, m);
            libeel_rotor_at(ulp->enigma, r) = libeel_rotor_at(&user_enigma, r);

            ulp->enigma->reflector = user_enigma.reflector;

            libeel_plugboard(ulp->enigma, 1).l = libeel_plugboard(&user_enigma, 1).l;
            libeel_plugboard(ulp->enigma, 1).r = libeel_plugboard(&user_enigma, 1).r;
            libeel_plugboard(ulp->enigma, 2).l = libeel_plugboard(&user_enigma, 2).l;
            libeel_plugboard(ulp->enigma, 2).r = libeel_plugboard(&user_enigma, 2).r;
            libeel_plugboard(ulp->enigma, 3).l = libeel_plugboard(&user_enigma, 3).l;
            libeel_plugboard(ulp->enigma, 3).r = libeel_plugboard(&user_enigma, 3).r;
            libeel_plugboard(ulp->enigma, 4).l = libeel_plugboard(&user_enigma, 4).l;
            libeel_plugboard(ulp->enigma, 4).r = libeel_plugboard(&user_enigma, 4).r;
            libeel_plugboard(ulp->enigma, 5).l = libeel_plugboard(&user_enigma, 5).l;
            libeel_plugboard(ulp->enigma, 5).r = libeel_plugboard(&user_enigma, 5).r;
            libeel_plugboard(ulp->enigma, 6).l = libeel_plugboard(&user_enigma, 6).l;
            libeel_plugboard(ulp->enigma, 6).r = libeel_plugboard(&user_enigma, 6).r;

            memset(&user_enigma, 0, sizeof(user_enigma));

            if (!(ulp->has_init = libeel_init_machine(ulp->enigma))) {
                result = -EINVAL;
            } else if (ulp->ebuf_head != NULL) {
                del_ebuf_ctx(ulp->ebuf_head);
                ulp->ebuf_head = NULL;
            }
            break;

        default:
            result = -EINVAL;
            break;

    }

    return result;
}
