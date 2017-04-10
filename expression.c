#include <stdio.h>
#include <stdlib.h>
#include <getsym.h>

#include "define.h"

extern TOKEN tok;

int f(int ch){
  switch(ch){
    case PLUS:    return 2;
    case MINUS:   return 2;
    case TIMES:   return 4;
    case DIV:     return 4;
    case '!' :    return 6; // ! は単項演算子の-
    case LPAREN:  return 0;
    case RPAREN:  return 11;
    case 'i':     return 11;
    case '$':     return 0;
    default:      fprintf(stderr, "illegal operator\n"), exit(1);
  }
}

int g(int ch){
  switch(ch){
    case PLUS:    return 1;
    case MINUS:   return 1;
    case TIMES:   return 3;
    case DIV:     return 3;
    case '!' :    return 15; // ! は単項演算子の-
    case LPAREN:  return 10;
    case RPAREN:  return 0;
    case 'i':     return 10;
    case '$':     return 0;
    default:      fprintf(stderr, "illegal operator\n"), exit(1);
  }
}

void term(void);
void factor(void);

void expression(void){
  fprintf(stderr, "hoge\n");

  do{
    getsym();
    term();
  }while(tok.attr == SYMBOL && (tok.value == PLUS || tok.value == MINUS));
  DebugOut("expr end");
}

void term(void){

  do{
    DebugOut2("value is %d\n", tok.value);
    factor();
    getsym();
  }while( (tok.attr == RWORD && tok.value == DIV) || 
          (tok.attr == SYMBOL&& tok.value == TIMES) );
}

void factor(void){
  // 単項演算子-の時
  if(tok.attr == SYMBOL && tok.value == MINUS){
  }

  getsym();

  switch(tok.attr){
    case IDENTIFIER:
      break;
    case NUMBER:
      break;
    case SYMBOL:
      if(tok.value != LPAREN)
        fprintf(stderr, "illegal bunpou\n"), exit(1);

      getsym();
      expression();

      getsym();
      if(tok.value != RPAREN)
        fprintf(stderr, "illegal bunpou\n"), exit(1);

      break;
  }

  getsym();
}
