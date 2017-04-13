#include <stdio.h>
#include <stdlib.h>

#include "define.h"
#include "expression.h"
#include "translation.h"

extern TOKEN tok;
extern FILE *infile;
extern FILE *outfile;
extern Node nodes[100];
extern int node_count;
static int register_num;
int var_count = 0; //グローバル化


extern char data_labels[100][255];
extern int label_counter;

MemData vars[MAXADDR]; // 変数の上限はないのか

void error(char* s);
void outblock(void);
void statement(void);
//int expression(void);
void condition(char*);
void paramlist(void);

void write(int _register_num){
  int i;
  DebugOut("Wow, Writing!");
  do{
    getsym();
    
    for(i = 0; i < MAXADDR; i++){
      if(strcmp(tok.charvalue, vars[i].name) == 0){
        Output2("load\tr0,%d\n", vars[i].addr);
        Output("writed r0");
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

  Output("loadi\t r0, '\\n'");
  Output("writec\tr0");
}

void compiler(void) {
  int i;
	init_getsym();

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
	
	// SP(r4) => 10000
	Output("loadi\tr4,10000");


	getsym();

	// outblockの処理
	outblock();
	getsym();

	// 本体読み込み<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// 式が正常に終わっているか確認
	if (tok.attr != SYMBOL || tok.value != PERIOD)
		error("At the end, a period is required.");

	fprintf(stderr, "Parsing Completed. No errors found.\n");

  Output("halt");

  for(i = 0; i < label_counter;i++)
    Output2("%s\n", data_labels[i]);
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
	int i, j, k;

	if(tok.attr == RWORD){
		if (tok.value == BEGIN) {
			DebugOut("scope begin");

			do {
				getsym();
				statement();

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
				condition("elseLabel");
				if(tok.attr != RWORD || tok.value != THEN)
				{
						DebugOut2("then attr and value is %d %s\n", tok.attr, tok.charvalue);
						error("if neccesary then");
				}

        fprintf(outfile, "ifLabel:");
				getsym();
				statement();

        
        Output("jmp\tifExitLabel");

        Output("elseLabel:");
				if(tok.attr == RWORD && tok.value == ELSE){
						DebugOut("else begin");
						getsym();
						statement();
						DebugOut("else end");
				}


        Output("ifExitLabel:");

				DebugOut("if end");
		}
		else if (tok.value == WHILE) {
				DebugOut("while begin");
//				getsym();
        Output("whileLabel:");
				condition("whileEndLabel");
				if(tok.attr != RWORD || tok.value != DO)
						error("while neccesary do");


				DebugOut("DO!");
				getsym();
				statement();
				getsym();

				DebugOut("while end");

        Output("jmp\t whileLabel");
        Output("whileEndLabel:");
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

        init_nodes();
        expression();
        flush_stack();

        for(j = 0; j < node_count; j++){
          DebugOut2("after flush, this is node: %s %s %s\n", nodes[j].op, nodes[j].l, nodes[j].r);
        }

        Output2("store\tr%d, %d\n", translate(), vars[i].addr);

        //ここで演算結果が格納されているであろうスタックからポップ
        //"pop R0"とか
        //からの"store 変数番地"

//        getsym();
				return;
			}
		}

    // でもまだ関数実装できてない
		paramlist();
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

  // SPを変数の数+1 にする
//  Output2("loadi\tr4,%d\n", var_count);

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

void condition(char* label){
  int r1,r2,sym;

  DebugOut("condition start");
	
  init_nodes();
  expression();	
  flush_stack();

  r1 = translate();
  // 比較のために退避しておく
  Output2("store\t r%d, %d\n", r1, MAXADDR-1);

	if(tok.attr != SYMBOL)
			error("illegal bunpou");

  sym = tok.value;
	
  init_nodes();
	expression();
  flush_stack();

  r2 = translate();

  // 愛を取り戻す
  r1 = (r2 + 1) % 4;
  Output2("load\t r%d, %d\n", r1, MAXADDR-1);

  Output2("cmpr\tr%d, r%d\n", r1, r2);

	switch(sym){
		case EQL:
        Output2("jnz\t%s\n", label);
			break;
		case NOTEQL:
        Output2("jz\t%s\n", label);
			break;
		case LESSTHAN:
        Output2("jge\t%s\n", label);
			break;
		case LESSEQL:
        Output2("jgt\t%s\n", label);
			break;
		case GRTRTHAN:
        Output2("jle\t%s\n", label);
			break;
		case GRTREQL:
        Output2("jlt\t%s\n", label);
			break;
	}
	DebugOut("condition end");
}

void paramlist(void){
	getsym();
	if(tok.value != LPAREN)
		error("func must begin '('");
}

