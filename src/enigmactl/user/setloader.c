/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "setloader.h"
#include "option.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

// WARN(Santiago): This code module is rather boring, you should not spend your time reading this piece of shit.

typedef int (*setting_verifier)(const char *data);

static int get_rotor_value(const char *data);

static int is_valid_rotor(const char *data);

static int is_valid_ring(const char *data);

static int is_valid_number(const char *data);

static int is_valid_plugboard(const char *data);

static int is_valid_rotor_pos(const char *data);


typedef int (*plugboard_verifier)(int c);

static int isnext(int c);

static int isbar(int c);


typedef void (*setting_loader)(libeel_enigma_ctx *enigma, const char *data);

static void ld_l_rotor(libeel_enigma_ctx *enigma, const char *data);

static void ld_m_rotor(libeel_enigma_ctx *enigma, const char *data);

static void ld_r_rotor(libeel_enigma_ctx *enigma, const char *data);

static void ld_l_rotor_pos(libeel_enigma_ctx *enigma, const char *data);

static void ld_m_rotor_pos(libeel_enigma_ctx *enigma, const char *data);

static void ld_r_rotor_pos(libeel_enigma_ctx *enigma, const char *data);

static void ld_l_ring(libeel_enigma_ctx *enigma, const char *data);

static void ld_m_ring(libeel_enigma_ctx *enigma, const char *data);

static void ld_r_ring(libeel_enigma_ctx *enigma, const char *data);

static void ld_plugboard(libeel_enigma_ctx *enigma, const char *data);

static int isnext(int c) {
    return (c == ',' || c == 0);
}

static int isbar(int c) {
    return (c == '/');
}

static int is_valid_number(const char *data) {
    const char *dp = NULL, *dp_end = NULL;

    if (data == NULL) {
        return 0;
    }

    dp = data;
    dp_end = dp + strlen(data);

    while (dp != dp_end) {
        if (!isdigit(*dp)) {
            return 0;
        }
    }

    return 1;
}

static int is_valid_ring(const char *data) {
    int value;
    return (is_valid_number(data) && (value = atoi(data)) >= 1 && value <= 26);
}

static int is_valid_rotor_pos(const char *data) {
    return (data != NULL && isalpha(*data));
}

static int is_valid_plugboard(const char *data) {
    const char *dp = NULL, *dp_end = NULL;
    int state = 0;
    plugboard_verifier vprogram[] = {
        isalpha,
        isbar,
        isalpha,
        isnext
    };
    const size_t vprogram_state_nr = sizeof(vprogram) / sizeof(vprogram[0]);
    size_t swaps_nr = 0;

    dp = data;
    dp_end = dp + strlen(dp);

    while (dp != dp_end && swaps_nr < 6) {
        if (!vprogram[state](*dp)) {
            printf("ERROR: Invalid plugboard setting.\n");
            return 0;
        }
        state = (state + 1) % vprogram_state_nr;
        if (state == 3) {
            swaps_nr++;
        }
        dp++;
    }

    if (swaps_nr > 6) {
        printf("ERROR: Not enough space to make the requested swaps in the plugboard.\n");
        return 0;
    }

    if (*dp != 0 || state != 3) {
        printf("ERROR: Unterminated plugboard setting.\n");
        return 0;
    }

    return 1;
}

static int is_valid_rotor(const char *data) {
    return (get_rotor_value(data) != -1);
}

static int get_rotor_value(const char *data) {
    static char *rotors[] = {
        "i",
        "ii",
        "iii",
        "iv",
        "v",
        "vi",
        "vii",
        "viii"
    };
    const size_t rotors_nr = sizeof(rotors) / sizeof(rotors[0]);
    size_t r;
    char temp[255];
    char *tp = NULL, *tp_end = NULL;
    const char *dp = NULL, *dp_end = NULL;

    if (data == NULL) {
        return -1;
    }

    dp = data;
    dp_end = dp + strlen(dp);
    tp = &temp[0];
    tp_end = tp + sizeof(temp);
    memset(temp, 0, sizeof(temp));

    while (dp != dp_end) {
        *tp = tolower(*dp);
        dp++;
        tp++;
    }

    for (r = 0; r < rotors_nr; r++) {
        if (strcmp(data, rotors[r]) == 0) {
            return r;
        }
    }

    return -1;
}

//  INFO(Santiago): Well, let's avoid being so pedantic here, if everything is fine after verifying
//                  we do not need to re-check all that boring stuff again.
//
//                  (It would be pretty stupid...)

static void ld_l_rotor(libeel_enigma_ctx *enigma, const char *data) {
    enigma->left_rotor = get_rotor_value(data);
}

static void ld_m_rotor(libeel_enigma_ctx *enigma, const char *data) {
    enigma->middle_rotor = get_rotor_value(data);
}

static void ld_r_rotor(libeel_enigma_ctx *enigma, const char *data) {
    enigma->right_rotor = get_rotor_value(data);
}

static void ld_l_rotor_pos(libeel_enigma_ctx *enigma, const char *data) {
    libeel_rotor_at(enigma, l) = *data;
}

static void ld_m_rotor_pos(libeel_enigma_ctx *enigma, const char *data) {
    libeel_rotor_at(enigma, m) = *data;
}

static void ld_r_rotor_pos(libeel_enigma_ctx *enigma, const char *data) {
    libeel_rotor_at(enigma, r) = *data;
}

static void ld_l_ring(libeel_enigma_ctx *enigma, const char *data) {
    libeel_ring(enigma, l) = atoi(data);
}

static void ld_m_ring(libeel_enigma_ctx *enigma, const char *data) {
    libeel_ring(enigma, m) = atoi(data);
}

static void ld_r_ring(libeel_enigma_ctx *enigma, const char *data) {
    libeel_ring(enigma, r) = atoi(data);
}

static void ld_plugboard(libeel_enigma_ctx *enigma, const char *data) {
    const char *dp = data;
    const char *dp_end = dp + strlen(dp);
    int swap = 0;

    while (dp < dp_end) {
        switch (swap) {
            case 0:
                libeel_plugboard(enigma, 1).l = *dp;
                libeel_plugboard(enigma, 1).r = *(dp + 2);
                break;

            case 1:
                libeel_plugboard(enigma, 2).l = *dp;
                libeel_plugboard(enigma, 2).r = *(dp + 2);
                break;

            case 2:
                libeel_plugboard(enigma, 3).l = *dp;
                libeel_plugboard(enigma, 3).r = *(dp + 2);
                break;

            case 3:
                libeel_plugboard(enigma, 4).l = *dp;
                libeel_plugboard(enigma, 4).r = *(dp + 2);
                break;

            case 4:
                libeel_plugboard(enigma, 5).l = *dp;
                libeel_plugboard(enigma, 5).r = *(dp + 2);
                break;

            case 5:
                libeel_plugboard(enigma, 6).l = *dp;
                libeel_plugboard(enigma, 6).r = *(dp + 2);
                break;
        }

        dp += 3;
    }
}

// WARN(Santiago): Go to sleep! :)

libeel_enigma_ctx *ld_enigma_setting(void) {
    struct enigma_loader_program {
        const char *option;
        const int mandatory;
        setting_verifier verify;
        setting_loader load;
    };
    struct enigma_loader_program ld_program[] = {
        { "l-rotor",    1, is_valid_rotor,     ld_l_rotor     },
        { "m-rotor",    1, is_valid_rotor,     ld_m_rotor     },
        { "r-rotor",    1, is_valid_rotor,     ld_r_rotor     },
        { "l-rotor-at", 1, is_valid_rotor_pos, ld_l_rotor_pos },
        { "m-rotor-at", 1, is_valid_rotor_pos, ld_m_rotor_pos },
        { "r-rotor-at", 1, is_valid_rotor_pos, ld_r_rotor_pos },
        { "l-ring",     0, is_valid_ring,      ld_l_ring      },
        { "m-ring",     0, is_valid_ring,      ld_m_ring      },
        { "r-ring",     0, is_valid_ring,      ld_r_ring      },
        { "plugboard",  0, is_valid_plugboard, ld_plugboard   }
    };
    const size_t ld_program_size = sizeof(ld_program) / sizeof(ld_program[0]);
    char *data = NULL;
    size_t program_counter = 0;
    libeel_enigma_ctx *enigma = libeel_new_enigma_ctx();

    while (program_counter++ < ld_program_size) {
        data = get_option(ld_program[program_counter].option, NULL);

        if (data == NULL && !ld_program[program_counter].mandatory) {
            continue;
        } if (data == NULL) {
            printf("ERROR: The mandatory option '%s' is lacking.\n", ld_program[program_counter].option);
            goto ___ld_error_epilogue; //  WARN(Santiago): Hold on your friend's hand and try not pee in your pants.
        }

        if (!ld_program[program_counter].verify(data)) {
            printf("ERROR: The option '%s' has invalid data.\n", ld_program[program_counter].option);
            goto ___ld_error_epilogue; //  WARN(Santiago): Hold on your friend's hand and try not pee in your pants.
        }

        ld_program[program_counter].load(enigma, data);
    }

    return enigma;

___ld_error_epilogue:
    libeel_del_enigma_ctx(enigma);

    return NULL;
}
