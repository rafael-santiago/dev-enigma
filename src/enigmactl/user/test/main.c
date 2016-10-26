/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include "../option.h"
#include "../setloader.h"
#include <cutest.h>
#include <eel.h>
#include <stdlib.h>
#include <string.h>

static int load_module(void);

static int unload_module(void);


static int load_module(void) {
    int exit_code = 1;

#if defined(__linux__)
    exit_code = system("insmod ../../../enigma.ko");
#elif defined(__FreeBSD__)
    exit_code = system("kldload ../../../enigma.ko");
#endif

    return exit_code;
}

static int unload_module(void) {
    int exit_code = 1;

#if defined(__linux__)
    exit_code = system("rmmod enigma");
#elif defined(__FreeBSD__)
    exit_code = system("kldunload enigma.ko");
#endif

    return exit_code;
}


CUTE_TEST_CASE(enigmactl_option_tests)
    char *argv[] = {
        "--foo=bar",
        "--dummy-flag"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    char *buf = NULL;

    CUTE_ASSERT(get_option("boo", NULL) == NULL);

    CUTE_ASSERT(get_bool_option("boo", 1) == 1);

    set_argc_argv(argc, argv);

    buf = get_option("foo", NULL);

    CUTE_ASSERT(buf != NULL);

    CUTE_ASSERT(strcmp(buf, "bar") == 0);

    CUTE_ASSERT(get_bool_option("dummy-flag", 0) == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(enigmactl_setloader_tests)
printf("booooo!\n");
    //  INFO(Santiago): Bad constructions.

    char *argv_0[] = {
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_1[] = {
        "--l-rotor=i",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_2[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_3[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_4[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_5[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--reflector=c"
    };

    char *argv_6[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z"
    };

    char *argv_7[] = {
        "--l-rotor=0",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_8[] = {
        "--l-rotor=i",
        "--r-rotor=b",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_9[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=3",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_10[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=#",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_11[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=?",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_12[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=8",
        "--reflector=c"
    };

    char *argv_13[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=x"
    };

    char *argv_14[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1a"
    };

    char *argv_15[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=b"
    };

    char *argv_16[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=three"
    };

    char *argv_17[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u"
    };

    char *argv_18[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,"
    };

    char *argv_19[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/"
    };

    char *argv_20[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/p,s/d,f/g,h/j,k/l,z/x,c"
    };

    char *argv_21[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/p,s/d,f/g,h/j,k/l,z/x,c/"
    };

    char *argv_22[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/p,s/d,f/g,h/j,k/l,z/x,c/m"
    };

    //  INFO(Santiago): From now on, all ok.

    char *argv_23[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c"
    };

    char *argv_24[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1"
    };

    char *argv_25[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2"
    };

    char *argv_26[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3"
    };

    char *argv_27[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w"
    };

    char *argv_28[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r"
    };

    char *argv_29[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y"
    };

    char *argv_30[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i"
    };

    char *argv_31[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/p"
    };

    char *argv_32[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/p,s/d"
    };

    char *argv_33[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/p,s/d,f/g"
    };

    char *argv_34[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/p,s/d,f/g,h/j"
    };

    char *argv_35[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/p,s/d,f/g,h/j,k/l"
    };

    char *argv_36[] = {
        "--l-rotor=i",
        "--r-rotor=ii",
        "--m-rotor=iii",
        "--l-rotor-at=a",
        "--r-rotor-at=a",
        "--m-rotor-at=z",
        "--reflector=c",
        "--l-ring=1",
        "--r-ring=2",
        "--m-ring=3",
        "--plugboard=q/w,e/r,t/y,u/i,o/p,s/d,f/g,h/j,k/l,z/x"
    };

    struct test_vector_ctx {
        char **argv;
        int argc;
        int ok;
    };

#define add_test_step(n, o) { argv_ ## n, sizeof(argv_ ## n) / sizeof(argv_ ## n[0]), o }

    struct test_vector_ctx test_vector[] = {
        add_test_step( 0, 0), add_test_step( 1, 0), add_test_step( 2, 0), add_test_step( 3, 0), add_test_step( 4, 0),
        add_test_step( 5, 0), add_test_step( 6, 0), add_test_step( 7, 0), add_test_step( 8, 0), add_test_step( 9, 0),
        add_test_step(10, 0), add_test_step(11, 0), add_test_step(12, 0), add_test_step(13, 0), add_test_step(14, 0),
        add_test_step(15, 0), add_test_step(16, 0), add_test_step(17, 0), add_test_step(18, 0), add_test_step(19, 0),
        add_test_step(20, 0), add_test_step(21, 0), add_test_step(22, 0), add_test_step(23, 1), add_test_step(24, 1),
        add_test_step(25, 1), add_test_step(26, 1), add_test_step(27, 1), add_test_step(28, 1), add_test_step(29, 1),
        add_test_step(30, 1), add_test_step(31, 1), add_test_step(32, 1), add_test_step(33, 1), add_test_step(34, 1),
        add_test_step(35, 1), add_test_step(36, 1)
    };

#undef add_test_step

    const size_t test_vector_nr = sizeof(test_vector) / sizeof(test_vector[0]);

    size_t t = 0;

    libeel_enigma_ctx *setting = NULL;

    set_argc_argv(0, NULL);
    setting = ld_enigma_setting();

    CUTE_ASSERT(setting == NULL);

    while (t < test_vector_nr) {
        set_argc_argv(test_vector[t].argc, test_vector[t].argv);

        setting = ld_enigma_setting();

        if (test_vector[t].ok) {
            CUTE_ASSERT(setting != NULL);
            libeel_del_enigma_ctx(setting);
            setting = NULL;
        } else {
            CUTE_ASSERT(setting == NULL);
        }

        t++;
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(enigmactl_trinket_poking_tests)
    const char *binpath = "../bin/enigmactl";
    char cmd[255] = "";

    unload_module();

    sleep(1);

    //  INFO(Santiago): no module, no args.
    CUTE_ASSERT(system(binpath) == 0);

    sprintf(cmd, "%s --unset", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    CUTE_ASSERT(load_module() == 0);

    sleep(1);

    //  INFO(Santiago): with module, no args.
    CUTE_ASSERT(system(binpath) == 0);

    sprintf(cmd, "%s --help", binpath);
    CUTE_ASSERT(system(cmd) == 0);

    sprintf(cmd, "%s --unset", binpath);
    CUTE_ASSERT(system(cmd) == 0);

    sprintf(cmd, "%s --set", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=b", binpath);
    CUTE_ASSERT(system(cmd) == 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=bx", binpath);
    CUTE_ASSERT(system(cmd) == 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=d", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=7 --reflector=b", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=3 --r-rotor-at=z --reflector=b", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=. --m-rotor-at=a --r-rotor-at=z --reflector=b", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iiia --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=b", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=2 --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=b", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=1 --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=b", binpath);
    CUTE_ASSERT(system(cmd) != 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=b --l-ring=2", binpath);
    CUTE_ASSERT(system(cmd) == 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=b --m-ring=3", binpath);
    CUTE_ASSERT(system(cmd) == 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=b --r-ring=5", binpath);
    CUTE_ASSERT(system(cmd) == 0);

    sprintf(cmd, "%s --set --l-rotor=i --m-rotor=ii --r-rotor=iii --l-rotor-at=a --m-rotor-at=a --r-rotor-at=z --reflector=c --plugboard=a/b,c/d,e/f,x/g", binpath);
    CUTE_ASSERT(system(cmd) == 0);

    sprintf(cmd, "%s --unset", binpath);
    CUTE_ASSERT(system(cmd) == 0);

    sleep(1);

    CUTE_ASSERT(unload_module() == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(enigmactl_tests)
    CUTE_RUN_TEST(enigmactl_option_tests);
    CUTE_RUN_TEST(enigmactl_setloader_tests);
    CUTE_RUN_TEST(enigmactl_trinket_poking_tests);
CUTE_TEST_CASE_END

CUTE_MAIN(enigmactl_tests);
