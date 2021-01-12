#!/bin/sh

if [ $QEMUBIN == ""]; then
    QEMUBIN=qemu
fi
#QEMUBIN=./seg-patched-qemu

$QEMUBIN -hda ci_hdd.img -boot c -m 256 $ADDARGS $@
#$QEMUBIN -m 32 -boot n -tftp /var/www/tftpboot -bootp /pxelinux.0 $ADDARGS $@

# -serial pty
