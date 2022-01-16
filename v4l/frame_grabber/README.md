# Frame grabber

From SO answer: https://stackoverflow.com/questions/278112/webcam-library-for-c-on-linux

v4l2 official examples

What you get:

./v4l2grab: capture a few snapshots to files outNNN.ppm
./v4l2gl: show video live on a window using an OpenGL texture (immediate rendering, hey!) and raw X11 windowing (plus GLUT's gluLookAt for good measure).
How to get it on Ubuntu 16.04:

sudo apt-get install libv4l-dev
sudo apt-get build-dep libv4l-dev
git clone git://linuxtv.org/v4l-utils.git
cd v4l-utils
# Matching the installed version of dpkg -s libv4l-dev
git checkout v4l-utils-1.10.0
./bootstrap.sh
./configure
make
# TODO: fails halfway, but it does not matter for us now.
cd contrib/tests
make
It is also easy to use those examples outside of the Git tree, just copy them out, make relative includes "" absolute <>, and remove config.h. I've done that for you at: https://github.com/cirosantilli/cpp-cheat/tree/09fe73d248f7da2e9c9f3eff2520a143c259f4a6/v4l2

Minimal example from docs

The docs 4.9.0 contain what appears to be a minimal version of ./v4l2grab at https://linuxtv.org/downloads/v4l-dvb-apis-new/uapi/v4l/v4l2grab-example.html. I needed to patch it minimally and I've sent the patch to http://www.spinics.net/lists/linux-media/ (their docs live in the Linux kernel tree as rst, neat), where it was dully ignored.

Usage:

gcc v4l2grab.c -lv4l2


## Get the list of supported sizes and formats

```
v4l2-ctl -d0 --list-formats-ext

```
