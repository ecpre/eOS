#!/bin/sh

set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/eOS.kernel isodir/boot/eOS.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "eOS" {
	multiboot /boot/eOS.kernel
}
EOF
grub-mkrescue -o eOS.iso isodir
