#!/bin/bash

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"/..

IMAGE="${PROJECT_DIR}/build/hypervisor.iso"
OS_IMAGE="${PROJECT_DIR}/windows/Win10_22H2_English_x64.iso"
LOGFILE="${PROJECT_DIR}/hypervisor.log"
DISK="${PROJECT_DIR}/windows/win10.img"


qemu-system-x86_64 \
        -enable-kvm -cpu host -smp cores=4 \
        -m 3G \
        -monitor vc \
        -name "Windows" \
        -boot d \
        -chardev stdio,id=char0,logfile="$LOGFILE" \
        -serial chardev:char0 \
        -drive file="$DISK" \
        -drive file="$IMAGE",media=cdrom \
        -no-reboot 
        # -drive file="$OS_IMAGE",media=cdrom,index=2
        # -net nic -net user,hostname=windowsvm \
