#include <eel.h>
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

libeel_enigma_ctx *g_enigma = NULL;

static int __init enigma_init(void) {
    printk(KERN_INFO "dev/enigma: Initializing the /dev/enigma...\n");
    g_enigma = libeel_new_enigma_ctx();
    if (g_enigma != NULL) {
        printk(KERN_INFO "dev/enigma: We got a new context.\n");
    } else {
        printk(KERN_INFO "dev/enigma: Error during libeel_new_enigma_ctx().\n");
    }
    printk(KERN_INFO "dev/enigma: Done.\n");
    return 0;
}

static void __exit enigma_exit(void) {
    printk(KERN_INFO "dev/enigma: The /dev/enigma was being unloaded...\n");
    printk(KERN_INFO "dev/enigma: Done.\n");
    if (g_enigma != NULL) {
        libeel_del_enigma_ctx(g_enigma);
        printk(KERN_INFO "dev/enigma: Freed context.\n");
    }
}

module_init(enigma_init);
module_exit(enigma_exit);
