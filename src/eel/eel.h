/*
 *                        Copyright (C) 2006, 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef LIBEEL_EEL_H
#define LIBEEL_EEL_H 1

#include "types.h"

#define libeel_rotor(e, p) ( (e)->gears_## p ## r )

#define libeel_rotor_inv(e, p) ( (e)->gears_## p ## r_1 )

#define libeel_notch(e, p) ( (e)->gears_ ## p ## n )

#define libeel_display(e, p) ( (e)->gears_## p ## d )

#define libeel_keyboard(e) ( (e)->keyboard )

#define libeel_reflector(e) ( (e)->gears_## rf )

#define libeel_rotor_offset(e, p) ( (e)->gears_ ## p ## _off )

#define libeel_rotor_at(e, p) ( (e)->gears_ ## p ## _at )

#define libeel_ring(e, p) ( (e)->gears_ ## p ## _ring )

#define libeel_plugboard(e, n) ( (e)->plugboard.plug ## n )

#define libeel_enigma_input(e) ( (e)->l_in )

#define libeel_enigma_output(e) ( (e)->l_out )

#ifdef __cplusplus

extern "C" {

#endif  // #ifdef __cplusplus

int libeel_init_machine(libeel_enigma_ctx *enigma);

char libeel_type(libeel_enigma_ctx *enigma);

void libeel_del_enigma_ctx(libeel_enigma_ctx *enigma);

libeel_enigma_ctx *libeel_new_enigma_ctx(void);

#ifdef __cplusplus

}

#endif  // #ifdef __cplusplus

#endif
