/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "mod_exit.h"
#include "mod_info.h"
#include <dev_ctx.h>
#include <eel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mutex.h>

void __exit enigma_exit(void) {
    libeel_enigma_ctx *enigma;

    printk(KERN_INFO "dev/enigma: The /dev/enigma is being unloaded...\n");

    deinit_ulines();

    mutex_destroy(&dev_ctx()->lock);
    enigma = dev_ctx()->default_setting;
    if (enigma != NULL) {
        libeel_del_enigma_ctx(enigma);
    }

    device_destroy(dev_ctx()->device_class, MKDEV(dev_ctx()->major_nr, 0));

    class_unregister(dev_ctx()->device_class);
    class_destroy(dev_ctx()->device_class);

    unregister_chrdev(dev_ctx()->major_nr, DEVNAME);

    printk(KERN_INFO "dev/enigma: Done.\n");
}
