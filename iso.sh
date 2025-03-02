#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/slate.kernel isodir/boot/slate.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "slate" {
	multiboot /boot/slate.kernel
}
EOF
grub-mkrescue -o slate.iso isodir
