/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef DEV_ENIGMA_ENIGMACTL_H
#define DEV_ENIGMA_ENIGMACTL_H 1

#if defined(__linux__)
#include <linux/ioctl.h>
#endif

//  WARN(Santiago): Do not include eel on this way -> "../eel/eel.h" <-
//                  Because code from userland will not be able to use this header.

#include <eel.h>

struct enigmactl_usage_line_ctx {
    int uline;
    libeel_enigma_ctx *setting;
};

#define ENIGMA_IOC_MAGIC 'E'   //  WARN(Santiago): I did not pick a valid IOC_MAGIC, for sure that it will clash.

#define ENIGMA_RESET _IO(ENIGMA_IOC_MAGIC, 0)
#define ENIGMA_SET _IOW(ENIGMA_IOC_MAGIC, 1, libeel_enigma_ctx)
#define ENIGMA_RESET_USAGE_LINE _IO(ENIGMA_IOC_MAGIC, 2)
#define ENIGMA_SET_USAGE_LINE _IOW(ENIGMA_IOC_MAGIC, 3, enigmactl_usage_line_ctx)

#endif
