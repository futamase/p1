#include <stdio.h>
#include <stdlib.h>
#include <getsym.h>
extern TOKEN tok;
extern FILE *infile;
extern FILE *outfile;
static int register_num;

#define Output(s) fprintf(outfile, s)
#define Output2(s, x) fprintf(outfile, s, x)

typedef struct MemData{
  char name[MAXIDLEN];
  int addr; 
}MemData;
#define MAXADDR 65535
MemData vars[MAXADDR]; // 変数の上限はないのか

void error(char* s);
void outblock(void);
void statement(void);
int expression(void);

void write(int _register_num){
  Output("writed\t");
  
  do{
    getsym();
    Output2(" r%d", tok.value);
  }while(tok.value == COMMA);

  // 改行
  Output("\n");
  Output2("load\t r%d", register_num+1);
  Output2("writec\t r%d", register_num+1);
}


void compiler(void) {
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

	// 本体読み込み
	getsym();

	// outblockの処理
	outblock();
	getsym();

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
	if (tok.value == BEGIN) {
		printf("begin\n");

		do {
			getsym();
			statement();
			getsym();
		} while (tok.value == SEMICOLON);

		if (tok.value != END)
			error("must need end");

		printf("end\n");
	}
	else if (tok.value == IF) {

	}
	else if (tok.value == WHILE) {

	}
	else{
		// identifier -> := -> expression
		// か
		// identifier -> paramlist

    // この調査で見つからなかったら関数名
    for(int 0; i < MAXADDR; i++){
      if(strcmp(tok.charvalue, vars[i].name) == 0){
        // 変数に対する処理
        // read := 
        getsym();
        
        if(tok.value != BECOMES)
          error("illegal bunpou");


      }
    }

    // でもまだ関数実装できてない
	}
}

// 単純な2項式を処理する
int expression(void) {
	int left_attr;
	int left_value; // 定数値か変数
	int right_attr;
	int right_value;

	int operator_ = 0;

	if (tok.attr != NUMBER && tok.attr != IDENTIFIER)
		error("uwa-");

	left_attr = tok.attr;
	left_value = tok.value;
	printf("the value is:%d\n", tok.value);

	getsym();

  if(tok.value == SEMICOLON)
    return left_value;

	if (tok.attr != SYMBOL)
		printf("After number, symbol is needed %d\n", tok.attr);

	// 演算子ごとに処理
	printf("the manu is:%c\n", tok.value);
	operator_ = tok.value;
	getsym();
	// num ○  num だけ
	if (tok.attr != NUMBER && tok.attr != IDENTIFIER)
		error("num hoge num is only accepted");

	right_attr = tok.attr;
	right_value = tok.value;

	switch (operator_) {
	case PLUS:
	{
		printf("the result is: %d\n", left_value + right_value);
    return left_value + right_value;
		break;
	}
	case MINUS:
	{
		printf("the result is: %d\n", left_value - right_value);
    return left_value - right_value;
		break;
	}
	case TIMES:
	{
		printf("the result is: %d\n", left_value * right_value);
    return left_value * right_value;
		break;
	}
	case DIV:
	{
		printf("the result is: %d\n", left_value / right_value);
    return left_value / right_value;
		break;
	}
	default:
		break;
	}

  return 0;
}

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

    var_count++;

		getsym();
		if (tok.value != COMMA && tok.value != SEMICOLON)
			error("need comma or semicolon");

		if (tok.value == COMMA)	
      continue;
		else					          
      break;
	} while (1);

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
    printf("%c\n", tok.value);
		statement();
	}
}
