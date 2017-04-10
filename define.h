#ifndef DEFINE_H
#define DEFINE_H

#if 1
#define DebugOut(s) printf("%s\n", s)
#define DebugOut2(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define DebugOut(s)
#define DebugOut2(fmt, ...)
#endif

#define Output(s) fprintf(outfile, s)
#define Output2(s, x) fprintf(outfile, s, x)


#endif
