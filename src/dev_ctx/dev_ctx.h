#ifndef DEV_ENIGMA_DEV_CTX_H
#define DEV_ENIGMA_DEV_CTX_H 1

#include <eel.h>
#include <ebuf.h>
#include <linux/mutex.h>

struct dev_enigma_ctx {
    libeel_enigma_ctx *enigma;
    ebuf_ctx *ebuf, *tail;
    int major_nr;
    struct class *device_class;
    struct device *device;
    int has_init;
    int has_open;
    struct mutex lock;
};

struct dev_enigma_ctx *dev_ctx(void);

#endif
