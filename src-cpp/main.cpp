#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <getsym.h>
#include "define.h"
#include "output.h"

TOKEN tok;
FILE *infile, *outfile;

void compiler(void);

int main(int argc, char*argv[])
{
  char outfile_name[255];
  fprintf(stderr, "Simple compiler: compile start.\n");

  if(argc < 2){
    fprintf(stderr, "USAGE: comp source-file-name\n");
    exit(1);
  }
  if((infile = fopen(argv[1], "r")) == NULL){
    fprintf(stderr, "Source file %s can't open.\n", argv[1]);
    exit(1);
  }

  if(argc == 3)
    strcpy(outfile_name, argv[2]);
  else
    strcpy(outfile_name, "a.asm");

  if((outfile = fopen(outfile_name, "w")) == NULL){
    fprintf(stderr, "Object file creation failed.\n");
    exit(1);
  }

  compiler();

  output_all(outfile);

  fclose(infile);
  fclose(outfile);
  exit(0);
}
