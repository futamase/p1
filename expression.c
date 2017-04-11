#include <stdio.h>
#include <stdlib.h>
#include <getsym.h>

#include "define.h"

#define OutValue printf("the value is %d\n", tok.value)

extern TOKEN tok;

typedef struct Node
{
	int op;
	char l[255];
	char r[255];
	int regi;
}Node;


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

	if(tok.attr == SYMBOL && tok.value == PLUS){
			DebugOut("PLUS!");
	}
	else if(tok.attr == SYMBOL && tok.value == MINUS){
			DebugOut("MINUS!");
	}
	else{
			break;
	}
  }while(1);
  DebugOut("expr end");
}

void term(void){

		do{
				DebugOut2("value is %d\n", tok.value);
				factor();
				getsym();
				if(tok.attr == RWORD && tok.value == DIV){
						//dosomething

						getsym();
				}
				else if(tok.attr == SYMBOL&& tok.value == TIMES) {
						//dosomething

						DebugOut("times come");
						getsym();
				}
				else{
						break;
				}
		}while(1);
}

void factor(void){
		  DebugOut2("factor begin and value is %s\n", tok.charvalue);
	

  // 単項演算子-の時
  if(tok.attr == SYMBOL && tok.value == MINUS){
		  getsym();
  }


  if(tok.attr == IDENTIFIER){
		  DebugOut2("%s\n", tok.charvalue);
  }
  else if(tok.attr == NUMBER){
		  DebugOut2("number come and the value is %d\n", tok.value);
  }
  else if(tok.attr == SYMBOL && tok.value == LPAREN){
//      getsym();
      expression();

      if(tok.value != RPAREN)
        fprintf(stderr, "illegal bunpou\n"), exit(1);
  }

//  getsym();
}
