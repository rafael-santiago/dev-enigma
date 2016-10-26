/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <mod_init.h>
#include <mod_exit.h>

#if defined(__linux__)

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rafael Santiago");
MODULE_DESCRIPTION("An Enigma machine simulator as char device");
MODULE_VERSION("0.0.1");

static int __init ini(void) {
    return enigma_init();
}

static void __exit finis(void) {
    enigma_exit();
}

module_init(ini);
module_exit(finis);

#elif defined(__FreeBSD__)

#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/conf.h>

static int enigma_modevent(module_t mod __unused, int event, void *arg __unused) {
    int error = 0;

    switch (event) {
        case MOD_LOAD:
            error = enigma_init();
            break;

        case MOD_UNLOAD:
            enigma_exit();
            break;

        default:
            error = EOPNOTSUPP;
            break;
    }

    return error;
}

DEV_MODULE(enigma, enigma_modevent, NULL);

#endif
