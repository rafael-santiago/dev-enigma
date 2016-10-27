/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "mod_quiesce.h"
#include <dev_ctx.h>

int safe_for_unloading(void) {
    int safe = 1;
    int u;

    for (u = 0; u < DEV_USAGE_LINES_NR && safe; u++) {
        if (!lock_uline(u)) {
            safe = 0;
        } else {
            safe = (dev_ctx()->ulines[u].enigma == NULL);
            unlock_uline(u);
        }
    }

    return safe;
}
