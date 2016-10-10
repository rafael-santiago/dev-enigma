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

static int __init enigma_init(void) {
    printk(KERN_INFO "dev/enigma: Initializing the /dev/enigma...\n");
    printk(KERN_INFO "dev/enigma: Done.\n");
    return 0;
}

static void __exit enigma_exit(void) {
    printk(KERN_INFO "dev/enigma: The /dev/enigma was being unloaded...\n");
    printk(KERN_INFO "dev/enigma: Done.\n");
}

module_init(enigma_init);
module_exit(enigma_exit);
