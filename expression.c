#include <stdio.h>
#include <string.h>
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


char expr_stack[100][255];
int p_stack = -1;
Node nodes[100];
int node_count = 0;

void init_nodes(void){
		p_stack = -1;
		memset(expr_stack, 0, sizeof(char) * 100 * 255);

		// push $
		p_stack++;
		strcpy(expr_stack[p_stack], "$");
}

void push_word(void){
		DebugOut("push begin");
		char* s = nodes[node_count].l;
		DebugOut2("\t\t\t%d %s\n", strlen(s), s);
		if(strlen(s) == 0)
		{
				node_count++;
				strcpy(nodes[node_count].l, tok.charvalue);
				DebugOut2("Push Left here:%d\n", node_count);
		}
		else
		{
				strcpy(nodes[node_count].r, tok.charvalue);
				DebugOut2("Push Right here:%d\n", node_count);
		}
		DebugOut("push end");
}

void flush_node(void){
		DebugOut("\tflush begin"); 
		int i;
		for(i = 0; i <= node_count; ++i){
				DebugOut2("\t\top %d, left %s, right %s, register %d\n",
				nodes[i].op, nodes[i].l, nodes[i].r, nodes[i].regi);
				memset(&nodes[i], 0, sizeof(Node));
		}
		node_count--; // temp
		DebugOut("\tflush end"); 
}

enum Rank
{
	RankGreater, // a >> b
	RankLess,	// a << b
	RankEqual   // a = b
};

int f(char* ch){
	if(strcmp(ch, "+") == 0)	return 2;
	else if(strcmp(ch, "-")==0)	return 2;
	else if(strcmp(ch, "*")==0)	return 4;
	else if(strcmp(ch,"div")==0)return 4;
	else if(strcmp(ch, "!")==0)	return 6;
	else if(strcmp(ch, "(")==0)	return 0;
	else if(strcmp(ch, ")")==0)	return 11;
	else if(strcmp(ch, "i")==0)	return 11;
	else if(strcmp(ch, "$")==0)	return 0;
    else fprintf(stderr, "illegal operator\n"), exit(1);
}

int g(char* ch){
	if(strcmp(ch, "+") == 0)	return 1;
	else if(strcmp(ch, "-")==0)	return 1;
	else if(strcmp(ch, "*")==0)	return 3;
	else if(strcmp(ch,"div")==0)return 3;
	else if(strcmp(ch, "!")==0)	return 15;
	else if(strcmp(ch, "(")==0)	return 10;
	else if(strcmp(ch, ")")==0)	return 0;
	else if(strcmp(ch, "i")==0)	return 10;
	else if(strcmp(ch, "$")==0)	return 0;
    else fprintf(stderr, "illegal operator\n"), exit(1);
}

int ranking(int f, int g){
	if(f > g) 			return RankGreater;
	else if(f == g) 	return RankEqual;
	else /*if(f < g)*/	return RankLess;
}

void term(void);
void factor(void);

void expression(void){
		DebugOut("expr begin");
		do{
				getsym();
				if((tok.attr == NUMBER) ||
					(tok.attr == IDENTIFIER) // guaranteed id is only var
				//		fprintf(stderr, "undeclared var\n");
					){
					DebugOut2("\t %d %d %s\n", tok.attr, tok.value, tok.charvalue);

					// push

				}
				else if(tok.attr == RWORD && tok.value == DIV){
				}
				else if(tok.attr == SYMBOL){
						switch(tok.value){
								case PLUS:
								case MINUS:
								case TIMES:
								case LPAREN:
								case RPAREN:
								break;
								default:
									goto EXIT;
						}
					DebugOut2("\t %d %d %s\n", tok.attr, tok.value, tok.charvalue);
				}
				else{
						break;
				}
		}while(1);
EXIT:
		//flush();
		DebugOut("expr end");
}

void expression2(void){
  do{
    getsym();
    term();

	if(tok.attr == SYMBOL && tok.value == PLUS){
			DebugOut("PLUS!");
						nodes[node_count].op = PLUS;
	}
	else if(tok.attr == SYMBOL && tok.value == MINUS){
			DebugOut("MINUS!");
						nodes[node_count].op = MINUS;
	}
	else{
			if(tok.attr == SYMBOL && tok.value == RPAREN) //temp
			{}//temp
			else//temp
		  flush_node();
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
						DebugOut("div come");
						nodes[node_count].op = DIV;

						getsym();
				}
				else if(tok.attr == SYMBOL&& tok.value == TIMES) {
						//dosomething

						DebugOut("times come");
						nodes[node_count].op = TIMES;
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
//		  nodes[node_count].l[0] = '-';
  }


  if(tok.attr == IDENTIFIER){
		  push_word();
		  DebugOut2("%s\n", tok.charvalue);
  }
  else if(tok.attr == NUMBER){
		  push_word();
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
