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

    switch (cmd) {

        case ENIGMA_RESET:
            if (dev_ctx()->has_init) {

                dev_ctx()->has_init = libeel_init_machine(dev_ctx()->enigma);

                if (!dev_ctx()->has_init) {
                    result = -EINVAL;
                } else if (dev_ctx()->ebuf != NULL) {
                    del_ebuf_ctx(dev_ctx()->ebuf);
                    dev_ctx()->ebuf = NULL;
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

            dev_ctx()->enigma->left_rotor = user_enigma.left_rotor;
            dev_ctx()->enigma->middle_rotor = user_enigma.middle_rotor;
            dev_ctx()->enigma->right_rotor = user_enigma.right_rotor;

            libeel_rotor_at(dev_ctx()->enigma, l) = libeel_rotor_at(&user_enigma, l);
            libeel_rotor_at(dev_ctx()->enigma, m) = libeel_rotor_at(&user_enigma, m);
            libeel_rotor_at(dev_ctx()->enigma, r) = libeel_rotor_at(&user_enigma, r);

            dev_ctx()->enigma->reflector = user_enigma.reflector;

            libeel_plugboard(dev_ctx()->enigma, 1).l = libeel_plugboard(&user_enigma, 1).l;
            libeel_plugboard(dev_ctx()->enigma, 1).r = libeel_plugboard(&user_enigma, 1).r;
            libeel_plugboard(dev_ctx()->enigma, 2).l = libeel_plugboard(&user_enigma, 2).l;
            libeel_plugboard(dev_ctx()->enigma, 2).r = libeel_plugboard(&user_enigma, 2).r;
            libeel_plugboard(dev_ctx()->enigma, 3).l = libeel_plugboard(&user_enigma, 3).l;
            libeel_plugboard(dev_ctx()->enigma, 3).r = libeel_plugboard(&user_enigma, 3).r;
            libeel_plugboard(dev_ctx()->enigma, 4).l = libeel_plugboard(&user_enigma, 4).l;
            libeel_plugboard(dev_ctx()->enigma, 4).r = libeel_plugboard(&user_enigma, 4).r;
            libeel_plugboard(dev_ctx()->enigma, 5).l = libeel_plugboard(&user_enigma, 5).l;
            libeel_plugboard(dev_ctx()->enigma, 5).r = libeel_plugboard(&user_enigma, 5).r;
            libeel_plugboard(dev_ctx()->enigma, 6).l = libeel_plugboard(&user_enigma, 6).l;
            libeel_plugboard(dev_ctx()->enigma, 6).r = libeel_plugboard(&user_enigma, 6).r;

            memset(&user_enigma, 0, sizeof(user_enigma));

            if (!(dev_ctx()->has_init = libeel_init_machine(dev_ctx()->enigma))) {
                result = -EINVAL;
            } else if (dev_ctx()->ebuf != NULL) {
                del_ebuf_ctx(dev_ctx()->ebuf);
                dev_ctx()->ebuf = NULL;
            }
            break;

        default:
            result = -EINVAL;
            break;

    }

    return result;
}
