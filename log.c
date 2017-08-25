/*

log utility structures and routines
Copyright (C) 2013-2017 xinoir <xinoir@theta.black>
This file is part of beta.

*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include "beta/log.h"

#define LOG_MESSAGE_MAX_SIZE 256

enum {
    LOG_OPTION = LOG_CONS | LOG_NDELAY | LOG_NOWAIT | LOG_PID,
    LOG_FACILITY = LOG_LOCAL0,
    LOG_MASK = LOG_DEBUG
};

typedef enum {
    ERROR_UNKNOWN,
    ERROR_LINE_TRUNCATED
} error_index_t;

struct error {
    const char *message;
    const int number;
    const int flag;
};

static const struct error errors[] = {
    [ERROR_UNKNOWN] = { "unknown error", 4000, 0 },
    [ERROR_LINE_TRUNCATED] = { "line truncated", 4001, 0 }
};

static int make_log_message(char *buffer, size_t size, const char *format, va_list va, int error)
{
    int status;
    va_list empty_va;
    const char *message = NULL;
    const struct error line_truncated_error = errors[ERROR_LINE_TRUNCATED];

    if (!va)
        va = empty_va;

    if (error)
        message = strerror(error);

    memset(buffer, 0, size);
    
    if ((status = vsnprintf(buffer, size, format, va)) < 0) {
        syslog(LOG_CRIT, "%m");
        return 1;
    } else if ((size_t)status >= size)
        syslog(LOG_WARNING, "%s: [%d]", line_truncated_error.message, line_truncated_error.number);

    if (message) {
        if ((status = snprintf(buffer, size, "%s, %s: [%d]", buffer, message, error)) < 0) {
            syslog(LOG_CRIT, "%s: [%d]", strerror(errno), errno);
            return 1;
        } else if ((size_t)status >= size)
            syslog(LOG_WARNING, "%s: [%d]", line_truncated_error.message, line_truncated_error.number);
    }

    return 0;
}

static void trace(const char *format, ...)
{
    va_list va;
    char buffer[LOG_MESSAGE_MAX_SIZE];

    va_start(va, format);
    if (!make_log_message(buffer, sizeof(buffer), format, va, 0))
        syslog(LOG_INFO, "%s", buffer);
    va_end(va);
}

void log_debug(const char *format, va_list va)
{
    const int status = errno;
    char buffer[LOG_MESSAGE_MAX_SIZE];

    if (!make_log_message(buffer, sizeof(buffer), format, va, 0))
        syslog(LOG_DEBUG, "%s", buffer);

    errno = status;
}

void log_trace(const char *format, va_list va)
{
    const int status = errno;
    char buffer[LOG_MESSAGE_MAX_SIZE];

    if (!make_log_message(buffer, sizeof(buffer), format, va, 0))
        syslog(LOG_INFO, "%s", buffer);

    errno = status;
}

void log_warning(const char *format, va_list va)
{
    const int status = errno;
    char buffer[LOG_MESSAGE_MAX_SIZE];

    if (!make_log_message(buffer, sizeof(buffer), format, va, 0))
        syslog(LOG_WARNING, "%s", buffer);

    errno = status;
}

void log_error(const char *format, va_list va, int error)
{
    const int status = errno;
    char buffer[LOG_MESSAGE_MAX_SIZE];

    if (!make_log_message(buffer, sizeof(buffer), format, va, error))
        syslog(LOG_ERR, "%s", buffer);

    errno = status;
}

void log_fatal(const char *format, va_list va, int error)
{
    const int status = errno;
    char buffer[LOG_MESSAGE_MAX_SIZE];

    if (!make_log_message(buffer, sizeof(buffer), format, va, error))
        syslog(LOG_EMERG, "%s", buffer);

    errno = status;
}

void log_deinit(const char *module)
{
    const int status = errno;

    closelog();
    trace("deinited log for [%s]", module);

    errno = status;
}

void log_init(const char *module)
{
    const int status = errno;

    openlog(module, LOG_OPTION, LOG_FACILITY);
    setlogmask(LOG_UPTO(LOG_MASK));
    trace("inited log for [%s]", module);

    errno = status;
}

