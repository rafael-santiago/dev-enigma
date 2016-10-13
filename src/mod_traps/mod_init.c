/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "mod_init.h"
#include "mod_info.h"
#include <dev_ctx.h>
#include <eel.h>
#include <dev_open.h>
#include <dev_release.h>
#include <dev_read.h>
#include <dev_write.h>
#include <dev_ioctl.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>

static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
    .unlocked_ioctl = dev_ioctl
};

int __init enigma_init(void) {
    printk(KERN_INFO "dev/enigma: Initializing the /dev/enigma...\n");
    dev_ctx()->enigma = libeel_new_enigma_ctx();

    if (dev_ctx()->enigma == NULL) {
        printk(KERN_INFO "dev/enigma: Error during libeel_new_enigma_ctx().\n");
        return 1;
    }

    dev_ctx()->major_nr = register_chrdev(0, DEVNAME, &fops);

    if (dev_ctx()->major_nr < 0) {
        printk(KERN_INFO "dev/enigma: device registration has failed.\n");
        return dev_ctx()->major_nr;
    }

    printk(KERN_INFO "dev/enigma: device registered under the number %d.\n", dev_ctx()->major_nr);

    dev_ctx()->device_class = class_create(THIS_MODULE, CLASS_NAME);

    if (IS_ERR(dev_ctx()->device_class)) {
        unregister_chrdev(dev_ctx()->major_nr, DEVNAME);
        printk(KERN_INFO "dev/enigma: class creating fail.\n");
        return PTR_ERR(dev_ctx()->device_class);
    }

    printk(KERN_INFO "dev/enigma: device class successfully created.\n");

    dev_ctx()->device = device_create(dev_ctx()->device_class, NULL, MKDEV(dev_ctx()->major_nr, 0), NULL, DEVNAME);

    if (IS_ERR(dev_ctx()->device)) {
        class_destroy(dev_ctx()->device_class);
        unregister_chrdev(dev_ctx()->major_nr, DEVNAME);
        printk(KERN_INFO "dev/enigma: device creation fail.\n");
        return PTR_ERR(dev_ctx()->device);
    }

    mutex_init(&dev_ctx()->lock);

    dev_ctx()->ebuf = NULL;

    printk(KERN_INFO "dev/enigma: Done.\n");

    return 0;
}
