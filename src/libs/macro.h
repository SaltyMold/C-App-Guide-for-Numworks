#ifndef MACRO_H
#define MACRO_H

#include <stddef.h>
#include <stdint.h>

#if defined(__has_include)
#  if __has_include(<inttypes.h>)
#    include <inttypes.h>
#    define FMT_U64 "%" PRIu64
#    define FMT_U32 "%" PRIu32
#  else
#    define FMT_U64 "%llu"
#    define FMT_U32 "%lu"
#  endif
#else
#  include <inttypes.h>
#  define FMT_U64 "%" PRIu64
#  define FMT_U32 "%" PRIu32
#endif

/* Boolean to string */
#define BOOL_STR(b) ((b) ? "true" : "false")

static inline void u64_to_str(unsigned long long v, char *buf, size_t bufsize) {
    if (bufsize == 0) return;
    char tmp[32];
    int i = 0;
    if (v == 0) tmp[i++] = '0';
    while (v != 0 && i < (int)sizeof(tmp)) {
        tmp[i++] = '0' + (v % 10);
        v /= 10;
    }
    int copy = i;
    if (copy > (int)bufsize - 1) copy = (int)bufsize - 1;
    for (int j = 0; j < copy; ++j) {
        buf[j] = tmp[copy - 1 - j];
    }
    buf[copy] = '\0';
}

static inline void u32_to_str(unsigned long v, char *buf, size_t bufsize) {
    u64_to_str((unsigned long long)v, buf, bufsize);
}

static inline void s64_to_str(long long v, char *buf, size_t bufsize) {
    if (bufsize == 0) return;
    if (v < 0) {
        if (bufsize > 1) {
            buf[0] = '-';
            u64_to_str((unsigned long long)(-v), buf+1, bufsize>1?bufsize-1:0);
        } else {
            buf[0] = '\0';
        }
    } else {
        u64_to_str((unsigned long long)v, buf, bufsize);
    }
}

static inline void s32_to_str(long v, char *buf, size_t bufsize) {
    s64_to_str((long long)v, buf, bufsize);
}

/* Minimal hex converter for pointers */
static inline void ptr_to_hex(void *p, char *buf, size_t bufsize) {
    if (bufsize == 0) return;
    uintptr_t v = (uintptr_t)p;
    char tmp[2 + sizeof(uintptr_t) * 2 + 1];
    int pos = 0;
    tmp[pos++] = '0'; tmp[pos++] = 'x';
    int started = 0;
    for (int i = (int)(sizeof(uintptr_t)*2 - 1); i >= 0; --i) {
        int nib = (v >> (i*4)) & 0xF;
        char c = (nib < 10) ? ('0' + nib) : ('a' + nib - 10);
        if (c != '0' || started || i == 0) { started = 1; tmp[pos++] = c; }
    }
    tmp[pos] = '\0';
    /* copy */
    size_t copy = (size_t)pos;
    if (copy >= bufsize) copy = bufsize - 1;
    for (size_t i = 0; i < copy; ++i) buf[i] = tmp[i];
    buf[copy] = '\0';
}

/* When building for the device (calculator) some printf length modifiers
 * like %llu may not be supported. We provide a small snprintf wrapper that
 * handles the common specifiers used in this codebase and falls back to a
 * safe behaviour. The wrapper is only enabled when not building for the
 * simulator (SIMULATOR_HOST).
 */
#ifndef SIMULATOR_HOST
#include <stdarg.h>
#include <string.h>

static int my_vsnprintf(char *str, size_t size, const char *fmt, va_list ap) {
    char *out = str;
    size_t left = size ? size - 1 : 0; /* leave room for NUL */
    const char *p = fmt;
    int written = 0;
    while (*p) {
        if (*p != '%') {
            if (left) { *out++ = *p; --left; }
            ++written; ++p; continue;
        }
        ++p; /* skip % */
        if (*p == '%') { if (left) { *out++ = '%'; --left; } ++written; ++p; continue; }

        /* parse length modifiers */
        int ll = 0, l = 0;
        if (p[0] == 'l' && p[1] == 'l') { ll = 1; p += 2; }
        else if (p[0] == 'l') { l = 1; p += 1; }

        char spec = *p++;
        char tmp[128]; tmp[0] = '\0';
        switch (spec) {
            case 'u':
                if (ll) { unsigned long long v = va_arg(ap, unsigned long long); u64_to_str(v, tmp, sizeof tmp); }
                else if (l) { unsigned long v = va_arg(ap, unsigned long); u32_to_str((unsigned long)v, tmp, sizeof tmp); }
                else { unsigned v = va_arg(ap, unsigned); u32_to_str((unsigned long)v, tmp, sizeof tmp); }
                break;
            case 'd': case 'i':
                if (ll) { long long v = va_arg(ap, long long); s64_to_str(v, tmp, sizeof tmp); }
                else if (l) { long v = va_arg(ap, long); s32_to_str(v, tmp, sizeof tmp); }
                else { int v = va_arg(ap, int); s32_to_str(v, tmp, sizeof tmp); }
                break;
            case 's': {
                const char *s = va_arg(ap, const char*);
                if (!s) s = "(null)";
                size_t sl = strlen(s);
                size_t tocopy = sl;
                if (tocopy > left) tocopy = left;
                for (size_t i = 0; i < tocopy; ++i) { *out++ = s[i]; }
                written += (int)sl;
                left = (left > tocopy) ? left - tocopy : 0;
                break; }
            case 'p': {
                void *pv = va_arg(ap, void*);
                ptr_to_hex(pv, tmp, sizeof tmp);
                break; }
            case 'c': {
                int ch = va_arg(ap, int);
                if (left) { *out++ = (char)ch; --left; }
                ++written;
                break; }
            default:
                /* unsupported specifier: copy it verbatim */
                if (left) { *out++ = '%'; --left; }
                ++written;
                if (left) { *out++ = spec; --left; }
                ++written;
                break;
        }
        /* if tmp was filled, append it */
        if (tmp[0]) {
            size_t tl = strlen(tmp);
            size_t tocopy = tl;
            if (tocopy > left) tocopy = left;
            for (size_t i = 0; i < tocopy; ++i) *out++ = tmp[i];
            written += (int)tl;
            left = (left > tocopy) ? left - tocopy : 0;
        }
    }
    if (size) *out = '\0';
    return written;
}

static int my_snprintf(char *str, size_t size, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int r = my_vsnprintf(str, size, fmt, ap);
    va_end(ap);
    return r;
}

/* Override snprintf for device builds so existing code doesn't need changes. */
#define snprintf(...) my_snprintf(__VA_ARGS__)
#endif

#endif
