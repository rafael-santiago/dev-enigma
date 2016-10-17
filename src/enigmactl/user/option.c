/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "option.h"
#include <stdio.h>
#include <string.h>

static int g_argc = 0;
static char **g_argv = NULL;

static void *getoptdata(const char *option, const char bool_option, void *default_value);

static void *getoptdata(const char *option, const char bool_option, void *default_value) {
    int o;
    char temp[255] = "";
    static int tru = 1;

    if (option == NULL || g_argc == 0 || g_argv == NULL) {
        return default_value;
    }

    sprintf(temp, "--%s%s", option, (!bool_option) ? "=" : "\0");

    for (o = 0; o < g_argc; o++) {
        if (strstr(g_argv[o], temp) == &g_argv[o][0]) {
            return  ((bool_option) ? (void *)&tru : (void *)&g_argv[o][0] + strlen(temp));
        }
    }

    return default_value;
}


char *get_option(const char *option, char *default_value) {
    return getoptdata(option, 0, default_value);
}

int get_bool_option(const char *option, int default_value) {
    return *((int *)getoptdata(option, 1, &default_value));
}

void set_argc_argv(int argc, char **argv) {
    g_argc = argc;
    g_argv = argv;
}
