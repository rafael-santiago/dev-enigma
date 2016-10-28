# ![/dev/enigma](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/logo.png)

This is a way to put an ``Enigma Machine`` under your ``/dev`` sub-directory. Yes, it is about a
``char device`` which emulates an ``Enigma``.

This device was written to run on more than one platform. Well, I think that the **Figure 1**
clarifies any question about supported platforms.

**Figure 1**: A lousy but effective joke.

![/dev/enigma/compat](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/dev-enigma-compat-explained.png)

Originally I have written it to run on ``2.6.x`` or higher ``Linux`` kernel versions. Maybe with a
little change on the ``dev_ioctl()`` prototype you can be able to build it on ``2.4.x`` kernels
but I am not so sure about.

Now with ``FreeBSD`` I have not tested it on older versions than ``11.0``. However, I guess that it
should work fine. The ``KLD`` infrastructure is pretty stable.

**Warning**: Do not use it to protect any sensible data. The ``Enigma`` was cracked during ``WWII``. So, it is just for fun.

## How to clone it?

The easiest way is:

```
tux@sché:~/Bletchley# git clone https://github.com/rafael-santiago/dev-enigma dev-enigma --recursive
```

## How to build it?

You need [Hefesto](https://github.com/rafael-santiago/hefesto) in order to abstract a bunch of complications
that you can get during this process.

Once ``Hefesto`` well-installed and working, you need to "teach" ``Hefesto`` how to build a ``LKM``. In order to
do it you need to install a "knowledge base" related with this task on the following way:

```
tux@sché:~/Bletchley# git clone https://github.com/rafael-santiago/helios helios
tux@sché:~/Bletchley# cd helios
tux@sché:~/Bletchley/helios# hefesto --install=<module-toolset-name>
```

If you are on ``Linux`` the ``<module-toolset-name>`` should be ``lnx-module-toolset``. If you are on ``FreeBSD``
the ``<module-toolset-name>`` should be ``freebsd-module-toolset``.

**Of course that you need to have the kernel sources into your environment.**

After executing the incantations above your ``Hefesto`` copy will start to know how to build a ``LKM`` for your
current platform. By the way, you can remove your ``Helios`` local directory, if you want to.

Now, to build this ``device`` you should jump to its ``src`` sub-directory and:

```
beastie@sché:~/Bletchley/dev-enigma/src# hefesto
```

I think that use Humans as space monkeys to test any kind of code is bad, harmful and shameful. Due to it, some
tests are executed over the generated module in some point during the build process, then I find that you should
consider the **Figure 2**. Yes, your "build user" must be allowed to load/unload ``LKMs``. Before building this
module watch for your permissions.

**Figure 2**: "Vandalism: as beautiful as stone in a ``Mainframe's`` face"... For the sake of your uptime...

![disaster-girl-remarks](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/disaster-girl-build-remarks.jpg)

After the build process you will get the module ``enigma.ko``. Now you should load it into your kernel in order
to get some fun. When inserted, this module automatically creates a char device named as ``/dev/enigma`` onto your box:

- ``Linux`` users: ``insmod/rmmod`` does the basic job.

- ``FreeBSD`` users: ``kldload/kldunload`` does the basic job.

## How to install it?

In order to install it you need to be under the ``src`` sub-directory and so:

```
tux@sché:~/Bletchley/dev-enigma/src# hefesto --install
```

For uninstalling issues, it is similar, look:

```
beastie@sché:~/Bletchley/dev-enigma/src# hefesto --uninstall
```

## How to use my /dev/enigma?

The usage of this device is pretty straightforward. You should:

1. ``Open``.
2. Set the Enigma configurations through an ``i/o control``.
3. ``Write``.
4. ``Read``.
5. ``Close`` (just because you are polite).

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
    if (fd > 0) {
        close(fd);
    }

    return exit_code;
}
```

This ``device driver`` uses another [library](https://github.com/rafael-santiago/eel) of mine related with the ``Enigma``.
Here, basically you will use its conveniences in order to setup your "k-enigma" ;)

You should learn how to configure the machine elements using [eel](https://github.com/rafael-santiago/eel) by reading
its ``README``. I will abstract these details here. However, it is pretty simple.

So, you need two "alien" ``includes`` to compile this sample:

- ``eel.h``
- ``enigmactl.h``

The ``Libeel`` follows embedded into ``/dev/enigma`` source code and all stuff of it is located under ``src/eel``.

The ``enigmactl.h`` gathers the ``i/o control commands`` definitions and it is located under ``src/enigmactl``.

Now, supposing that you have cloned the ``/dev/enigma`` to ``~/src/dev-enigma`` and saved the sample shown above
into ``~/src/enigma.c``. To compile it you should proceed as follows:

```
tux@sché:~/src# gcc enigma.c -oenigma -Idev-enigma/src/enigmactl -Idev-enigma/src/eel
```

If you have installed this device driver using its build scripts you can indicate only one include directory
which would be ``/usr/local/share/dev-enigma/include``.

When running this sample you should get something like:

```
beastie@sché:~/src# kldload dev-enigma/src/enigma.ko
beastie@sché:~/src# ./enigma
PLAINTEXT: In the first few seconds an aching sadness wrenched his heart, but it soon gave way to a feeling
of sweet disquiet, the excitement of gypsy wanderlust

CRYPTOGRAM: Ye bfj uzjro eby ccinjuw ru lvvqbm ncnzurd ugbqkxgb cfp pmncr, awd oy dfbm klpm qnj ze p lbbmuod
je epubl skccvcpp, laa nvxmbndyql yd lznyn gjgqdmuptz

PLAINTEXT: In the first few seconds an aching sadness wrenched his heart, but it soon gave way to a feeling
of sweet disquiet, the excitement of gypsy wanderlust

That's all folks!!
beastie@sché:~/src# _
```

The presented sample introduces an ``i/o`` control that is useful in cases in that you want to return your
``Enigma`` to its initial state. There we have used it in order to decrypt the previous encrypted buffer.
So, as you know the ``Enigma`` is about a reciprocal cipher. Like ``albam``, ``atbash`` or even our ultra
modern over used ``xor``.

Then, when you need to set up your ``Enigma`` you should use:

```c
libeel_enigma_ctx enigma_conf;
(...)
ioctl(dev, ENIGMA_SET, &enigma_conf);
```

Once the settings accepted, anytime you can revert the device to its initial state, just by:

```c
ioctl(dev, ENIGMA_RESET);
```

You can also set up a default setting for any file descriptor opened:

```c
libeel_enigma_ctx enigma_default_conf;
(...)
ioctl(dev, ENIGMA_SET_DEFAULT_SETTING, &enigma_default_conf);
```

If you want to unset the default setting you should:

```c
ioctl(dev, ENIGMA_UNSET_DEFAULT_SETTING);
```

Of course that this sample is pretty dull because it is kind of static. You should implement an application
which receives the ``Enigma machine`` settings by command line and also the data buffer. Well, the device is done
now this kind of code is up to you.

### What are usage lines?

Usage lines is the way of dividing the ``Enigma device`` present in your system among several users. Several but not
inifite, in fact, I have fixed the number of usage lines to ``10``. By default, it is possible to have ten users
simultaneously hung in this device. On normal conditions is expected that none of them step on the foot of each other.

For this reason it makes the usage of this device a little bit tough. Thus if you try something like:

```
tux@sché:~/src# echo "foobar" > /dev/enigma
tux@sché:~/src# cat /dev/enigma
```

Nothing will occur due to two reasons:

1. You can not configure the Enigma setting with native shell commands, so the ``echo`` command will open and immediately try to use a non configurated machine.
2. Even if the write operation succeeds, the ``echo`` command will close its file descriptor. When closing it all buffers related with this usage line will be freed.

There is a way to beat these complications. Just follow reading...

### The enigmactl application

This application is installed together with the device driver. Basically it is a nice application used to define
a default configuration for any usage line acquired.

The basic usage for it is as follows:

```
beastie@sché:~/src# enigmactl --set --l-rotor=i --m-rotor=ii --r-rotor=v \
> --l-rotor-at=Y --m-rotor-at=Y --r-rotor-at=Z \
> --reflector=b \
> --l-ring=2 --m-ring=3 --r-ring=9 \
> --plugboard=R/U,S/H
```

The option:

- ``--l-rotor`` defines the rotor used on the left (from ``i`` to ``viii``).
- ``--m-rotor`` defines the rotor used at the middle (from ``i`` to ``viii``).
- ``--r-rotor`` defines the rotor used on the right  (from ``i`` to ``viii``).
- ``--l-rotor-at`` defines the initial left rotor position (from ``A`` to ``Z``).
- ``--m-rotor-at`` defines the initial middle rotor position (from ``A`` to ``Z``).
- ``--r-rotor-at`` defines the initial right rotor position (from ``A`` to ``Z``).
- ``--reflector`` defines the chosen reflector (can be ``B`` or ``C``).
- ``--l-ring`` defines the chosen left ring position (from ``1`` to ``26``). It is optional, the default is ``1``.
- ``--m-ring`` defines the chosen middle ring position (from ``1`` to ``26``). It is optional, the default is ``1``.
- ``--r-ring`` defines the chosen right ring position (from ``1`` to ``26``). It is optional, the default is ``1``.
- ``--plugboard`` defines the swaps on the machine plugboard. (in the form of ``s1/s1',...,s10/s10'``). It is optional, the default is ``no swaps``.

In order to clear a previous defined default setting you should use the ``--unset`` command as follows:

```
tux@sché:~/src# enigmactl --unset
```

You can also use the shell script ``enigmactl.sh``. This script allows you to set up the default configuration on a more cooked way:

![Sample](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/enigmactl-sh-sample.gif)

This shell script is installed with the device driver and it is based on ``bash``. This remark can be useless for ``Linuxers`` anyway
it can be valuable for ``FreeBSD`` users. You do not need to worry about changing the shell or indicating the ``bash's`` home. The sheebang
is configurated when installing the device into your environment. The only thing that you need to do is to install the ``bash`` if you
do not have it or to do little changes into ``enigmactl.sh`` to be executed natively by yours shell of choice.

### Using the device directly from shell

As said in a previous [section](#what-are-usage-lines) to use ``/dev/enigma`` directly from shell is not so trivial. I
will show you a simple ``shell script`` which uses the ``enigmactl.sh`` for setting issues and so uses the device driver
to perform some encryption/decryption.

Firstly, here goes the code and after I will comment what I judge relevant:

```bash
#!/bin/bash

# File name: 'enigma.sh'

if [ -f /usr/local/bin/enigmactl.sh ] ; then
    dialog --yesno "Do you want setup /dev/enigma?" 0 0

    if [ $? -eq 0 ]; then
        /usr/local/bin/enigmactl.sh
        if [ $? -ne 0 ]; then
            exit $?
        fi
    fi
fi

exec 3<>/dev/enigma

if [ $? -ne 0 ]; then
    echo "error: when trying to open /dev/enigma."
    exit $0
fi

text=$1

if [ -z $text ]; then
    text="$(dialog --stdout --inputbox "Type the text to be written to /dev/enigma" 0 0)"
fi

if [ -z $text ]; then
    echo ""
    echo "INFO: aborted by the user."
    exit 1
fi

echo $text >&3

echo ""
cat <&3

exec 3>&-
```

Well, the script uses file descriptors techniques because the usage line must be hold by the user until the read operation
happen, otherwise the written data will be lost. The script expects to receive a text to be processed as an argument, if it
is not supplied, a dialog asking for this data will be shown. Thus, to use it you should proceed as follows:

```
tux@sché:~/src# ./enigma.sh "once you are lost in twillight's blue \
> you do not find the way, the way finds you."
vcsi qpe qki yuqz ba uzwbpxuin'i ysom odv nj bzf toor zgi ffx, jra unh acjpn ptq.
tux@shcé:~/src# _
```

### Best practices

This is just a ``toy device``, so I have used a bunch of things that is not so advised for being used in real-world devices.
Let's take for example the ``Linux`` implementation... The usage of ``kmalloc``. Among other issues, it imposes a limit of ``128kb``
(more or less) to the passed data buffers. Do not try to pass huge buffers to your ``Enigma device``. Still, do not write for a long
time without performing any read. These things will not put fire on your ``kernel`` but will cause malfunction in your ``char device``.

That's it! Have fun!
