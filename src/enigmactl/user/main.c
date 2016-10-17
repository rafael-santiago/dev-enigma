/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "option.h"
#include "setloader.h"
#include <mod_info.h>
#include <enigmactl.h>
#include <eel.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

static int enigmactl_help(void) {
    if (get_bool_option("default-setting", 0)) {
        printf("usage: enigmactl --default-setting --l-rotor=<rotor> --m-rotor=<rotor> --r-rotor=<rotor> --l-rotor-at=<letter> --m-rotor-at=<letter> --r-rotor-at=<letter> [--l-ring=<pos> --m-ring=<pos> --r-ring=<pos> --plugboard=SWP1/SWP1',...,SWP6/SWP6']\n");
    } else {
        printf("usage: enigmactl --<operation> args...\n\n"
               "By the way, the operations are: 'default-setting' and 'help'.\n\n"
               "enigmactl is Copyright (C) 2016 by Rafael Santiago.\n"
               "Bug reports, feedback, etc: <voidbrainvoid@gmail.com> or <https://github.com/rafael-santiago/dev-enigma/issues>\n");
    }
    return 0;
}

static int enigmactl_default_setting(void) {
    libeel_enigma_ctx *enigma_setting;
    int result = 1;
    int dev = open("/dev/" DEVNAME, O_RDWR);

    if (dev < 0) {
        printf("ERROR: unable to open '%s'.\n", "/dev/" DEVNAME);
        return 1;
    }

    enigma_setting = ld_enigma_setting();

    if (enigma_setting != NULL) {
        result = ioctl(dev, ENIGMA_SET_DEFAULT_SETTING, enigma_setting);

        if (result != 0) {
            perror("ioctl");
        }
    }

    close(dev);

    return result;
}

int main(int argc, char **argv) {
    set_argc_argv(argc, argv);

    if (get_bool_option("help", 0)) {
        return enigmactl_help();
    }

    if (get_bool_option("default-setting", 0)) {
        return enigmactl_default_setting();
    }

    return enigmactl_help();
}
