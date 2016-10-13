/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <mod_init.h>
#include <mod_exit.h>
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
