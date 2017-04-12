#ifndef DEFINE_H
#define DEFINE_H

#if 1
#define DebugOut(s) printf("%s\n", s)
#define DebugOut2(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define DebugOut(s)
#define DebugOut2(fmt, ...)
#endif

#define Output(s) fprintf(outfile, "%s\n", s)
#define Output2(s, x) fprintf(outfile, s, x)


typedef struct Node
{
	char op[255];
	char l[255];
	char r[255];
	int regi;
}Node;

#endif
