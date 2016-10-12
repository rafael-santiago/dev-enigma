/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "keel.h"
#include "enigmactl.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVNAME "enigma"
#define CLASS_NAME "ems"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rafael Santiago");
MODULE_DESCRIPTION("An Enigma machine simulator");
MODULE_VERSION("0.0.1");

static int dev_open(struct inode *ip, struct file *fp);
static int dev_release(struct inode *ip, struct file *fp);
static ssize_t dev_read(struct file *fp, char *buf, size_t len, loff_t *loff);
static ssize_t dev_write(struct file *fp, const char *buf, size_t len, loff_t *loff);
static long dev_ioctl(struct file *fp, unsigned int cmd, unsigned long usr_param);

static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
    .unlocked_ioctl = dev_ioctl
};

struct dev_enigma_ctx {
    libeel_enigma_ctx *enigma;
    int major_nr;
    struct class *device_class;
    struct device *device;
    int has_init;
};

static struct dev_enigma_ctx g_dev_enigma = { 0 };

static int __init enigma_init(void) {
    printk(KERN_INFO "dev/enigma: Initializing the /dev/enigma...\n");
    g_dev_enigma.enigma = libeel_new_enigma_ctx();

    if (g_dev_enigma.enigma == NULL) {
        printk(KERN_INFO "dev/enigma: Error during libeel_new_enigma_ctx().\n");
    }

    g_dev_enigma.major_nr = register_chrdev(0, DEVNAME, &fops);

    if (g_dev_enigma.major_nr < 0) {
        printk(KERN_INFO "dev/enigma: device registration has failed.\n");
        return g_dev_enigma.major_nr;
    }

    printk(KERN_INFO "dev/enigma: device registered under the number %d.\n", g_dev_enigma.major_nr);

    g_dev_enigma.device_class = class_create(THIS_MODULE, CLASS_NAME);

    if (IS_ERR(g_dev_enigma.device_class)) {
        unregister_chrdev(g_dev_enigma.major_nr, DEVNAME);
        printk(KERN_INFO "dev/enigma: class creating fail.\n");
        return PTR_ERR(g_dev_enigma.device_class);
    }

    printk(KERN_INFO "dev/enigma: device class successfully created.\n");

    g_dev_enigma.device = device_create(g_dev_enigma.device_class, NULL, MKDEV(g_dev_enigma.major_nr, 0), NULL, DEVNAME);

    if (IS_ERR(g_dev_enigma.device)) {
        class_destroy(g_dev_enigma.device_class);
        unregister_chrdev(g_dev_enigma.major_nr, DEVNAME);
        printk(KERN_INFO "dev/enigma: device creation fail.\n");
        return PTR_ERR(g_dev_enigma.device);
    }

    printk(KERN_INFO "dev/enigma: Done.\n");

    return 0;
}

static void __exit enigma_exit(void) {
    printk(KERN_INFO "dev/enigma: The /dev/enigma is being unloaded...\n");
    if (g_dev_enigma.enigma != NULL) {
        libeel_del_enigma_ctx(g_dev_enigma.enigma);
    }
    device_destroy(g_dev_enigma.device_class, MKDEV(g_dev_enigma.major_nr, 0));
    class_unregister(g_dev_enigma.device_class);
    class_destroy(g_dev_enigma.device_class);
    unregister_chrdev(g_dev_enigma.major_nr, DEVNAME);
    printk(KERN_INFO "dev/enigma: Done.\n");
}

static int dev_open(struct inode *ip, struct file *fp) {
    return 0;
}

static int dev_release(struct inode *ip, struct file *fp) {
    return 0;
}

static ssize_t dev_read(struct file *fp, char *buf, size_t len, loff_t *loff) {
    return 0;
}

static ssize_t dev_write(struct file *fp, const char *buf, size_t len, loff_t *loff) {
    return 0;
}

static long dev_ioctl(struct file *fp, unsigned int cmd, unsigned long usr_param) {
    int result = 0;

    switch (cmd) {
        case ENIGMA_RESET:
            if (g_dev_enigma.has_init) {
                g_dev_enigma.has_init = libeel_init_machine(g_dev_enigma.enigma);
                if (!g_dev_enigma.has_init) {
                    result = -EINVAL;
                }
            } else {
                result = -EINVAL;
            }
            break;

        case ENIGMA_SET:
            if (!access_ok(VERIFY_READ, (void __user *)usr_param, sizeof(libeel_enigma_ctx))) {
                return -EFAULT;
            }
            break;

        default:
            result = -EINVAL;
            break;
    }

    return result;
}


module_init(enigma_init);
module_exit(enigma_exit);
