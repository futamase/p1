#include <stdio.h>
#include <getsym.h>
extern TOKEN tok;
extern FILE *infile;
extern FILE *outfile;

void error(char* s);
void outblock(void);
void statement(void);
void expression(void);

typedef struct Tree {
	int value;
	struct Tree* first;
	struct Tree* second;
}Tree;

void compiler(void) {
	init_getsym();
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
	statement();
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
    printf("%s is substituted %d", id, expression());
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
		printf("the %dst var is: %s\n", var_count++, tok.charvalue);

		getsym();
		if (tok.value != COMMA && tok.value != SEMICOLON)
			error("need comma or semicolon");

		if (tok.value == COMMA)	continue;
		else					break;
	} while (1);
}

// 変数宣言の処理
void outblock(void) {
	if (tok.attr == RWORD && tok.value == VAR)
		read_var();

	getsym();
	if (tok.attr == RWORD && tok.value == PROCEDURE) {
		// 関数処理 実験２ではまだ
	}
	else
	{
		// きっとstatementと信じて送り出す
		statement();
	}
}
