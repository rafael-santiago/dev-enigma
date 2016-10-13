#ifndef DEV_ENIGMA_ENIGMACTL_H
#define DEV_ENIGMA_ENIGMACTL_H 1

#if defined(__linux__)
#include <linux/ioctl.h>
#endif

#define ENIGMA_IOC_MAGIC 'E'

#define ENIGMA_RESET _IO(ENIGMA_IOC_MAGIC, 0)
#define ENIGMA_SET _IOW(ENIGMA_IOC_MAGIC, 1, libeel_enigma_ctx)

#endif
