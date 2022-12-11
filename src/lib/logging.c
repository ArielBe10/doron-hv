#include "lib/logging.h"

#include "lib/string.h"
#include "lib/print.h"
#include "drivers/serial.h"

static log_level_t min_level;

static char *log_level_str[5] = {
    [DEBUG_LEVEL] = "DEBUG",
    [INFO_LEVEL] = "INFO",
    [WARNING_LEVEL] = "WARNING",
    [ERROR_LEVEL] = "ERROR",
    [PANIC_LEVEL] = "PANIC"};

static void print_serial(const char *string);

void init_logging(log_level_t log_level)
{
    min_level = log_level;
    init_serial();
}

void set_log_level(log_level_t log_level)
{
    min_level = log_level;
}

log_level_t get_log_level(void)
{
    return min_level;
}

void print_serial(const char *string)
{
    for (; *string != '\0'; string++)
        write_serial(*string);
}

void log_printf(log_level_t level, int line_no, const char *filename, const char *fmt, ...)
{
    if (level >= min_level)
    {
        char msg[0x100];
        char buffer[0x100];

        va_list argp;
        va_start(argp, fmt);
        vsprintf(msg, fmt, argp);
        va_end(argp);

        sprintf(buffer, "[%s] %s:%d: %s", log_level_str[level], filename, line_no, msg);
        print_serial(buffer);
        write_serial('\n');
    }
}
