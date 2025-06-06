#ifndef STRING_H
#define STRING_H

#ifndef DOOM64
#define memset __memset
#endif

__attribute__((used))
static inline void*
memset(void* dest, int ch, size_t count) {
    uint8_t* d = (uint8_t*) dest;

    size_t i = 0;

    for (; i < count; ++i) {
        d[i] = ch;
    }

    return dest;
}

#define strlen __strlen
static inline size_t
strlen(const char* s) {
    const char* _s = s;
    while(*s++);
    return s - _s;
}

#define strncpy __strncpy
static inline void
strncpy(char* dest, const char* src, size_t l) {
    for (size_t i = 0; i < l; i++) {
        dest[i] = src[i];
        if (!src[i])
            return;
    }
}

#define strcmp __strcmp
static inline int
strcmp(const char* lhs, const char* rhs) {
    for(;;) {
        char l = *lhs++;
        char r = *rhs++;
        
        if (l != r)
            return 1;

        if (!l)
            return 0;
    }
    return 0;
}

#define strncmp __strncmp
static inline int
strncmp(const char* lhs, const char* rhs, size_t s) {
    for(size_t i = 0; i < s; i++) {
        char l = *lhs++;
        char r = *rhs++;
        
        if (l != r)
            return 1;

        if (!l)
            return 0;
    }
    return 0;
}

#define strncasecmp __strncasecmp
static inline int
strncasecmp(const char* lhs, const char* rhs, size_t s) {
    for(size_t i = 0; i < s; i++) {
        char l = toupper(*lhs++);
        char r = toupper(*rhs++);
        
        if (l != r)
            return 1;

        if (!l)
            return 0;
    }
    return 0;
}

#endif
