#!/bin/bash

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"/..

IMAGE="${PROJECT_DIR}/build/hypervisor.iso"
HARD_DISK="${PROJECT_DIR}/build/harddisk.hd"
LOGFILE="${PROJECT_DIR}/hypervisor.log"

qemu-system-x86_64 \
        -drive file="$IMAGE",index=1,media=cdrom \
        -drive file="$HARD_DISK",index=0,media=disk \
        -chardev stdio,id=char0,logfile="$LOGFILE",signal=off \
        -serial chardev:char0 \
        -no-reboot