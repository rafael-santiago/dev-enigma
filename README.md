# ![/dev/enigma](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/logo.png)

This is a way to put an ``Enigma Machine`` under your ``Linux /dev`` sub-directory.
Yes, it is about a ``Linux char device`` which emulates an ``Enigma``.

Until now I have written it to run on ``2.6.x kernels``.

## How to build it?

You need [Hefesto](https://github.com/rafael-santiago/hefesto) in order to abstract a bunch
of complications that you can get during this process.

Once ``Hefesto`` well-installed and working, you need to "teach" ``Hefesto`` how to build a
LKM. In order to do it you need to install a "knowledge base" related with this task on the
following way:

```
tux@sché:~/Bletchley# git clone https://github.com/rafael-santiago/helios helios
tux@sché:~/Bletchley# cd helios
tux@sché:~/Bletchley/helios# hefesto --install=lnx-toolsets
```

After executing the incantations above your ``Hefesto`` copy will start to know how to build
``Linux modules``. By the way, you can remove your ``Helios`` local directory, if you want to.

Now, to build this ``device`` you should jump to its ``src`` sub-directory and:

```
tux@sché:~/Bletchley/dev-enigma/src# hefesto
```

After the build process you will get the module ``enigma.ko``. Now you should use
``insmod``/``rmmod`` in order to get some fun with. When inserted, this module automatically
creates a char device named as ``/dev/enigma`` onto your box.

## How to use my /dev/enigma?

The usage of this device is pretty straightforward. You should:

1. ``Open``.
2. Set the Enigma configurations through an ``i/o control``.
3. ``Write``.
4. ``Read``.

Follows all it in plain *C*:

```c
#include <enigmactl.h>
#include <eel.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    int fd;
    libeel_enigma_ctx enigma_conf;
    char buf[255];
    size_t buf_size;
    int exit_code = 0;

    fd = open("/dev/enigma", O_RDWR);

    if (fd < 0) {
        printf("ERROR: during open() call.\n");
        exit_code = 1;
        goto ___go_home;
    }

    /*Now let's to configure our enigma device*/

    memset(&enigma_conf, 0, sizeof(enigma_conf));

    enigma_conf.left_rotor = i;
    enigma_conf.middle_rotor = ii;
    enigma_conf.right_rotor = iv;

    libeel_rotor_at(&enigma_conf, l) = 'B';
    libeel_rotor_at(&enigma_conf, m) = 'L';
    libeel_rotor_at(&enigma_conf, r) = 'K';

    enigma_conf.reflector = c;

    libeel_ring(&enigma_conf, l) = 1;
    libeel_ring(&enigma_conf, m) = 1;
    libeel_ring(&enigma_conf, r) = 1;

    libeel_plugboard(&enigma_conf, 1).l = 'H';
    libeel_plugboard(&enigma_conf, 1).r = 'C';

    libeel_plugboard(&enigma_conf, 2).l = 'V';
    libeel_plugboard(&enigma_conf, 2).r = 'Q';

    if (ioctl(fd, ENIGMA_SET, &enigma_conf) != 0) {
        printf("ERROR: during ioctl() call.\n");
        exit_code = 1;
        goto ___go_home;
    }

    memset(buf, 0, sizeof(buf));

    sprintf(buf, "In the first few seconds an aching sadness wrenched "
                 "his heart, but it soon gave way to a feeling of sweet "
                 "disquiet, the excitement of gypsy wanderlust");
    buf_size = strlen(buf);

    printf("PLAINTEXT: %s\n\n", buf);

    if (write(fd, buf, buf_size) != buf_size) {
        printf("ERROR: during write() call.\n");
        exit_code = 1;
        goto ___go_home;
    }

    if (read(fd, buf, buf_size) != buf_size) {
        printf("ERROR: during read() call.\n");
        exit_code = 1;
        goto ___go_home;
    }

    printf("CRYPTOGRAM: %s\n\n", buf);

    if (ioctl(fd, ENIGMA_RESET) != 0) {
        printf("ERROR: during ioctl() call.\n");
        exit_code = 1;
        goto ___go_home;
    }

    if (write(fd, buf, buf_size) != buf_size) {
        printf("ERROR: during write() call.\n");
        exit_code = 1;
        goto ___go_home;
    }

    if (read(fd, buf, buf_size) != buf_size) {
        printf("ERROR: during read() call.\n");
        exit_code = 1;
        goto ___go_home;
    }

    printf("PLAINTEXT: %s\n\nThat's all folks!!\n", buf);

___go_home:
    close(fd);

    return exit_code;
}
```

This ``device driver`` uses another [library](https://github.com/rafael-santiago/eel) of mine related with the ``Enigma``.
Here basically you will use its conveniences in order to setup your "kenigma" ;)

You should learn how to configure the machine elements using [eel](https://github.com/rafael-santiago/eel) by reading
its ``README``. I will abstract these details here. However it is pretty simple.

So, you need two "alien" ``includes`` to compile this sample:

- eel.h
- enigmactl.h

The ``Libeel`` follows embedded into ``/dev/enigma`` source code and all stuff of it is located under ``src/eel``.

The ``enigmactl.h`` gathers the ``i/o control commands`` definitions and it is located under ``src/fops_impl``.

Now, supposing that you have cloned the ``/dev/enigma`` to ``~/src/dev-enigma`` and saved the sample shown above
under ``~/src/enigma.c``. To compile it you should proceed as follows:

```
tux@sché:~/src# gcc enigma.c -oenigma -Idev-enigma/src/fops_impl -Idev-enigma/src/eel
```

When running this sample you should get something like:

```
tux@sché:~/src# insmod dev-enigma/src/enigma.ko
tux@sché:~/src# ./enigma
PLAINTEXT: In the first few seconds an aching sadness wrenched his heart, but it soon gave way to a feeling
of sweet disquiet, the excitement of gypsy wanderlust

CRYPTOGRAM: Ye bfj uzjro eby ccinjuw ru lvvqbm ncnzurd ugbqkxgb cfp pmncr, awd oy dfbm klpm qnj ze p lbbmuod
je epubl skccvcpp, laa nvxmbndyql yd lznyn gjgqdmuptz

PLAINTEXT: In the first few seconds an aching sadness wrenched his heart, but it soon gave way to a feeling
of sweet disquiet, the excitement of gypsy wanderlust

That's all folks!!
tux@sché:~/src# _
```

The presented sample introduces an ``i/o`` control that is useful in cases in that you want to return your
``Enigma`` to its initial state. There you have used it in order to decrypt the previous encrypted buffer.
So, as you know the ``Enigma`` is about a reciprocal cipher. Like ``albam``, ``atbash`` or even our ultra
modern over used ``xor``.

Of course that this sample is pretty dull because it is kind of statical. You should to implement an application
which receives the enigma settings by command line and also the data buffer. Well, the device is done now this
kind of code is up to you.

## How to install it?

(not yet, coming soon!)
