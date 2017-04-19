#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "define.h"
#include "sym_table.h"

#define OutValue printf("the value is %d\n", tok.value)

extern TOKEN tok;
extern int var_count;

#define MAX_CHAR 25

char expr_stack[100][MAX_CHAR];
int p_stack = -1;
Node nodes[100];
SymData regi_var[100];
int regi_var_count = 0;

int node_count = -1;
int g_register = 0;

void init_nodes(void){
    g_register = 0;
    node_count = -1;
	regi_var_count = 0;

		p_stack = -1;
		memset(expr_stack, 0, sizeof(char) * 100 * MAX_CHAR);
    memset(nodes, 0, sizeof(Node) * 100);

		// push $
		p_stack++;
		strcpy(expr_stack[p_stack], "$");
}

static void push(char* s){
  DebugOut2("push %s\n", s);
  p_stack++;
  strcpy(expr_stack[p_stack], s);
}
static void pop(){
  DebugOut("pop");
  memset(expr_stack[p_stack], 0, sizeof(char) * MAX_CHAR);
  p_stack--;
}

static void erase(int index){
  int i;
  DebugOut("erase");
  for(i = 0; i < p_stack - index; i++){
    memcpy(expr_stack[index+i], expr_stack[index+i+1], sizeof(char) * MAX_CHAR);
  }
  memset(expr_stack[p_stack], 0, sizeof(char) * MAX_CHAR);
  p_stack--;

  DebugOut("-----------------------------this is after erase------------------------------------");
    for(i = p_stack; i >= 0; i--)
      DebugOut2("\t\tstack No.%d is : %s\n", i, expr_stack[i]);
  DebugOut("-----------------------------this is after erase------------------------------------");
}

// index番地とindex+1番地を入れ替える
// a_i == a_j の時の(がきた時に使用する
static void swap(int index){
  DebugOut("SWAPPING!");
  char tmp[MAX_CHAR];
  strcpy(tmp, expr_stack[index]);
  strcpy(expr_stack[index], expr_stack[index+1]);
  strcpy(expr_stack[index+1], tmp);
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

int calculate_const_value(char* op, int left, int right){
  char* ch = op;
	if(strcmp(ch, "+")     == 0)	  return left + right;
	else if(strcmp(ch, "-") ==0)	  return left - right;
	else if(strcmp(ch, "*") ==0)	  return left * right;
	else if(strcmp(ch,"div")==0)    return left / right;

}

void push2nodes(char* op, char* left, char* right){

  char r[MAX_CHAR];
  // 定数計算用
  int result, l_value, r_value;
  int i;

  DebugOut2("THIS IS DEBUG l is %s, r is %s\n", left, right);

    for(i = p_stack; i>=0;i--){
      DebugOut2("\tstack No.%d is %s\n", i, expr_stack[i]);
    }



  // コンパイル時に計算できる定数は先に計算する
  l_value = atoi(left);
  r_value = atoi(right);
  if(l_value != 0 && r_value != 0){
    result = calculate_const_value(op, l_value, r_value);
    pop();pop();pop();
    sprintf(r, "%d", result);
    push(r);
    return;
  }

  node_count ++;
  strcpy(nodes[node_count].op, op);
  strcpy(nodes[node_count].l, left);
  strcpy(nodes[node_count].r, right);
  nodes[node_count].regi = g_register++;

  DebugOut2("#########Now, I pushed this node: %s %s %s\n", nodes[node_count].op, nodes[node_count].l, nodes[node_count].r);

  pop();pop();pop();


  for(i = p_stack; i>=0;i--){
      DebugOut2("\tAfter pop, stack No.%d is %s\n", i, expr_stack[i]);
  }

  sprintf(r, "__r%d", nodes[node_count].regi);
  push(r);

  // 追加した仮想のレジスタをメモリに追加するため
  strcpy(regi_var[regi_var_count].name, r);
  regi_var[regi_var_count].addr = var_count + regi_var_count;

  DebugOut2("regi %d is %s\n", regi_var_count,
	  regi_var[regi_var_count].name);
  regi_var_count++;

}


// ノードからアセンブリを出力（仮）
void flush_node_impl(int index){
}

int is_register(char* s){
  if(strlen(s) < 3)
    return 0;

  if(s[0] == '_' && s[1] == '_')
    return 1;
  else return 0;
}

void flush_stack(void){
		DebugOut("\tflush begin");
		int i;
    for(i = p_stack; i>=0;i--){
      DebugOut2("\tstack No.%d is %s\n", i, expr_stack[i]);
      // 最後に残った被演算子がレジスタでなく定数値だったらスタックに積む
      if(!is_register(expr_stack[i]) && strcmp(expr_stack[i], "$") != 0)
      {
        node_count ++;
        strcpy(nodes[node_count].op, "");
        strcpy(nodes[node_count].l, expr_stack[i]);
        strcpy(nodes[node_count].r, "");
        nodes[node_count].regi = g_register++;
        DebugOut2("#########Now, I pushed this node: %s %s %s\n", nodes[node_count].op, nodes[node_count].l, nodes[node_count].r);
      }
    }
		DebugOut("\tflush end");

    DebugOut("\t and flush node begin");
    for(i = 0; i <= node_count; i++)
      DebugOut2("\t\tnodes No.%d is : op %s, l %s, r %s, regi %d\n", i, nodes[i].op, nodes[i].l, nodes[i].r, nodes[i].regi);
    DebugOut("\t and flush node end");
}

enum Rank
{
	RankGreater, // a >> b
	RankLess,	// a << b
	RankEqual   // a = b
};

int f(char* ch){
	if(strcmp(ch, "+")     == 0)	  return 2;
	else if(strcmp(ch, "-") ==0)	  return 2;
	else if(strcmp(ch, "*") ==0)	  return 4;
	else if(strcmp(ch,"div")==0)    return 4;
	else if(strcmp(ch, "!") ==0)	  return 6;
	else if(strcmp(ch, "(") ==0)	  return 0;
	else if(strcmp(ch, ")") ==0)	  return 11;
//	else if(strcmp(ch, "i")==0)	return 11;
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
//	else if(strcmp(ch, "i")==0)	return 10;
	else if(strcmp(ch, "$")==0)	return 0;
    else fprintf(stderr, "illegal operator\n"), exit(1);
}

int ranking(int f, int g){
	if(f > g) 			return RankGreater;
	else if(f == g) 	return RankEqual;
	else /*if(f < g)*/	return RankLess;
}



int isOperator(char* s)
{
  int i;
    if(
      (strcmp(s, "+") == 0)	||
      (strcmp(s, "-")==0)   ||
      (strcmp(s, "*")==0)   ||
      (strcmp(s,"div")==0)  ||
      (strcmp(s, "!")==0)	  ||
      (strcmp(s, "(")==0)	  ||
      (strcmp(s, ")")==0)	  ||
//      (strcmp(s, "i")==0)   ||
      (strcmp(s, "$")==0)
      )
    {
      return 1;
    }
    else
    {
      return 0;
    }
}



static void ranking_function(char* a_j){
  DebugOut2("THIS IS RANKING FUNC ABOUT %s!\n", a_j);
  int i, j, rank;
  char* a_i;
  int hogehoge;

  for(i = p_stack; i >= 0; i--){
      DebugOut2("\t stack No.%d is %s\n", i, expr_stack[i]);
  }

  for(i = p_stack; i >= 0; i--){
    if(isOperator(expr_stack[i]) == 1){
      a_i = expr_stack[i];
      rank = ranking(f(a_i), g(a_j));
        if(rank == RankGreater){
          do{
            DebugOut2("compared %s and %s, op is greater!\n", a_i, a_j);
            DebugOut2("\t\t\t---------------------- %s %s\n", expr_stack[i-1], expr_stack[i+1]);
            for(hogehoge = p_stack; hogehoge>=0;hogehoge--){
              DebugOut2("\t stack No.%d is %s\n", i, expr_stack[hogehoge]);
            }
            push2nodes(a_i, expr_stack[i-1],expr_stack[i+1]);
//            push(a_j);

            for(j = p_stack; j >= 0; j--){
              if(isOperator(expr_stack[j]))
              {
                i = j;
                a_i = expr_stack[j]; // 最終的に$が入る
                rank = ranking(f(a_i), g(a_j));
                break;
              }
            }

            if(rank == RankGreater)
              continue;
            else if(rank == RankLess)
            {
              push(a_j);
              break;
            }
            else if(rank == RankEqual){
              if(strcmp(a_i, "(") == 0)
              {
                erase(j);
                break;
              }
              else if(strcmp(a_i, "$") == 0)
                return;
            }
          }while(1);//(rank == RankGreater);
          break;
        }
        else if(rank == RankLess){
          DebugOut2("compared %s and %s, op is less!\n", a_i, a_j);
          push(a_j);
          break;
        }
        else if(rank == RankEqual){
          DebugOut2("compared %s and %s, op is equal!\n", a_i, a_j);
          if(strcmp(a_i, "(") == 0)
          {
            swap(i); pop();
          }
          else if(strcmp(a_i, "$") == 0)
            break;
          break;
        }
    }
    else
      continue;
  }
}

void term(void);
void factor(void);

void expression2(void){
		DebugOut("expr begin");
		do{
				getsym();
				if((tok.attr == NUMBER) ||
					(tok.attr == IDENTIFIER) // guaranteed id is only var
				//		fprintf(stderr, "undeclared var\n");
					){
					DebugOut2("\t %d %d %s\n", tok.attr, tok.value, tok.charvalue);
					// push
          push(tok.charvalue);
        }
				else if(tok.attr == RWORD && tok.value == DIV){
            ranking_function(tok.charvalue);
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
            ranking_function(tok.charvalue);
				}
				else{
						break;
				}
		}while(1);
EXIT:
    //flush_node();
    ranking_function("$");
		DebugOut("expr end");
}

void expression(void){
 do{
   getsym();
   term();

	if(tok.attr == SYMBOL && tok.value == PLUS){
      ranking_function(tok.charvalue);
	}
	else if(tok.attr == SYMBOL && tok.value == MINUS){
      ranking_function(tok.charvalue);
	}
	else{
			// if(tok.attr == SYMBOL && tok.value == RPAREN) //temp
			// {}//temp
			// else//temp
		  // flush_node();
			break;
	}
 }while(1);

 ranking_function("$");
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
      ranking_function(tok.charvalue);
      getsym();
    }
    else if(tok.attr == SYMBOL&& tok.value == TIMES) {
      //dosomething
      DebugOut("times come");
      ranking_function(tok.charvalue);
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
      ranking_function("!");
  }

  if(tok.attr == IDENTIFIER){
		  push(tok.charvalue);
		  DebugOut2("%s\n", tok.charvalue);
  }
  else if(tok.attr == NUMBER){
		  push(tok.charvalue);
		  DebugOut2("number come and the value is %d\n", tok.value);
  }
  else if(tok.attr == SYMBOL && tok.value == LPAREN){
      ranking_function(tok.charvalue);
      DebugOut("LPAREN is come");
      expression();

      if(tok.value != RPAREN)
        fprintf(stderr, "illegal bunpou\n"), exit(1);
      DebugOut("RPAREN is come");
  }
}

void output_nodes(void){
  int i;
  for(i = 0; i <= node_count; i++)
    DebugOut2("node No.%d is: op=%s, l=%s, r=%s, regi=%d\n",
      i, nodes[i].op, nodes[i].l, nodes[i].r, nodes[i].regi);
}
