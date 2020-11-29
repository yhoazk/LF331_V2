# Simple build


For ubuntu:

```
sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib build-essential

sudo apt-get install chrpath socat cpio python python3 python3-pip python3-pexpect

sudo apt-get install xz-utils debianutils iputils-ping libsdl1.2-dev xterm
```

clone poky from git:


mkdir yocto
cd yocto
git clone -b git://git.yoctoproject.org/poky
source poky/oe-init-build-env


Build the vase image core-image-minimal, which is hte minimal rootfs <10Mb

yocto/build/$ bitbake core-image-minimal


outputs the image 
yocto/build$ cd tmp/deploy/images

clone poky from git:


mkdir yocto
cd yocto
git clone -b git://git.yoctoproject.org/poky
source poky/oe-init-build-env


Build the vase image core-image-minimal, which is hte minimal rootfs <10Mb

yocto/build/$ bitbake core-image-minimal


outputs the image 
yocto/build$ cd tmp/deploy/images
