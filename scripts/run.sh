#!/bin/bash

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"/..

IMAGE="${PROJECT_DIR}/build/hypervisor.iso"
LOGFILE="${PROJECT_DIR}/hypervisor.log"

qemu-system-x86_64 -cdrom "$IMAGE" \
        -chardev stdio,id=char0,logfile="$LOGFILE",signal=off \
        -serial chardev:char0 \
        -no-reboot