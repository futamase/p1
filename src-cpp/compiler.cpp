#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "define.h"
#include "output.h"
#include "sym_table.h"
#include "expression.h"
#include "translation.h"

extern TOKEN tok;
extern FILE *infile;
extern FILE *outfile;
extern Node nodes[100];
extern int node_count;
static int register_num;


extern SymData data_labels[100];
extern int label_counter;

static int while_label_num = 0;
static int if_label_num = 0;

static SymData symbol;

void error(char* s) {
	fprintf(stderr, "%s\n", s);
	exit(1);
}

void inblock(void);
void outblock(void);
void statement(void);
//int expression(void);
void condition(char*);
void paramlist(char* label);

// pos dest のpos番目からコピー
void deep_copy_node(Node* dest, Node* src, int pos, size_t num){
    int i;

    for(i = 0; i < num; i++){
        strcpy(dest[pos+i].op, src[i].op);
        strcpy(dest[pos+i].l, src[i].l);
        strcpy(dest[pos+i].r, src[i].r);
        dest[pos+i].regi = src[i].regi;
    }
}
// pos dest のpos番目からコピー
void deep_copy_node2(Node* dest, Node* src, int pos, size_t num){
    int i;
    DebugOut2("copy %d %d\n", pos, num);

    for(i = 0; i < num; i++){
        strcpy(dest[i].op, src[pos+i].op);
        strcpy(dest[i].l, src[pos+i].l);
        strcpy(dest[i].r, src[pos+i].r);
        dest[i].regi = src[pos+i].regi;
    }
}






void write(void){
  int i;
  int write_space_flag = 0;
  DebugOut("Wow, Writing!");


  do{
    getsym();

    if(get_symbol(tok.charvalue, &symbol)){
//        Output2("load\tr0,%d%s\n", symbol.addr, get_additional_op[symbol.attr]);
				add_op("load", "r0", std::to_string(symbol.addr) + get_additional_op(symbol.attr));
//        Output("writed r0");
				add_op("writed", "r0", "");
    }
    else
      error("var was'nt found");

    getsym();
    if(tok.value == COMMA){
      if(!write_space_flag){
        // スペースのロードは一回
//        Output("loadi\tr1, ' '");
				add_op("loadi", "r1", "' '");
        write_space_flag = 1;
      }
//      Output("writec\tr1");
			add_op("writec", "r1", "");
      continue;
    }
    else
      break;
  }while(1);//(tok.value == COMMA);

//  Output("loadi\t r0, '\\n'");
		add_op("loadi", "r0", "'\\n'");
//  Output("writec\tr0");
		add_op("writec", "r0", "");
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
//	Output("loadi\tr4,10000");
	add_op("loadi", "r4", "10000");
  // 関数が定義される可能性があるため、mainラベルに飛ぶ処理を書く
  // Output("jmp main");
	add_op("jmp", "main", "");


	getsym();

	// outblockの処理
	outblock();
	getsym();

	// 本体読み込み<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// 式が正常に終わっているか確認
	if (tok.attr != SYMBOL || tok.value != PERIOD)
		error("At the end, a period is required.");

	fprintf(stderr, "Parsing Completed. No errors found.\n");

//  Output("halt");
	add_op("halt", "","");

  for(i = 0; i < label_counter;i++)
//    Output2("%s\n", data_labels[i].name);
		add_op(data_labels[i].name, "","");
}

/* 式を読み込み、その式を計算し結果を表示するコードを生成する処理*/
void statement(void) {
	int i, j, k;

	char label_buffer[255];
	int if_label_counter_register = 0;
	int while_label_counter_register = 0;

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
				if_label_counter_register = if_label_num++;
				DebugOut("if begin");
//				getsym();
				sprintf(label_buffer, "elseLabel%d", if_label_counter_register);
				condition(label_buffer);
				if(tok.attr != RWORD || tok.value != THEN)
				{
						DebugOut2("then attr and value is %d %s\n", tok.attr, tok.charvalue);
						error("if neccesary then");
				}

//				Output2("ifLabel%d:\n", if_label_counter_register);
				add_op("ifLabel"+std::to_string(if_label_counter_register) + ":", "", "");
				getsym();
				statement();

//        Output2("jmp\tifExitLabel%d\n", if_label_counter_register);
				add_op("jmp", "ifExitLabel" + std::to_string(if_label_counter_register), "");

				// 糞コードを極めていくスタイル
				// if 部がbegin-endのときにelseが読み込まれなかった
				if(tok.attr == RWORD && tok.value == END)
					getsym();

//				Output2("elseLabel%d:\n", if_label_counter_register);
				add_op("elseLabel" + std::to_string(if_label_counter_register) + ":", "", "");
				if(tok.attr == RWORD && tok.value == ELSE){
					DebugOut("else begin");
					getsym();

					statement();
					DebugOut("else end");
				}

//        Output2("ifExitLabel%d:\n", if_label_counter_register);
				add_op("ifExitLabel"+std::to_string(if_label_counter_register) + ":", "", "");
				DebugOut("if end");
		}
		else if (tok.value == WHILE) {
			while_label_counter_register = while_label_num++;
				DebugOut("while begin");
//				getsym();
//        Output2("whileLabel%d:\n", while_label_counter_register);
				add_op("whileLabel"+std::to_string(while_label_counter_register) + ":", "", "");

				sprintf(label_buffer, "whileEndLabel%d", while_label_counter_register);
				condition(label_buffer);
				if(tok.attr != RWORD || tok.value != DO)
						error("while neccesary do");

				DebugOut("DO!");
				getsym();
				statement();
				getsym();

				DebugOut("while end");

        // Output2("jmp\t whileLabel%d\n", while_label_counter_register);
				add_op("jmp", "whileLabel" + std::to_string(while_label_counter_register), "");
        // Output2("whileEndLabel%d:\n", while_label_counter_register);
				add_op("whileEndLabel"+std::to_string(while_label_counter_register) + ":", "", "");
		}
		else if(tok.value == WRITE){
				write();
		}
	}
	else{
		// identifier -> := -> expression
		// か
		// identifier -> paramlist

		DebugOut("identifier begin");

    if(!get_symbol(tok.charvalue, &symbol))
      error("identifier was'nt found");

    if(symbol.attr == Func){
      // でもまだ関数実装できてない
      paramlist(symbol.name);
    }
    else{
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

      // Output2("store\tr%d, %d%s\n", translate(), symbol.addr, get_additional_op[symbol.attr]);
			std::string __1 = "store";
			DebugOut("馬鹿野郎");
			int sabanomisoni = translate();
			std::string __2 = "r"+std::to_string(sabanomisoni);
			DebugOut2("馬鹿野郎with %s, addr:%d, attr:%d\n", symbol.name, symbol.addr, symbol.attr);
			std::string __3 = std::to_string(symbol.addr) + get_additional_op(symbol.attr);
			DebugOut("馬鹿野郎");

			add_op(__1,__2,__3);

			DebugOut("馬鹿野郎");
    }
	}
}

// グローバル変数読み込み
void read_var(void) {
	do{
		getsym();
		if (tok.attr != IDENTIFIER)
			error("not var");

    add_symbol(tok.charvalue, Var);

		getsym();
		if (tok.value != COMMA && tok.value != SEMICOLON)
			error("need comma or semicolon");

		if (tok.value == COMMA)
			continue;
		else
			break;
	} while (1);

  flush_all_symbols();

  // SPを変数の数+1 にする
//  Output2("loadi\tr4,%d\n", var_count);

  // セミコロン後１ワードを取り出す
  getsym();
}

// 変数宣言の処理
void outblock(void) {
  int i;
	if (tok.attr == RWORD && tok.value == VAR)
		read_var();

	if (tok.attr == RWORD && tok.value == PROCEDURE) {
		// 関数定義の処理 実験２ではまだ
    do{
      DebugOut("procudure begin");

      clear_local_vars();

      getsym();
      // 関数名登録
      // Output2("%s:\n", tok.charvalue);
			add_op(std::string(tok.charvalue)+":", "", "");
      add_symbol(tok.charvalue, Func);
      // Output("push\t r5\nloadr\t r5, r4\naddi\t r5, 1");
			add_op("push", "r5", "");
			add_op("loadr", "r5", "r4");
			add_op("addi", "r5", "1");

      getsym();
DebugOut("inblock begin");
      inblock();
DebugOut("inblock end");

      // Output("pop\t r5\nreturn");
			add_op("pop", "r5", "");
			add_op("return", "","");

      if(tok.attr != SYMBOL || tok.value != SEMICOLON)
        error("illegal bunpou in outblock");

      getsym();
      if(tok.value != PROCEDURE)
        break;

      DebugOut("procudure end");
    }while(1);
	}

	clear_local_vars();

  // きっとstatementと信じて送り出す
  // main部分が始まる
//  Output("main:");
	add_op("main:", "", "");
  DebugOut("statement begin");
  statement();
}

void inblock(void){
  int i;
  int arg_count = 0, var_ = 0;
  if(tok.attr != SYMBOL || tok.value != LPAREN)
    error("illegal bunpou in inblock");

  getsym();
  if(tok.attr == IDENTIFIER){
    //
    do{
      // ここで引数に関する処理
      //
//      DebugOut2("arg %d: is %s\n", local_var_count++, tok.charvalue);
      add_symbol(tok.charvalue, Arg);
			DebugOut2("add ARG : %s\n", tok.charvalue);
      arg_count++;

      // , を期待して読み込む
      getsym();
      if(tok.value != COMMA)
        break;
      getsym();
    }while(1);
  }


  if(tok.value != RPAREN)
    error("illegal bunpou");

  getsym();
  if(tok.value != SEMICOLON)
    error("illegal bunpou");


  getsym();
  // ローカル変数が定義されていたら読み込む
  if(tok.attr == RWORD && tok.value == VAR){
    do{
      getsym();
      // これは変数名
//      DebugOut2("local_var %d: is %s\n", local_var_count++, tok.charvalue);
      add_symbol(tok.charvalue, Local);
      var_++;

      getsym();
      if(tok.value == COMMA)
        continue;
      else if(tok.value == SEMICOLON)
        break;
      else
        error("illegal bunpou");
    }while(1);
  }

// Output2("addi\tr4, %d\n", var_);
	add_op("addi", "r4", std::to_string(var_));

  getsym();
  statement();

  for(i = 0; i < arg_count; i++){
  }

  // Output2("subi\tr4, %d\n", var_);
	add_op("subi", "r4", std::to_string(var_));

  getsym();
}

void condition(char* label){
  int r1,r2,sym;
	std::string op;

  DebugOut("condition start");

  init_nodes();
  expression();
  flush_stack();

  r1 = translate();
  // 比較のために退避しておく
  // Output2("store\t r%d, %d\n", r1, MAXADDR-1);
	// add_op("store", "r"+std::to_string(r1), std::to_string(MAXADDR-1));
	add_op("loadr", "r3", "r"+std::to_string(r1));

	if(tok.attr != SYMBOL)
			error("illegal bunpou");

  sym = tok.value;

  init_nodes();
	expression();
  flush_stack();

  r2 = translate();

  // 愛を取り戻す
  r1 = (r2 + 1) % 4;
  // Output2("load\t r%d, %d\n", r1, MAXADDR-1);
//	add_op("load", "r"+std::to_string(r1), std::to_string(MAXADDR-1));

  // Output2("cmpr\tr%d, r%d\n", r1, r2);
	// add_op("cmpr", "r"+std::to_string(r1), "r"+std::to_string(r2));
	add_op("cmpr", "r3", "r"+std::to_string(r2));

	switch(sym){
		case EQL: 		op = "jnz"; break;
		case NOTEQL: 	op = "jz";  break;
		case LESSTHAN:op = "jge"; break;
		case LESSEQL: op = "jgt"; break;
		case GRTRTHAN:op = "jle"; break;
		case GRTREQL: op = "jlt";	break;
	}
	add_op(op, label, "");

	DebugOut("condition end");
}


int sum_of_node_count_storage(int* storage, int arg_num){
  int i;
  int sum = 0;
  for(i = 0; i < arg_num; i++){
      sum += storage[i];
  }

  return sum;
}

void paramlist(char* label){
  int regi;
  int i, j;
  Node* nodes_storage;
  int* node_count_storage;
  int arg_num = 0;

  nodes_storage = (Node*)calloc(1, sizeof(Node));
  node_count_storage = (int*)calloc(1, sizeof(int));

	getsym();
	if(tok.value != LPAREN)
		error("func must begin '('");

  do{
    init_nodes();
    expression();
    flush_stack();

    // void の可能性を考慮
    // if(tok.attr == SYMBOL && tok.value == RPAREN)
    //   break;


    arg_num++;


    DebugOut2("%d\n", (sum_of_node_count_storage(node_count_storage, arg_num-1) + node_count+1));
    nodes_storage = (Node*)realloc(nodes_storage,
      sizeof(Node) * (sum_of_node_count_storage(node_count_storage, arg_num-1) + node_count+1));
        DebugOut("EEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
    deep_copy_node(nodes_storage, nodes, sum_of_node_count_storage(node_count_storage, arg_num-1),
        node_count+1);

    node_count_storage = (int*)realloc(node_count_storage, sizeof(int) * arg_num);
    node_count_storage[arg_num-1] = node_count + 1;
    DebugOut2("%d\n", node_count_storage[arg_num-1]);
//    memcpy(nodes_storage, nodes, sizeof(Node) * node_count+1);
//    strcpy(nodes_storage[0].l, "hoge");
    for(i = 0; i <= node_count; ++i)
      DebugOut2("node No.%d is: op=%s, l=%s, r=%s, regi=%d\n", i, nodes_storage[i].op, nodes_storage[i].l, nodes_storage[i].r, nodes_storage[i].regi);

//    regi = translate();
 //   Output2("pop\tr%d\n", regi);

    if(tok.value != COMMA){
      break;
    }

    DebugOut2("|||||||||||||%s||||||||||||||\n", tok.charvalue);

  }while(1);

  DebugOut("ほげーーーーーーーーー");

    for(j = 0; j < sum_of_node_count_storage(node_count_storage, arg_num); ++j){
      DebugOut2("node No.%d is: op=%s, l=%s, r=%s, regi=%d\n", j,
      nodes_storage[j].op, nodes_storage[j].l, nodes_storage[j].r, nodes_storage[j].regi);
    }
  DebugOut2("%d\n", sum_of_node_count_storage(node_count_storage, arg_num-1));
  DebugOut("ほげーーーーーーーーー");

  for(i = arg_num-1; i >= 0; --i){
    deep_copy_node2(nodes, nodes_storage, sum_of_node_count_storage(node_count_storage, i), node_count_storage[i]);
    node_count = node_count_storage[i] -1;
    for(j = 0; j <= node_count; ++j){
      DebugOut("はげーーーーーーーーー");
      DebugOut2("node No.%d is: op=%s, l=%s, r=%s, regi=%d\n", j,
      nodes[j].op, nodes[j].l, nodes[j].r, nodes[j].regi);
    }

    regi = translate();
  //  Output2("push\tr%d\n", regi);
		add_op("push", "r"+std::to_string(regi), "");
  }
  free(nodes_storage);
  free(node_count_storage);

	if(tok.value != RPAREN)
		error("func must end ')'");

  // Output2("call\t %s\n", label);
	add_op("call", label, "");
  // Output2("subi\t r4, %d\n", arg_num);
	add_op("subi", "r4", std::to_string(arg_num));

    getsym();
  DebugOut2("END OF PARAMLIST|||||||||||||%s||||||||||||||\n", tok.charvalue);
}
