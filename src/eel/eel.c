/*
 *                        Copyright (C) 2006, 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "eel.h"
#include <linux/ctype.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/slab.h>

struct libeel_rotor_wiring_ctx {
    libeel_rotor_wiring_t normal, inverse;
    libeel_notch_t notch;
};

static struct libeel_rotor_wiring_ctx g_rotor_wiring[] = {
    {
        {
            'E', 'K', 'M', 'F', 'L', 'G', 'D', 'Q', 'V', 'Z', 'N', 'T', 'O',
            'W', 'Y', 'H', 'X', 'U', 'S', 'P', 'A', 'I', 'B', 'R', 'C', 'J'
        },
        {
            'U', 'W', 'Y', 'G', 'A', 'D', 'F', 'P', 'V', 'Z', 'B', 'E', 'C',
            'K', 'M', 'T', 'H', 'X', 'S', 'L', 'R', 'I', 'N', 'Q', 'O', 'J'
        },
        'R'
    },
    {
        {
            'A', 'J', 'D', 'K', 'S', 'I', 'R', 'U', 'X', 'B', 'L', 'H', 'W',
            'T', 'M', 'C', 'Q', 'G', 'Z', 'N', 'P', 'Y', 'F', 'V', 'O', 'E'
        },
        {
            'A', 'J', 'P', 'C', 'Z', 'W', 'R', 'L', 'F', 'B', 'D', 'K', 'O',
            'T', 'Y', 'U', 'Q', 'G', 'E', 'N', 'H', 'X', 'M', 'I', 'V', 'S'
        },
        'F'
    },
    {
        {
            'B', 'D', 'F', 'H', 'J', 'L', 'C', 'P', 'R', 'T', 'X', 'V', 'Z',
            'N', 'Y', 'E', 'I', 'W', 'G', 'A', 'K', 'M', 'U', 'S', 'Q', 'O'
        },
        {
            'T', 'A', 'G', 'B', 'P', 'C', 'S', 'D', 'Q', 'E', 'U', 'F', 'V',
            'N', 'Z', 'H', 'Y', 'I', 'X', 'J', 'W', 'L', 'R', 'K', 'O', 'M'
        },
        'W'
    },
    {
        {
            'E', 'S', 'O', 'V', 'P', 'Z', 'J', 'A', 'Y', 'Q', 'U', 'I', 'R',
            'H', 'X', 'L', 'N', 'F', 'T', 'G', 'K', 'D', 'C', 'M', 'W', 'B'
        },
        {
            'H', 'Z', 'W', 'V', 'A', 'R', 'T', 'N', 'L', 'G', 'U', 'P', 'X',
            'Q', 'C', 'E', 'J', 'M', 'B', 'S', 'K', 'D', 'Y', 'O', 'I', 'F'
        },
        'K'
    },
    {
        {
            'V', 'Z', 'B', 'R', 'G', 'I', 'T', 'Y', 'U', 'P', 'S', 'D', 'N',
            'H', 'L', 'X', 'A', 'W', 'M', 'J', 'Q', 'O', 'F', 'E', 'C', 'K'
        },
        {
            'Q', 'C', 'Y', 'L', 'X', 'W', 'E', 'N', 'F', 'T', 'Z', 'O', 'S',
            'M', 'V', 'J', 'U', 'D', 'K', 'G', 'I', 'A', 'R', 'P', 'H', 'B'
        },
        'A'
    },
    {
        {
            'J', 'P', 'G', 'V', 'O', 'U', 'M', 'F', 'Y', 'Q', 'B', 'E', 'N',
            'H', 'Z', 'R', 'D', 'K', 'A', 'S', 'X', 'L', 'I', 'C', 'T', 'W'
        },
        {
            'S', 'K', 'X', 'Q', 'L', 'H', 'C', 'N', 'W', 'A', 'R', 'V', 'G',
            'M', 'E', 'B', 'J', 'P', 'T', 'Y', 'F', 'D', 'Z', 'U', 'I', 'O'
        },
        'A'
    },
    {
        {
            'N', 'Z', 'J', 'H', 'G', 'R', 'C', 'X', 'M', 'Y', 'S', 'W', 'B',
            'O', 'U', 'F', 'A', 'I', 'V', 'L', 'P', 'E', 'K', 'Q', 'D', 'T'
        },
        {
            'Q', 'M', 'G', 'Y', 'V', 'P', 'E', 'D', 'R', 'C', 'W', 'T', 'I',
            'A', 'N', 'U', 'X', 'F', 'K', 'Z', 'O', 'S', 'L', 'H', 'J', 'B'
        },
        'A'
    },
    {
        {
            'F', 'K', 'Q', 'H', 'T', 'L', 'X', 'O', 'C', 'B', 'J', 'S', 'P',
            'D', 'Z', 'R', 'A', 'M', 'E', 'W', 'N', 'I', 'U', 'Y', 'G', 'V'
        },
        {
            'Q', 'J', 'I', 'N', 'S', 'A', 'Y', 'D', 'V', 'K', 'B', 'F', 'R',
            'U', 'H', 'M', 'C', 'P', 'L', 'E', 'W', 'Z', 'T', 'G', 'X', 'O'
        },
        'N'
    }
};

static size_t g_rotor_wiring_nr = sizeof(g_rotor_wiring) / sizeof(g_rotor_wiring[0]);

static libeel_rotor_wiring_t g_reflector[] = {
    {
        'Y', 'R', 'U', 'H', 'Q', 'S', 'L', 'D', 'P', 'X', 'N', 'G', 'O',
        'K', 'M', 'I', 'E', 'R', 'F', 'Z', 'C', 'W', 'V', 'J', 'A', 'T'
    },
    {
        'F', 'V', 'P', 'J', 'I', 'A', 'O', 'Y', 'E', 'D', 'R', 'Z', 'X',
        'W', 'G', 'C', 'T', 'K', 'U', 'Q', 'S', 'B', 'N', 'M', 'H', 'L'
    }
};

static size_t g_reflector_nr = sizeof(g_reflector) / sizeof(g_reflector[0]);

static libeel_rotor_wiring_t g_basic_display = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};

static void libeel_set_rotor_position(libeel_rotor_wiring_t rotor, const libeel_rotor_display_t stop);

static void libeel_set_rotor(libeel_rotor_wiring_t rotor, libeel_rotor_wiring_t rotor_inv, const libeel_rotor_t r);

static void libeel_spin_rotor(libeel_rotor_wiring_t rotor);

static void libeel_spin_rotor_l(libeel_rotor_wiring_t rotor);

static void libeel_set_plugboard(libeel_enigma_ctx *enigma);

static void libeel_refresh_display(libeel_rotor_display_t *display);

static int libeel_setting_sanity_check(const libeel_enigma_ctx *enigma);

static libeel_rotor_display_t libeel_get_rotor_signal(const libeel_rotor_wiring_t rotor_offset, const libeel_rotor_wiring_t rotor, const char input);

static void libeel_set_reflector(libeel_rotor_wiring_t reflector, const libeel_reflector_t r);

static void libeel_clear_enigma_ctx(libeel_enigma_ctx *enigma);

int libeel_init_machine(libeel_enigma_ctx *enigma) {

    libeel_ring_t r;

    if (enigma == NULL) {
        return 0;
    }

    if (!libeel_setting_sanity_check(enigma)) {
        return 0;
    }

    libeel_set_rotor(libeel_rotor(enigma, l), libeel_rotor_inv(enigma, l), enigma->left_rotor);

    libeel_set_rotor(libeel_rotor(enigma, m), libeel_rotor_inv(enigma, m), enigma->middle_rotor);
    libeel_notch(enigma, m) = g_rotor_wiring[enigma->middle_rotor % g_rotor_wiring_nr].notch;

    libeel_set_rotor(libeel_rotor(enigma, r), libeel_rotor_inv(enigma, r), enigma->right_rotor);
    libeel_notch(enigma, r) = g_rotor_wiring[enigma->right_rotor % g_rotor_wiring_nr].notch;

    memcpy(libeel_rotor_offset(enigma, l), g_basic_display, sizeof(libeel_rotor_wiring_t));
    libeel_set_rotor_position(libeel_rotor_offset(enigma, l), libeel_rotor_at(enigma, l));

    memcpy(libeel_rotor_offset(enigma, m), g_basic_display, sizeof(libeel_rotor_wiring_t));
    libeel_set_rotor_position(libeel_rotor_offset(enigma, m), libeel_rotor_at(enigma, m));

    memcpy(libeel_rotor_offset(enigma, r), g_basic_display, sizeof(libeel_rotor_wiring_t));
    libeel_set_rotor_position(libeel_rotor_offset(enigma, r), libeel_rotor_at(enigma, r));

    for (r = 1; r < libeel_ring(enigma, l); r++) {
        libeel_spin_rotor_l(libeel_rotor_offset(enigma, l));
    }

    for (r = 1; r < libeel_ring(enigma, m); r++) {
        libeel_spin_rotor_l(libeel_rotor_offset(enigma, m));
    }

    for (r = 1; r < libeel_ring(enigma, r); r++) {
        libeel_spin_rotor_l(libeel_rotor_offset(enigma, r));
    }

    libeel_display(enigma, l) = libeel_rotor_at(enigma, l);
    libeel_display(enigma, m) = libeel_rotor_at(enigma, m);
    libeel_display(enigma, r) = libeel_rotor_at(enigma, r);

    memcpy(libeel_keyboard(enigma), g_basic_display, sizeof(libeel_rotor_wiring_t));

    libeel_set_plugboard(enigma);

    libeel_set_reflector(libeel_reflector(enigma), enigma->reflector);

    return 1;
}

static void libeel_set_rotor(libeel_rotor_wiring_t rotor, libeel_rotor_wiring_t rotor_inv, const libeel_rotor_t r) {
    memcpy(rotor, g_rotor_wiring[r % g_rotor_wiring_nr].normal, sizeof(libeel_rotor_wiring_t));
    memcpy(rotor_inv, g_rotor_wiring[r % g_rotor_wiring_nr].inverse, sizeof(libeel_rotor_wiring_t));
}

static void libeel_set_rotor_position(libeel_rotor_wiring_t rotor, const libeel_rotor_display_t stop) {
    int s = toupper(stop);
    while (s-- > 'A') {
        libeel_spin_rotor(rotor);
    }
}

static void libeel_spin_rotor(libeel_rotor_wiring_t rotor) {
    int s = 24;
    char aux = rotor[25];

    while (s >= 0) {
        rotor[s + 1] = rotor[s];
        s--;
    }

    rotor[0] = aux;
}

static void libeel_spin_rotor_l(libeel_rotor_wiring_t rotor) {
    int s = 1;
    char aux = rotor[0];

    while (s < 26) {
        rotor[s + 1] = rotor[s];
        s++;
    }

    rotor[25] = aux;
}

static void libeel_set_plugboard(libeel_enigma_ctx *enigma) {
    if (libeel_plugboard(enigma, 1).l != 0) {
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 1).l) - 'A'] = toupper(libeel_plugboard(enigma, 1).r);
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 1).r) - 'A'] = toupper(libeel_plugboard(enigma, 1).l);
    }

    if (libeel_plugboard(enigma, 2).l != 0) {
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 2).l) - 'A'] = toupper(libeel_plugboard(enigma, 2).r);
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 2).r) - 'A'] = toupper(libeel_plugboard(enigma, 2).l);
    }

    if (libeel_plugboard(enigma, 3).l != 0) {
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 3).l) - 'A'] = toupper(libeel_plugboard(enigma, 3).r);
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 3).r) - 'A'] = toupper(libeel_plugboard(enigma, 3).l);
    }

    if (libeel_plugboard(enigma, 4).l != 0) {
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 4).l) - 'A'] = toupper(libeel_plugboard(enigma, 4).r);
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 4).r) - 'A'] = toupper(libeel_plugboard(enigma, 4).l);
    }

    if (libeel_plugboard(enigma, 5).l != 0) {
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 5).l) - 'A'] = toupper(libeel_plugboard(enigma, 5).r);
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 5).r) - 'A'] = toupper(libeel_plugboard(enigma, 5).l);
    }

    if (libeel_plugboard(enigma, 6).l != 0) {
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 6).l) - 'A'] = toupper(libeel_plugboard(enigma, 6).r);
        libeel_keyboard(enigma)[toupper(libeel_plugboard(enigma, 6).r) - 'A'] = toupper(libeel_plugboard(enigma, 6).l);
    }
}

static void libeel_set_reflector(libeel_rotor_wiring_t reflector, const libeel_reflector_t r) {
    memcpy(reflector, g_reflector[r % g_reflector_nr], sizeof(libeel_rotor_wiring_t));
}

char libeel_type(libeel_enigma_ctx *enigma) {
    char outchar = 0;
    libeel_rotor_display_t m_pos = 0;
    int spin = 0, is_lwr = 0;

    if (enigma == NULL) {
        return 0;
    }

    outchar = libeel_enigma_input(enigma);

    if (isalpha(outchar)) {
        m_pos = libeel_display(enigma, m) + 1;

        if (m_pos > 'Z') {
            m_pos = 'A';
        }

        spin = (libeel_display(enigma, r) == libeel_notch(enigma, r) && m_pos == libeel_notch(enigma, m));

        libeel_spin_rotor(libeel_rotor_offset(enigma, r));

        libeel_refresh_display(&libeel_display(enigma, r));

        if (spin || libeel_display(enigma, r) == libeel_notch(enigma, r)) {
            libeel_spin_rotor(libeel_rotor_offset(enigma, m));

            libeel_refresh_display(&libeel_display(enigma, m));

            if (libeel_display(enigma, m) == libeel_notch(enigma, m)) {
                libeel_spin_rotor(libeel_rotor_offset(enigma, l));

                libeel_refresh_display(&libeel_display(enigma, l));
            }
        }

        is_lwr = islower(outchar);

        outchar = libeel_get_rotor_signal(g_basic_display, libeel_keyboard(enigma), toupper(outchar));

        outchar = libeel_get_rotor_signal(libeel_rotor_offset(enigma, r), libeel_rotor(enigma, r), outchar);
        outchar = libeel_get_rotor_signal(g_basic_display, libeel_rotor_offset(enigma, r), outchar);

        outchar = libeel_get_rotor_signal(libeel_rotor_offset(enigma, m), libeel_rotor(enigma, m), outchar);
        outchar = libeel_get_rotor_signal(g_basic_display, libeel_rotor_offset(enigma, m), outchar);

        outchar = libeel_get_rotor_signal(libeel_rotor_offset(enigma, l), libeel_rotor(enigma, l), outchar);
        outchar = libeel_get_rotor_signal(g_basic_display, libeel_rotor_offset(enigma, l), outchar);

        outchar = libeel_get_rotor_signal(libeel_reflector(enigma), g_basic_display, outchar);

        outchar = libeel_get_rotor_signal(libeel_rotor_offset(enigma, l), libeel_rotor_inv(enigma, l), outchar);
        outchar = libeel_get_rotor_signal(g_basic_display, libeel_rotor_offset(enigma, l), outchar);

        outchar = libeel_get_rotor_signal(libeel_rotor_offset(enigma, m), libeel_rotor_inv(enigma, m), outchar);
        outchar = libeel_get_rotor_signal(g_basic_display, libeel_rotor_offset(enigma, m), outchar);

        outchar = libeel_get_rotor_signal(libeel_rotor_offset(enigma, r), libeel_rotor_inv(enigma, r), outchar);
        outchar = libeel_get_rotor_signal(g_basic_display, libeel_rotor_offset(enigma, r), outchar);

        outchar = libeel_get_rotor_signal(libeel_keyboard(enigma), g_basic_display, outchar);

        if (is_lwr) {
            outchar = tolower(outchar);
        }

        libeel_enigma_output(enigma) = outchar;
    }

    return outchar;
}

void libeel_del_enigma_ctx(libeel_enigma_ctx *enigma) {
    if (enigma != NULL) {
        libeel_clear_enigma_ctx(enigma);
        kfree(enigma);
    }
}

static void libeel_clear_enigma_ctx(libeel_enigma_ctx *enigma) {
    if (enigma != NULL) {
        memset(enigma, 0, sizeof(libeel_enigma_ctx));
        libeel_ring(enigma, l) = 1;
        libeel_ring(enigma, m) = 1;
        libeel_ring(enigma, r) = 1;
    }
}

libeel_enigma_ctx *libeel_new_enigma_ctx(void) {
    libeel_enigma_ctx *enigma = (libeel_enigma_ctx *)kmalloc(sizeof(libeel_enigma_ctx), GFP_ATOMIC);
    libeel_clear_enigma_ctx(enigma);
    return enigma;
}

static libeel_rotor_display_t libeel_get_rotor_signal(const libeel_rotor_wiring_t rotor_offset, const libeel_rotor_wiring_t rotor, const char input) {
    int r;

    for (r = 0; r < 26; r++) {
        if (rotor_offset[r] == input) {
            return rotor[r];
        }
    }

    for (r = 0; r < 26; r++) {
        if (rotor[r] == input) {
            return rotor_offset[r];
        }
    }

    return 0; //  WARN(Santiago): On normal conditions it should never happen.
}

static void libeel_refresh_display(libeel_rotor_display_t *display) {
    if (display == NULL) {
        return;
    }

    *display = (toupper(*display) + 1) % 91;

    if (*display == 0) {
        *display = 'A';
    }
}

static int libeel_setting_sanity_check(const libeel_enigma_ctx *enigma) {
    if (enigma == NULL) {
        return 0;
    }

    if (!(enigma->left_rotor >= i && enigma->left_rotor <= viii)) {
        return 0;
    }

    if (!(enigma->middle_rotor >= i && enigma->middle_rotor <= viii)) {
        return 0;
    }

    if (!(enigma->right_rotor >= i && enigma->right_rotor <= viii)) {
        return 0;
    }

    if (enigma->left_rotor  == enigma->right_rotor   ||
        enigma->left_rotor  == enigma->middle_rotor  ||
        enigma->right_rotor == enigma->middle_rotor) {
        return 0;
    }

    if (!(enigma->reflector >= b && enigma->reflector <= c)) {
        return 0;
    }

    if (!(libeel_ring(enigma, l) >= 1 && libeel_ring(enigma, l) <= 26)) {
        return 0;
    }

    if (!(libeel_ring(enigma, m) >= 1 && libeel_ring(enigma, m) <= 26)) {
        return 0;
    }

    if (!(libeel_ring(enigma, r) >= 1 && libeel_ring(enigma, r) <= 26)) {
        return 0;
    }

    if ((libeel_plugboard(enigma, 1).l != 0 && !isalpha(libeel_plugboard(enigma, 1).l)) ||
        (libeel_plugboard(enigma, 1).r != 0 && !isalpha(libeel_plugboard(enigma, 1).r)) ||
        (libeel_plugboard(enigma, 1).l != 0 && libeel_plugboard(enigma, 1).r == 0)      ||
        (libeel_plugboard(enigma, 1).r != 0 && libeel_plugboard(enigma, 1).l == 0)) {
        return 0;
    }

    if ((libeel_plugboard(enigma, 2).l != 0 && !isalpha(libeel_plugboard(enigma, 2).l)) ||
        (libeel_plugboard(enigma, 2).r != 0 && !isalpha(libeel_plugboard(enigma, 2).r)) ||
        (libeel_plugboard(enigma, 2).l != 0 && libeel_plugboard(enigma, 2).r == 0)      ||
        (libeel_plugboard(enigma, 2).r != 0 && libeel_plugboard(enigma, 2).l == 0)) {
        return 0;
    }

    if ((libeel_plugboard(enigma, 3).l != 0 && !isalpha(libeel_plugboard(enigma, 3).l)) ||
        (libeel_plugboard(enigma, 3).r != 0 && !isalpha(libeel_plugboard(enigma, 3).r)) ||
        (libeel_plugboard(enigma, 3).l != 0 && libeel_plugboard(enigma, 3).r == 0)      ||
        (libeel_plugboard(enigma, 3).r != 0 && libeel_plugboard(enigma, 3).l == 0)) {
        return 0;
    }

    if ((libeel_plugboard(enigma, 4).l != 0 && !isalpha(libeel_plugboard(enigma, 4).l)) ||
        (libeel_plugboard(enigma, 4).r != 0 && !isalpha(libeel_plugboard(enigma, 4).r)) ||
        (libeel_plugboard(enigma, 4).l != 0 && libeel_plugboard(enigma, 4).r == 0)      ||
        (libeel_plugboard(enigma, 4).r != 0 && libeel_plugboard(enigma, 4).l == 0)) {
        return 0;
    }

    if ((libeel_plugboard(enigma, 5).l != 0 && !isalpha(libeel_plugboard(enigma, 5).l)) ||
        (libeel_plugboard(enigma, 5).r != 0 && !isalpha(libeel_plugboard(enigma, 5).r)) ||
        (libeel_plugboard(enigma, 5).l != 0 && libeel_plugboard(enigma, 5).r == 0)      ||
        (libeel_plugboard(enigma, 5).r != 0 && libeel_plugboard(enigma, 5).l == 0)) {
        return 0;
    }

    if ((libeel_plugboard(enigma, 6).l != 0 && !isalpha(libeel_plugboard(enigma, 6).l)) ||
        (libeel_plugboard(enigma, 6).r != 0 && !isalpha(libeel_plugboard(enigma, 6).r)) ||
        (libeel_plugboard(enigma, 6).l != 0 && libeel_plugboard(enigma, 6).r == 0)      ||
        (libeel_plugboard(enigma, 6).r != 0 && libeel_plugboard(enigma, 6).l == 0)) {
        return 0;
    }

    if ((libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 2).l) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 2).r) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 3).l) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 3).r) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 4).l) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 4).r) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 5).l) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 5).r) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).l == libeel_plugboard(enigma, 6).r) ||
        (libeel_plugboard(enigma, 1).l > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 2).l) ||
        (libeel_plugboard(enigma, 1).r > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 2).r) ||
        (libeel_plugboard(enigma, 1).r > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 3).l) ||
        (libeel_plugboard(enigma, 1).r > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 3).r) ||
        (libeel_plugboard(enigma, 1).r > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 4).l) ||
        (libeel_plugboard(enigma, 1).r > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 4).r) ||
        (libeel_plugboard(enigma, 1).r > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 5).l) ||
        (libeel_plugboard(enigma, 1).r > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 5).r) ||
        (libeel_plugboard(enigma, 1).r > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 1).r > 0 && libeel_plugboard(enigma, 1).r == libeel_plugboard(enigma, 6).r) ||
        (libeel_plugboard(enigma, 2).l > 0 && libeel_plugboard(enigma, 2).l == libeel_plugboard(enigma, 3).l) ||
        (libeel_plugboard(enigma, 2).l > 0 && libeel_plugboard(enigma, 2).l == libeel_plugboard(enigma, 3).r) ||
        (libeel_plugboard(enigma, 2).l > 0 && libeel_plugboard(enigma, 2).l == libeel_plugboard(enigma, 4).l) ||
        (libeel_plugboard(enigma, 2).l > 0 && libeel_plugboard(enigma, 2).l == libeel_plugboard(enigma, 4).r) ||
        (libeel_plugboard(enigma, 2).l > 0 && libeel_plugboard(enigma, 2).l == libeel_plugboard(enigma, 5).l) ||
        (libeel_plugboard(enigma, 2).l > 0 && libeel_plugboard(enigma, 2).l == libeel_plugboard(enigma, 5).r) ||
        (libeel_plugboard(enigma, 2).l > 0 && libeel_plugboard(enigma, 2).l == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 2).l > 0 && libeel_plugboard(enigma, 2).l == libeel_plugboard(enigma, 6).r) ||
        (libeel_plugboard(enigma, 2).l > 0 && libeel_plugboard(enigma, 2).r == libeel_plugboard(enigma, 3).l) ||
        (libeel_plugboard(enigma, 2).r > 0 && libeel_plugboard(enigma, 2).r == libeel_plugboard(enigma, 3).r) ||
        (libeel_plugboard(enigma, 2).r > 0 && libeel_plugboard(enigma, 2).r == libeel_plugboard(enigma, 4).l) ||
        (libeel_plugboard(enigma, 2).r > 0 && libeel_plugboard(enigma, 2).r == libeel_plugboard(enigma, 4).r) ||
        (libeel_plugboard(enigma, 2).r > 0 && libeel_plugboard(enigma, 2).r == libeel_plugboard(enigma, 5).l) ||
        (libeel_plugboard(enigma, 2).r > 0 && libeel_plugboard(enigma, 2).r == libeel_plugboard(enigma, 5).r) ||
        (libeel_plugboard(enigma, 2).r > 0 && libeel_plugboard(enigma, 2).r == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 2).r > 0 && libeel_plugboard(enigma, 2).r == libeel_plugboard(enigma, 6).r) ||
        (libeel_plugboard(enigma, 3).l > 0 && libeel_plugboard(enigma, 3).l == libeel_plugboard(enigma, 4).l) ||
        (libeel_plugboard(enigma, 3).l > 0 && libeel_plugboard(enigma, 3).l == libeel_plugboard(enigma, 4).r) ||
        (libeel_plugboard(enigma, 3).l > 0 && libeel_plugboard(enigma, 3).l == libeel_plugboard(enigma, 5).l) ||
        (libeel_plugboard(enigma, 3).l > 0 && libeel_plugboard(enigma, 3).l == libeel_plugboard(enigma, 5).r) ||
        (libeel_plugboard(enigma, 3).l > 0 && libeel_plugboard(enigma, 3).l == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 3).l > 0 && libeel_plugboard(enigma, 3).l == libeel_plugboard(enigma, 6).r) ||
        (libeel_plugboard(enigma, 3).r > 0 && libeel_plugboard(enigma, 3).r == libeel_plugboard(enigma, 4).l) ||
        (libeel_plugboard(enigma, 3).r > 0 && libeel_plugboard(enigma, 3).r == libeel_plugboard(enigma, 4).r) ||
        (libeel_plugboard(enigma, 3).r > 0 && libeel_plugboard(enigma, 3).r == libeel_plugboard(enigma, 5).l) ||
        (libeel_plugboard(enigma, 3).r > 0 && libeel_plugboard(enigma, 3).r == libeel_plugboard(enigma, 5).r) ||
        (libeel_plugboard(enigma, 3).r > 0 && libeel_plugboard(enigma, 3).r == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 3).r > 0 && libeel_plugboard(enigma, 3).r == libeel_plugboard(enigma, 6).r) ||
        (libeel_plugboard(enigma, 4).l > 0 && libeel_plugboard(enigma, 4).l == libeel_plugboard(enigma, 5).l) ||
        (libeel_plugboard(enigma, 4).l > 0 && libeel_plugboard(enigma, 4).l == libeel_plugboard(enigma, 5).r) ||
        (libeel_plugboard(enigma, 4).l > 0 && libeel_plugboard(enigma, 4).l == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 4).l > 0 && libeel_plugboard(enigma, 4).l == libeel_plugboard(enigma, 6).r) ||
        (libeel_plugboard(enigma, 4).r > 0 && libeel_plugboard(enigma, 4).r == libeel_plugboard(enigma, 5).l) ||
        (libeel_plugboard(enigma, 4).r > 0 && libeel_plugboard(enigma, 4).r == libeel_plugboard(enigma, 5).r) ||
        (libeel_plugboard(enigma, 4).r > 0 && libeel_plugboard(enigma, 4).r == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 4).r > 0 && libeel_plugboard(enigma, 4).r == libeel_plugboard(enigma, 6).r) ||
        (libeel_plugboard(enigma, 5).l > 0 && libeel_plugboard(enigma, 5).l == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 5).l > 0 && libeel_plugboard(enigma, 5).l == libeel_plugboard(enigma, 6).r) ||
        (libeel_plugboard(enigma, 5).r > 0 && libeel_plugboard(enigma, 5).r == libeel_plugboard(enigma, 6).l) ||
        (libeel_plugboard(enigma, 5).r > 0 && libeel_plugboard(enigma, 5).r == libeel_plugboard(enigma, 6).r)) {
        return 0;
    }

    return 1;
}
