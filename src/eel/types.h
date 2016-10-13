/*
 *                        Copyright (C) 2006, 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef LIBEEL_TYPES_H
#define LIBEEL_TYPES_H 1

#ifdef __cplusplus

extern "C" {

#endif  // #ifdef __cplusplus

typedef enum _libeel_rotor_t {
    i = 0, ii, iii, iv, v, vi, vii, viii
}libeel_rotor_t;

typedef enum _libeel_reflector_t {
    b = 0, c
}libeel_reflector_t;

struct libeel_plug_setting_ctx {
    char l, r;
};

typedef struct _libeel_plugboard_ctx {
    struct libeel_plug_setting_ctx plug1, plug2, plug3, plug4, plug5, plug6;
}libeel_plugboard_ctx;

typedef char libeel_rotor_wiring_t[26];

typedef char libeel_notch_t;

typedef char libeel_rotor_display_t;

typedef int libeel_ring_t;

typedef struct _libeel_enigma_ctx {
    libeel_rotor_t left_rotor, middle_rotor, right_rotor;
    libeel_reflector_t reflector;
    libeel_rotor_wiring_t keyboard;
    libeel_plugboard_ctx plugboard;
    libeel_rotor_wiring_t gears_lr, gears_mr, gears_rr;
    libeel_rotor_wiring_t gears_lr_1, gears_mr_1, gears_rr_1;
    libeel_notch_t gears_mn, gears_rn;
    libeel_rotor_display_t gears_ld, gears_md, gears_rd;
    libeel_rotor_wiring_t gears_rf;
    libeel_rotor_wiring_t gears_l_off, gears_m_off, gears_r_off;
    libeel_rotor_display_t gears_l_at, gears_m_at, gears_r_at;
    libeel_ring_t gears_l_ring, gears_m_ring, gears_r_ring;
    libeel_rotor_display_t l_in, l_out;
}libeel_enigma_ctx;

#ifdef __cplusplus

}

#endif  //  #ifdef __cplusplus

#endif
