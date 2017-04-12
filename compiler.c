#include <stdio.h>
#include <stdlib.h>
#include <getsym.h>

#include "define.h"
#include "expression.h"

extern TOKEN tok;
extern FILE *infile;
extern FILE *outfile;
extern Node nodes[100];
extern int node_count;
static int register_num;


typedef struct MemData{
  char name[MAXIDLEN];
  int addr; 
  char hoge;
}MemData;
#define MAXADDR 65535
MemData vars[MAXADDR]; // 変数の上限はないのか

void error(char* s);
void outblock(void);
void statement(void);
//int expression(void);

void write(int _register_num){
  int i;
  fprintf(stderr, "hoge\n");
  do{
        DebugOut2("%s ", tok.charvalue);
    getsym();
        DebugOut2("%s ", tok.charvalue);
    
    for(i = 0; i < MAXADDR; i++){
      if(strcmp(tok.charvalue, vars[i].name) == 0){
  fprintf(stderr, "hoge %s\n", vars[i].name);
        Output2("load\tr0,%d\n", vars[i].addr);
  fprintf(stderr, "hoge %s\n", vars[i].name);
        Output("writed r0");
  fprintf(stderr, "hoge %s\n", vars[i].name);
        break;
      }
    }

//    Output("writed\t");
//    DebugOut2("write %s\n", tok.charvalue, tok.attr);
//    Output2(" r%d", tok.value);
    getsym();
    if(tok.value == COMMA){
      Output("loadi\tr0, ' '");
      Output("writec\tr0");
      continue;
    }
    else
      break;
  }while(1);//(tok.value == COMMA);
  DebugOut("hoge");

  Output("loadi\t r0, '\\n'");
  Output("writec\tr0");
}

void compiler(void) {
	init_getsym();
	init_nodes();

  register_num = -1;

	// programを読み込み
	getsym();

	if (tok.attr != RWORD || tok.value != PROGRAM)
		error("At the first, program declarationi is required.");

	// プログラム名を読み込み
	getsym();

	if (tok.attr != IDENTIFIER)
		error("Program identifier is needed.");

	// プログラム名のあとのセミコロンを読み込み
	getsym();

	if (tok.attr != SYMBOL || tok.value != SEMICOLON)
		error("After program name, a semicolon is needed.");

	// 本体読み込み>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	getsym();

	// outblockの処理
	outblock();
	getsym();

	// 本体読み込み<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// 式が正常に終わっているか確認
	if (tok.attr != SYMBOL || tok.value != PERIOD)
		error("At the end, a period is required.");

	fprintf(stderr, "Parsing Completed. No errors found.\n");
}

void error(char* s) {
	fprintf(stderr, "%s\n", s);
	exit(1);
}

// 変数に対する代入か、関数呼び出しか
void dosomething(char* _id){
  char* id;
  id = _id;

  getsym();

  if(tok.value == BECOMES){
    getsym();
//    printf("%s is substituted %d", id, expression());
  }
  else if(tok.value == LPAREN){
  }
  else{
    error("illegal bunpou");
  }
}

/* 式を読み込み、その式を計算し結果を表示するコードを生成する処理*/
void statement(void) {
	int i;

	if(tok.attr == RWORD){
		if (tok.value == BEGIN) {
			DebugOut("scope begin");

			do {
				getsym();
				statement();
        flush_node();
//				getsym();
			} while (tok.value == SEMICOLON);

			printf("\t%c\n", tok.value);
			if (tok.value != END)
				error("must need end");

			DebugOut2("scope end and current value is %s\n", tok.charvalue);
		}
		else if (tok.value == IF) {
				DebugOut("if begin");
//				getsym();
				condition();
				if(tok.attr != RWORD || tok.value != THEN)
				{
						DebugOut2("then attr and value is %d %s\n", tok.attr, tok.charvalue);
						error("if neccesary then");
				}

				getsym();
				statement();

				if(tok.attr == RWORD && tok.value == ELSE){
						DebugOut("else begin");
						getsym();
						statement();
						DebugOut("else end");
				}
				DebugOut("if end");
		}
		else if (tok.value == WHILE) {
				DebugOut("while begin");
//				getsym();
				condition();
				if(tok.attr != RWORD || tok.value != DO)
						error("while neccesary do");

				DebugOut("DO!");
				getsym();
				statement();
				getsym();

				DebugOut("while end");
		}
		else if(tok.value == WRITE){
				write(1);
		}
	}
	else{
		// identifier -> := -> expression
		// か
		// identifier -> paramlist

		DebugOut("identifier begin");

		// この調査で見つからなかったら関数名
		for (i = 0; i < MAXADDR; i++) {
			if (strcmp(tok.charvalue, vars[i].name) == 0) {
				// 変数に対する処理
				// read := 
				DebugOut("var is found");
				getsym();

				if (tok.value != BECOMES)
					error("illegal bunpou");

        expression();

        //ここで演算結果が格納されているであろうスタックからポップ
        //"pop R0"とか
        //からの"store 変数番地"

//        getsym();
			}
		}

    // でもまだ関数実装できてない
	}
}

// 単純な2項式を処理する
//int expression(void) {
//	int left_attr;
//	int left_value; // 定数値か変数
//	int right_attr;
//	int right_value;
//
//	int operator_ = 0;
//
//	if (tok.attr != NUMBER && tok.attr != IDENTIFIER)
//		error("uwa-");
//
//	left_attr = tok.attr;
//	left_value = tok.value;
//	printf("the value is:%d\n", tok.value);
//
//	getsym();
//
//  if(tok.value == SEMICOLON)
//    return left_value;
//
//	if (tok.attr != SYMBOL)
//		printf("After number, symbol is needed %d\n", tok.attr);
//
//	// 演算子ごとに処理
//	printf("the manu is:%c\n", tok.value);
//	operator_ = tok.value;
//	getsym();
//	// num ○  num だけ
//	if (tok.attr != NUMBER && tok.attr != IDENTIFIER)
//		error("num hoge num is only accepted");
//
//	right_attr = tok.attr;
//	right_value = tok.value;
//
//	switch (operator_) {
//	case PLUS:
//	{
//		printf("the result is: %d\n", left_value + right_value);
//    return left_value + right_value;
//		break;
//	}
//	case MINUS:
//	{
//		printf("the result is: %d\n", left_value - right_value);
//    return left_value - right_value;
//		break;
//	}
//	case TIMES:
//	{
//		printf("the result is: %d\n", left_value * right_value);
//    return left_value * right_value;
//		break;
//	}
//	case DIV:
//	{
//		printf("the result is: %d\n", left_value / right_value);
//    return left_value / right_value;
//		break;
//	}
//	default:
//		break;
//	}
//
//  return 0;
//}

void expr(void){
  switch(tok.attr){
    case NUMBER:
      {
        break;
      }
    case IDENTIFIER:
      {
        break;
      }
    default:
      error("illegal bunpou");
  }
}

void read_var(void) {
	int var_count = 0; //後後グローバル化


	// 変数読み込み
	do{
		getsym();
		if (tok.attr != IDENTIFIER)
			error("not var");

		// 変数名とインデックスのマップが欲しいか
		strcpy(vars[var_count].name, tok.charvalue);
		vars[var_count].addr = var_count;

		DebugOut2("var %d is: %s\n", vars[var_count].addr, vars[var_count].name);

		var_count++;

		getsym();
		if (tok.value != COMMA && tok.value != SEMICOLON)
			error("need comma or semicolon");

		if (tok.value == COMMA)	
			continue;
		else					          
			break;
	} while (1);

  // セミコロン後１ワードを取り出す
  getsym();
}

// 変数宣言の処理
void outblock(void) {
	if (tok.attr == RWORD && tok.value == VAR)
		read_var();

	if (tok.attr == RWORD && tok.value == PROCEDURE) {
		// 関数定義の処理 実験２ではまだ
	}
	else
	{
		// きっとstatementと信じて送り出す
		DebugOut("statement begin");
		statement();
	}
}

void condition(void){
		DebugOut("condition start");
	expression();	
  flush_node();

	if(tok.attr != SYMBOL)
			error("illegal bunpou");
	
	switch(tok.value){
		case EQL:
			break;
		case NOTEQL:
			break;
		case LESSTHAN:
			break;
		case LESSEQL:
			break;
		case GRTRTHAN:
			break;
		case GRTREQL:
			DebugOut("Grater equal come");
			break;
	}
//	getsym();
	expression();
  flush_node();

	DebugOut("condition end");
}
