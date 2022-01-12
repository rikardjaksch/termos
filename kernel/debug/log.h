#pragma once

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
} debug_log_type_e;

#define klog_debug(...) klog_log(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define klog_info(...) klog_log(LOG_LEVEL_INFO, __VA_ARGS__)
#define klog_warning(...) klog_log(LOG_LEVEL_WARNING, __VA_ARGS__)
#define klog_error(...) klog_log(LOG_LEVEL_ERROR, __VA_ARGS__)
#define klog_fatal(...) klog_log(LOG_LEVEL_FATAL, __VA_ARGS__)

void klog_log(debug_log_type_e level, const char* fmt, ...);