#pragma once

#include <stdint.h>


extern void __syscall(uint64_t syscall_number);
extern void __sysenter(uint64_t syscall_number);


#define SSDT_LENGTH 512  // 1 page

typedef void (*syscall_handler_t)(void);

typedef struct {
    syscall_handler_t entries[SSDT_LENGTH];
} __attribute__((aligned(0x1000))) ssdt_t;


char pad[0x1000];
ssdt_t ssdt;

uint64_t sysenter_rsp;


void setup_syscall_handler(uint64_t syscall_number, syscall_handler_t handler);
void syscall_handler(uint64_t syscall_number);
void init_ssdt(void);
