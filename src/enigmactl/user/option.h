/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef ENIGMACTL_OPTION_H
#define ENIGMACTL_OPTION_H 1

char *get_option(const char *option, char *default_value);

int get_bool_option(const char *option, int default_value);

void set_argc_argv(int argc, char **argv);

#endif
