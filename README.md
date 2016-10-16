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
tux@sché:~/Bletchley/helios# cd helios
tux@sché:~/Bletchley# hefesto --install=lnx-toolsets
```

After executing the commands above your ``Hefesto`` copy will start to know how to build
``Linux modules``. If you want to you can remove your ``Helios`` local directory.

Now, to build this ``device`` you should jump to its ``src`` sub-directory and:

```
tux@sché:~/Bletchley/dev-enigma/src# hefesto
```

After the build process you will get the module ``enigma.ko``. Now you should use
``insmod``/``rmmod`` in order to get some fun with. When inserted, this module automatically
creates a char device named as ``/dev/enigma`` onto your box.

## How to use my /dev/enigma?

(not yet, comming soon!)

## How to install it?

(not yet, comming soon!)
