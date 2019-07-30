/**
 *  @file logging
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@varnsen)
 *  @created      Monday Jul 29, 2019 17:57:18 MDT
 *  @bug No Known Bugs
 */

#ifndef LOGGING_HPP

#define LOGGING_HPP

#include <stdarg.h>
#include <stdio.h>

#define LOG_VERSION "0.1.0"

typedef void (*log_LockFn)(void *udata, int lock);

enum { LLOG_TRACE, LLOG_DEBUG, LLOG_INFO, LLOG_WARN, LLOG_ERROR, LLOG_FATAL };

#define log_trace(...) log_log(LLOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) log_log(LLOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) log_log(LLOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void log_set_udata(void *udata);
void log_set_lock(log_LockFn fn);
void log_set_fp(FILE *fp);
void log_set_level(int level);
void log_set_quiet(int enable);

void log_log(int level, const char *file, int line, const char *fmt, ...);

static struct {
  void *udata;
  log_LockFn lock;
  FILE *fp;
  int level;
  int quiet;
} L;

static const char *level_names[] = {"TRACE", "DEBUG", "INFO",
                                    "WARN",  "ERROR", "FATAL"};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {"\x1b[94m", "\x1b[36m", "\x1b[32m",
                                     "\x1b[33m", "\x1b[31m", "\x1b[35m"};
#endif

static void lock(void) {
  if (L.lock) {
    L.lock(L.udata, 1);
  }
}

static void unlock(void) {
  if (L.lock) {
    L.lock(L.udata, 0);
  }
}

void log_set_udata(void *udata) { L.udata = udata; }

void log_set_lock(log_LockFn fn) { L.lock = fn; }

void log_set_fp(FILE *fp) { L.fp = fp; }

void log_set_level(int level) { L.level = level; }

void log_set_quiet(int enable) { L.quiet = enable ? 1 : 0; }

void log_log(int level, const char *file, int line, const char *fmt, ...) {
  if (level < L.level) {
    return;
  }

  /* Acquire lock */
  lock();

  /* Get current time */
  time_t t = time(NULL);
  struct tm *lt = localtime(&t);

  /* Log to stderr */
  if (!L.quiet) {
    va_list args;
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
#ifdef LOG_USE_COLOR
    fprintf(stderr, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ", buf,
            level_colors[level], level_names[level], file, line);
#else
    fprintf(stderr, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
#endif
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
  }

  /* Log to file */
  if (L.fp) {
    va_list args;
    char buf[32];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
    fprintf(L.fp, "%s %-5s %s:%d: ", buf, level_names[level], file, line);
    va_start(args, fmt);
    vfprintf(L.fp, fmt, args);
    va_end(args);
    fprintf(L.fp, "\n");
    fflush(L.fp);
  }

  /* Release lock */
  unlock();
}

static inline void LOG_TRACE(const std::string &message) {
  log_trace(message.c_str());
}

static inline void LOG_DEBUG(const std::string &message) {
  log_debug(message.c_str());
}

static inline void LOG_INFO(const std::string &message) {
  log_info(message.c_str());
}

static inline void LOG_WARN(const std::string &message) {
  log_warn(message.c_str());
}

static inline void LOG_ERROR(const std::string &message) {
  log_error(message.c_str());
}

static inline void LOG_FATAL(const std::string &message) {
  log_fatal(message.c_str());
}

#endif /* end of include guard LOGGING_HPP */
