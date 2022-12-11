#pragma once

#define LOG(log_level, ...) log_printf((log_level), __LINE__, __FILE__, __VA_ARGS__)
#define DEBUG(...) LOG(DEBUG_LEVEL, __VA_ARGS__)
#define INFO(...) LOG(INFO_LEVEL, __VA_ARGS__)
#define WARNING(...) LOG(WARNING_LEVEL, __VA_ARGS__)
#define ERROR(...) LOG(ERROR_LEVEL, __VA_ARGS__)
#define PANIC(...)                     \
    do                                 \
    {                                  \
        LOG(PANIC_LEVEL, __VA_ARGS__); \
        __asm__("cli; hlt");           \
    } while (0)

typedef enum
{
    DEBUG_LEVEL = 0,
    INFO_LEVEL = 1,
    WARNING_LEVEL = 2,
    ERROR_LEVEL = 3,
    PANIC_LEVEL = 4
} log_level_t;

void init_logging(log_level_t log_level);
void set_log_level(log_level_t log_level);
log_level_t get_log_level(void);
void log_printf(log_level_t level, int line_no, const char *filename, const char *fmt, ...);