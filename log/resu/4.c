#include <stdarg.h>
#include <stdio.h>

#define CPPAPI_PRINTFLIKE(fmt, arg) __attribute__((format(printf, fmt, arg)))

int IsDebugTagSet(const char * v)
{
    return 1;
}

void Debug(const char *tag, const char *fmt, ...)
{
    if (IsDebugTagSet(tag)) {
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
}

int main1()
{
    Debug("tag","this is %d\n", 99);
    return 0;
}

void Error(const char *fmt, ...) CPPAPI_PRINTFLIKE(1, 2);

extern void myprint(const char *format,...) __attribute__((format(printf,1,2)));

int main()
{
    CPPAPI_PRINTFLIKE("this is", 2);
    //Error("this is %d\n", 99);
    //myprint("this is %d\n", 99);
    return 0;
}
