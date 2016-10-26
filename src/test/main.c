/*
 *                              Copyright (C) 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <cutest.h>
#include <eel.h>
#include <enigmactl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#define BREATH_INSECS 0  //  WARN(Santiago): If you are experiencing some LKM loading/unload issues try to increase it.

static int devfd;

static int load_module(void);

static int unload_module(void);

static int load_module(void) {
    int exit_code = 1;

#if defined(__linux__)
    exit_code = system("insmod ../enigma.ko");
#elif defined(__FreeBSD__)
    exit_code = system("kldload ../enigma.ko");
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

CUTE_TEST_CASE(open_tests)
    int exit_code;
    int fd;

    fd = open("/dev/enigma", O_RDWR);

    CUTE_ASSERT(fd != -1);

    close(fd);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(dev_ctls_tests)
    int fd;
    libeel_enigma_ctx daily_setting;
    int result = 0;

    fd = open("/dev/enigma", O_RDWR);

    CUTE_ASSERT(fd >= 0);

    CUTE_ASSERT(ioctl(fd, ENIGMA_RESET) != 0);

    sleep(BREATH_INSECS);

    CUTE_ASSERT(ioctl(fd, ENIGMA_SET, NULL) != 0);

    sleep(BREATH_INSECS);

    CUTE_ASSERT(ioctl(fd, ENIGMA_UNSET_DEFAULT_SETTING) == 0);

    sleep(BREATH_INSECS);

    CUTE_ASSERT(ioctl(fd, ENIGMA_SET_DEFAULT_SETTING, NULL) != 0);

    daily_setting.left_rotor = i;
    daily_setting.middle_rotor = iv;
    daily_setting.right_rotor = i;

    daily_setting.reflector = b;

    libeel_ring(&daily_setting, l) = 1;
    libeel_ring(&daily_setting, m) = 5;
    libeel_ring(&daily_setting, r) = 23;

    libeel_rotor_at(&daily_setting, l) = 'L';
    libeel_rotor_at(&daily_setting, m) = 'N';
    libeel_rotor_at(&daily_setting, r) = 'X';

    libeel_plugboard(&daily_setting, 1).l = 'A';
    libeel_plugboard(&daily_setting, 1).r = 'M';
    libeel_plugboard(&daily_setting, 2).l = 'B';
    libeel_plugboard(&daily_setting, 2).r = 'O';
    libeel_plugboard(&daily_setting, 3).l = 'C';
    libeel_plugboard(&daily_setting, 3).r = 'P';
    libeel_plugboard(&daily_setting, 4).l = 'D';
    libeel_plugboard(&daily_setting, 4).r = 'Q';
    libeel_plugboard(&daily_setting, 5).l = 'E';
    libeel_plugboard(&daily_setting, 5).r = 'R';
    libeel_plugboard(&daily_setting, 6).l = 'F';
    libeel_plugboard(&daily_setting, 6).r = 'S';

    CUTE_ASSERT(ioctl(fd, ENIGMA_SET, &daily_setting) != 0);

    CUTE_ASSERT(ioctl(fd, ENIGMA_SET_DEFAULT_SETTING, &daily_setting) != 0);

    daily_setting.right_rotor = vii;

    CUTE_ASSERT(ioctl(fd, ENIGMA_SET, &daily_setting) == 0);

    sleep(BREATH_INSECS);

    CUTE_ASSERT(ioctl(fd, ENIGMA_SET_DEFAULT_SETTING, &daily_setting) == 0);

    sleep(BREATH_INSECS);

    result = ioctl(fd, ENIGMA_RESET);

    if (result != 0) {
        perror("ioctl");
        printf("result = %d\n", result);
        sleep(2);
        result = ioctl(fd, ENIGMA_RESET);
    }

    CUTE_ASSERT(result == 0);

    sleep(BREATH_INSECS);

    CUTE_ASSERT(ioctl(fd, ENIGMA_RESET) == 0);

    sleep(BREATH_INSECS);

    CUTE_ASSERT(ioctl(fd, ENIGMA_UNSET_DEFAULT_SETTING) == 0);

    sleep(BREATH_INSECS);

    close(fd);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(devio_open_tests)
    devfd = open("/dev/enigma", O_RDWR);
    CUTE_ASSERT(devfd > -1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(devio_ioctlset_tests)
    libeel_enigma_ctx dev_enigma;

    memset(&dev_enigma, 0, sizeof(dev_enigma));

    dev_enigma.left_rotor = i;
    dev_enigma.middle_rotor = ii;
    dev_enigma.right_rotor = iii;

    libeel_ring(&dev_enigma, l) = 1;
    libeel_ring(&dev_enigma, m) = 1;
    libeel_ring(&dev_enigma, r) = 1;

    dev_enigma.reflector = b;

    libeel_rotor_at(&dev_enigma, l) = 'A';
    libeel_rotor_at(&dev_enigma, m) = 'A';
    libeel_rotor_at(&dev_enigma, r) = 'Z';

    CUTE_ASSERT(ioctl(devfd, ENIGMA_SET, &dev_enigma) == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(devio_ioctldefaultset_tests)
    libeel_enigma_ctx dev_enigma;
    int devfd;

    CUTE_ASSERT((devfd = open("/dev/enigma", O_RDWR)) > -1);

    memset(&dev_enigma, 0, sizeof(dev_enigma));

    dev_enigma.left_rotor = i;
    dev_enigma.middle_rotor = ii;
    dev_enigma.right_rotor = iii;

    libeel_ring(&dev_enigma, l) = 1;
    libeel_ring(&dev_enigma, m) = 1;
    libeel_ring(&dev_enigma, r) = 1;

    dev_enigma.reflector = b;

    libeel_rotor_at(&dev_enigma, l) = 'A';
    libeel_rotor_at(&dev_enigma, m) = 'A';
    libeel_rotor_at(&dev_enigma, r) = 'Z';

    CUTE_ASSERT(ioctl(devfd, ENIGMA_SET_DEFAULT_SETTING, &dev_enigma) == 0);

    sleep(BREATH_INSECS);

    CUTE_ASSERT(close(devfd) == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(devio_write_tests)
    const char *plaintext = "GGGGGGGGGG";
    size_t plaintext_size = strlen(plaintext);
    CUTE_ASSERT(write(devfd, plaintext, plaintext_size) == plaintext_size);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(devio_read_tests)
    char ciphertext[11];
    size_t ciphertext_size = 10;
    memset(ciphertext, 0, sizeof(ciphertext));
    CUTE_ASSERT(read(devfd, ciphertext, ciphertext_size) == ciphertext_size);
    CUTE_ASSERT(strcmp(ciphertext, "PXWIAFZZLY") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(devio_ioctlreset_tests)
    CUTE_ASSERT(ioctl(devfd, ENIGMA_RESET) == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(devio_write_inv_tests)
    const char *plaintext = "PXWIAFZZLY";
    size_t plaintext_size = strlen(plaintext);
    CUTE_ASSERT(write(devfd, plaintext, plaintext_size) == plaintext_size);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(devio_read_inv_tests)
    char ciphertext[11];
    size_t ciphertext_size = 10;
    memset(ciphertext, 0, sizeof(ciphertext));
    CUTE_ASSERT(read(devfd, ciphertext, ciphertext_size) == ciphertext_size);
    CUTE_ASSERT(strcmp(ciphertext, "GGGGGGGGGG") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(devio_close_tests)
    CUTE_ASSERT(close(devfd) == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(usage_lines_tests)
    const int usage_lines_nr = 10;
    int fd[usage_lines_nr];
    int f;

    for (f = 0; f < usage_lines_nr; f++) {
        fd[f] = open("/dev/enigma", O_RDWR);
        CUTE_ASSERT(fd[f] > -1);
    }

    f = open("/dev/enigma", O_RDWR);
    CUTE_ASSERT(f != 0);

    CUTE_ASSERT(close(fd[0]) == 0);

    f = open("/dev/enigma", O_RDWR);

    CUTE_ASSERT(f > -1);

    CUTE_ASSERT(close(f) == 0);

    for (f = 1; f < usage_lines_nr; f++) {
        CUTE_ASSERT(close(fd[f]) == 0);
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE_SUITE(device_poking_tests)
    CUTE_RUN_TEST(devio_open_tests);
    CUTE_RUN_TEST(devio_ioctlset_tests);
    CUTE_RUN_TEST(devio_write_tests);
    CUTE_RUN_TEST(devio_read_tests);
    CUTE_RUN_TEST(devio_ioctlreset_tests);
    CUTE_RUN_TEST(devio_write_inv_tests);
    CUTE_RUN_TEST(devio_read_inv_tests);
    CUTE_RUN_TEST(devio_close_tests);

    CUTE_RUN_TEST(devio_ioctldefaultset_tests);

    //  INFO(Santiago): We are expecting the same result
    //                  even without a regular ioctl(ENIGMA_SET).
    CUTE_RUN_TEST(devio_open_tests);
    CUTE_RUN_TEST(devio_write_tests);
    CUTE_RUN_TEST(devio_read_tests);
    CUTE_RUN_TEST(devio_ioctlreset_tests);
    CUTE_RUN_TEST(devio_write_inv_tests);
    CUTE_RUN_TEST(devio_read_inv_tests);
    CUTE_RUN_TEST(devio_close_tests);
CUTE_TEST_CASE_SUITE_END

CUTE_TEST_CASE(device_tests)
    int exit_code = 1;

    printf("\n\n\033[1m\033[31m"
           "*** Hello Human, step back, I will test the \"enigma.ko\" within 5 secs.\n"
           "    In case of any explosion, I strongly advise you to restart this computer. ***\033[m\n\n");

    sleep(5);

    unload_module();

    sleep(BREATH_INSECS);

    exit_code = load_module();

    sleep(BREATH_INSECS);

    CUTE_ASSERT(exit_code == 0);

    CUTE_RUN_TEST(open_tests);
    CUTE_RUN_TEST(dev_ctls_tests);
    CUTE_RUN_TEST(usage_lines_tests);
    CUTE_RUN_TEST_SUITE(device_poking_tests);

    sleep(BREATH_INSECS);

    exit_code = unload_module();

    CUTE_ASSERT(exit_code == 0);
CUTE_TEST_CASE_END

CUTE_MAIN(device_tests)
