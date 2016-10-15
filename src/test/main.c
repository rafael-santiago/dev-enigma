#include <cutest.h>
#include <eel.h>
#include <stdlib.h>
#include <fcntl.h>

CUTE_TEST_CASE(lkm_insertion_tests)
    int exit_code;
    int fd;

    system("rmmod enigma 2>&1 > /dev/null");

    exit_code = system("insmod ../enigma.ko");

    CUTE_ASSERT(exit_code == 0);

    fd = open("/dev/enigma", O_RDWR);

    CUTE_ASSERT(fd != -1);

    close(fd);

    exit_code = system("rmmod enigma");

    CUTE_ASSERT(exit_code == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(device_tests)
    CUTE_RUN_TEST(lkm_insertion_tests);
CUTE_TEST_CASE_END

CUTE_MAIN(device_tests)
