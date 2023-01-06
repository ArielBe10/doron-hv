#!/bin/bash

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"/..
MULTIBOOT_FILE="${PROJECT_DIR}/build/hypervisor.so"


verify_multiboot () {
    if grub-file --is-x86-multiboot2 "$MULTIBOOT_FILE"; then
        echo multiboot2 confirmed
    else
        echo the file is not multiboot2
    fi
}

make -C "$PROJECT_DIR"
verify_multiboot
