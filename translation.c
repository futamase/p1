// 3124
#include "translation.h"
#include "define.h"
#include "sym_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for isdigit()


char additional_op[3][255] = {
  "", // Var
  "(BR)",// Local
  "(BR)",// Arg
};

extern FILE* outfile;
extern Node nodes[100];
extern int node_count;
extern SymData vars[MAXADDR];
extern int var_count ;

extern SymData local_vars[MAXADDR];
extern int local_var_count;

extern SymData regi_var[100];
extern int regi_var_count;

static SymData symbol;

SymData data_labels[100];
int label_counter = 0;

int isDigit(char* s){
  int i = 0;
  DebugOut2("isDigit: %s\n", s);
  //終端文字か数字以外の文字が出てくるまでループ
  for (i = 0; s[i] != NULL && isdigit(*(s + i)); ++i);
  //ループ終了時、終端に達していなければエラー
  DebugOut2("endendend %d\n", (s[i] == NULL));
  return (s[i] == NULL);
}

int isVar(char* s){
  int i;

  for(i = 0; i <= var_count; i++){
    if(strcmp(local_vars[i].name, s) == 0)
      return 1;
  }

  for(i = 0; i <= var_count; i++){
    if(strcmp(vars[i].name, s) == 0)
      return 1;
  }

  for(i = 0; i < regi_var_count; i++){
    if(strcmp(regi_var[i].name, s) == 0)
      return 1;
  }
  return 0;
}

int isRegister(char* s) {
	int i;
  for(i = 0; i < regi_var_count; i++){
    if(strcmp(regi_var[i].name, s) == 0)
      return 1;
  }
  return 0;
}

// -1 のときはレジスタ名とする
int get_var_addr(char* var){
  int i;
  DebugOut2("regi count is %d\n", regi_var_count);
  for(i = 0; i < MAXADDR; i++){
    if(strcmp(vars[i].name, var) == 0)
      return vars[i].addr;
  }
  for(i = 0; i < regi_var_count; i++){
	  DebugOut2("regi %d is %s\n", i, regi_var[i].name);
	  if (strcmp(regi_var[i].name, var) == 0) {
		  return regi_var[i].addr;
	  }
  }
  fprintf(stderr, "var %s is not matching\n", var), exit(1);
  return -1;
}

// レジスタ番号を置き換える
// origin_regi: 変える対象のレジスタ名
// dest: これに変わる
void change_register_count(int target, char* dest){
  int i;
  char target_str[255];
  sprintf(target_str, "__r%d", target);

  for(i = 0; i <= node_count; i++){
    if(strcmp(target_str, nodes[i].l) == 0){
      strcpy(nodes[i].l, dest);
	  DebugOut2("taget:%s, dest:%s\n", target_str, dest);
    }

    if(strcmp(target_str, nodes[i].r) == 0){
      strcpy(nodes[i].r, dest);
	  DebugOut2("taget:%s, dest:%s\n", target_str, dest);
    }
  }
}

int translate_(void){
  int i, j, left_var_flag;
  int addr;
  int register_ = 0;
  int const_num;

  //先に定数値のみの場合を書いてしまおう
  //と思ったが条件式には変数のみの場合もあるじゃあないか！
  if(strcmp(nodes[0].op, "") == 0){
    if(isDigit(nodes[0].l)){

      // ここで数値が16bitで扱えるかチェックして、ダメならラベルに登録
      const_num = atoi(nodes[0].l);
      if(const_num < -32768 || 32768 < const_num){
        sprintf(data_labels[label_counter].name, "label%d: data %d", label_counter, const_num);

        Output2("load\tr0, label%d\n", label_counter);
        label_counter++;
      }
      else
      Output2("loadi\tr0, %d\n", const_num);
    }
    else{

      Output2("load\tr0, %d\n", get_var_addr(nodes[0].l));
    }

    return 0;
  }
  // 2項演算
  else {
    for(i = 0; i <= node_count; i++){
      DebugOut2("これはデバッグであってリリースではない: regi %d\n", nodes[i].regi);




      // 左辺を出力
      if(isDigit(nodes[i].l))
      {
        const_num = atoi(nodes[i].l);
        if(const_num < -32768 || 32768 < const_num){
          sprintf(data_labels[label_counter].name, "label%d: data %d", label_counter, const_num);

          Output2("load\tr0, label%d\n", label_counter);
          label_counter++;
        }
        else
        Output2("loadi\tr%d, %d\n", register_, const_num);
        left_var_flag = 0;
      }
      else{
        addr = get_var_addr(nodes[i].l);
        //        if(addr)
        Output2("load\tr%d, %d\n", register_, addr);

        left_var_flag = 0;
      }

      //      register_ = (register_ + 1) % 4;

      // opによって処理を変える
      if(strcmp(nodes[i].op, "+") == 0){
        if(isDigit(nodes[i].r)){
          const_num = atoi(nodes[i].r);
          if(const_num < -32768 || 32768 < const_num){
            sprintf(data_labels[label_counter].name, "label%d: data %d", label_counter, const_num);

            Output2("add\tr0, label%d\n", label_counter);
            label_counter++;
          }
          else
          Output2("addi\tr0,%d\n", atoi(nodes[i].r));
        }
        else
        {
          addr = get_var_addr(nodes[i].r);
          //         if(addr)
          Output2("add\tr0,%d\n", addr);
          //          else
          //            Output2("a");
        }
      }
      else if(strcmp(nodes[i].op, "-") == 0){
        if(isDigit(nodes[i].r)){
          const_num = atoi(nodes[i].r);
          if(const_num < -32768 || 32768 < const_num){
            sprintf(data_labels[label_counter].name, "label%d: data %d", label_counter, const_num);

            Output2("sub\tr0, label%d\n", label_counter);
            label_counter++;
          }
          else
          Output2("subi\tr0,%d\n", atoi(nodes[i].r));
        }
        else
        Output2("sub\tr0,%d\n", get_var_addr(nodes[i].r));
      }
      else if(strcmp(nodes[i].op, "*") == 0){
        if(isDigit(nodes[i].r)){
          const_num = atoi(nodes[i].r);
          if(const_num < -32768 || 32768 < const_num){
            sprintf(data_labels[label_counter].name, "label%d: data %d", label_counter, const_num);

            Output2("mul\tr0, label%d\n", label_counter);
            label_counter++;
          }
          else{
            Output2("muli\tr0,%d\n", atoi(nodes[i].r));
          }
        }
        else
        Output2("mul\tr0,%d\n", get_var_addr(nodes[i].r));
      }
      else if(strcmp(nodes[i].op, "div") == 0){
        if(isDigit(nodes[i].r)){
          const_num = atoi(nodes[i].r);
          if(const_num < -32768 || 32768 < const_num){
            sprintf(data_labels[label_counter].name, "label%d: data %d", label_counter, const_num);

            Output2("div\tr0, label%d\n", label_counter);
            label_counter++;
          }
          else
          Output2("divi\tr0,%d\n", atoi(nodes[i].r));
        }
        else
        Output2("div\tr0,%d\n", get_var_addr(nodes[i].r));
      }

      Output2("store\tr0, %d\n", var_count + nodes[i].regi);
    }

    // 最後にメモリにためた演算結果をレジスタ0にロードしつつ０を返す
    Output2("load\tr0, %d\n", var_count + nodes[i-1].regi);
    return 0;
  }
}




void output_suitable_load_word(char* name, int regi_num){
  int i;
  SymData symbol;

  if(get_symbol(name, &symbol)){
    DebugOut2("serching success! %d, %d\n", symbol.addr, symbol.attr);
    if(symbol.attr == Arg)
      Output2("load\tr%d, %d%s\n", regi_num, symbol.addr, additional_op[symbol.attr]);
    else
      Output2("load\tr%d, %d%s\n", regi_num, symbol.addr, additional_op[symbol.attr]);
  }
  else{
    DebugOut("serching register field");
    // それでもなかったらレジスタ変数
    for(i = 0; i < regi_var_count; i++){
      if(strcmp(regi_var[i].name, name) == 0)
      Output("ばか");
    }
  }
}

int register_state[4];

int get_unused_register(void){
  int i;
  for(i = 0; i < 4; i++){
    if(register_state[i] == 0)
      return i;
  }
  return 0;
}


void maesyori(void) {
	int i;
	int regi;
	for (i = 0; i <= node_count; i++) {
		if (isRegister(nodes[i].l) ) {
			regi = atoi(&nodes[i].l[3]); // __r〇ってなってるはず
			change_register_count(nodes[i].regi, nodes[i].l);
			nodes[i].regi = regi;
		}
		else if (isRegister(nodes[i].r)) {
//			regi = atoi(&nodes[i].r[3]); // __r〇ってなってるはず
//			change_register_count(nodes[i].regi, nodes[i].r);
//			nodes[i].regi = regi;
		}
	}

	for (i = 0; i <= node_count; i++) {
		DebugOut2("(### op:%s l:%s r:%s regi:%d ###)\n", nodes[i].op, nodes[i].l, nodes[i].r, nodes[i].regi);
	}
}

void hoge(char* _operator, char* _operand, int is_left, int _register) {
	int const_num, addr, register_;

	register_ = get_unused_register();

	if (isDigit(_operand)){
		const_num = atoi(_operand);
		if (const_num < -32768 || 32768 < const_num) {
			sprintf(data_labels[label_counter].name, "label%d: data %d", label_counter, const_num);

			Output2("%s\tr%d, label%d\n", _operator,
				is_left ? register_ : _register,
				label_counter);
			label_counter++;
		}
		else {
			Output2("%si\tr%d, %d\n", _operator,
				is_left ? register_ : _register,
				const_num);
			register_state[register_] = 1;
		}
	}
	else {
		DebugOut("こっち");
    if(get_symbol(_operand, &symbol)){
      DebugOut2("serching success! %d, %d\n", symbol.addr, symbol.attr);
        Output2("%s\tr%d, %d%s\n", _operator, is_left ? register_ : _register,
        symbol.addr, additional_op[symbol.attr]);
    }
		else if (isRegister(_operand))
		{
			Output2("%sr\tr%d, r%d\n", _operator,
				_register, atoi(&_operand[3]));
		}
		else {
		addr = get_var_addr(_operand);
			Output2("%s\tr%d, %d\n", _operator,
				is_left ? register_ : _register,
				addr);
		}
		register_state[register_] = 1;
	}

	//if (isDigit(nodes[i].r)) {
	//	const_num = atoi(nodes[i].r);
	//	if (const_num < -32768 || 32768 < const_num) {
	//		sprintf(data_labels[label_counter].name, "label%d: data %d", label_counter, const_num);

	//		Output2("add\tr0, label%d\n", label_counter);
	//		label_counter++;
	//	}
	//	else
	//		Output2("addi\tr0,%d\n", atoi(nodes[i].r));
	//}
	//else
	//{
	//	addr = get_var_addr(nodes[i].r);
	//	Output2("add\tr0,%d\n", addr);
	//}
}

int translate(void){
  int i, j, k, left_var_flag;
  int addr;
  int register_ = 0;
  int const_num;
  memset(register_state, 0, sizeof(int) * 4);

  DebugOut("before change");
  maesyori();
  DebugOut("after change");


  // 式が１つしかない場合
  if(strcmp(nodes[0].op, "") == 0){
    if(isDigit(nodes[0].l)){
      // ここで数値が16bitで扱えるかチェックして、ダメならラベルに登録
      const_num = atoi(nodes[0].l);
      if(const_num < -32768 || 32768 < const_num){
        sprintf(data_labels[label_counter].name, "label%d: data %d", label_counter, const_num);

        Output2("load\tr0, label%d\n", label_counter);
        label_counter++;
      }
      // 普通の定数値
      else{
        Output2("loadi\tr0, %d\n", const_num);
      }
    }
    else{
        output_suitable_load_word(nodes[0].l, 0);
//      Output2("load\tr0, %d\n", get_var_addr(nodes[0].l));
    }
    return 0;
  }

  for (i = 0; i <= node_count; i++) {
	  DebugOut2("op:%s l:%s r:%s\n", nodes[i].op, nodes[i].l, nodes[i].r);
	  // 左にレジスタがあれば
	  if (isRegister(nodes[i].l))
	  {
      register_ = nodes[i].regi;
		  if (strcmp(nodes[i].op, "+") == 0) {
			  hoge("add", nodes[i].r, 0, nodes[i].regi);
		  }
		  else if (strcmp(nodes[i].op, "-") == 0) {
			  hoge("sub", nodes[i].r, 0, nodes[i].regi);
		  }
		  else if (strcmp(nodes[i].op, "*") == 0) {
			  hoge("mul", nodes[i].r, 0, nodes[i].regi);
		  }
		  else if (strcmp(nodes[i].op, "div") == 0) {
			  DebugOut("hoge-");
			  hoge("div", nodes[i].r, 0, nodes[i].regi);
		  }
		  continue;
	  }
	  // else if (isRegister(nodes[i].r)) {
		//   if (strcmp(nodes[i].op, "+") == 0) {
		// 	  hoge("add", nodes[i].l, 0, nodes[i].regi);
		//   }
		//   else if (strcmp(nodes[i].op, "-") == 0) {
		// 	  hoge("sub", nodes[i].l, 0, nodes[i].regi);
		//   }
		//   else if (strcmp(nodes[i].op, "*") == 0) {
		// 	  hoge("mul", nodes[i].l, 0, nodes[i].regi);
		//   }
		//   else if (strcmp(nodes[i].op, "div") == 0) {
		// 	  hoge("div", nodes[i].l, 0, nodes[i].regi);
		//   }
		//   continue;
	  // }


	  // 左辺を出力
	  hoge("load", nodes[i].l, 0, nodes[i].regi);
    register_ = nodes[i].regi;


	  if (strcmp(nodes[i].op, "+") == 0) {
		  hoge("add", nodes[i].r, 0, nodes[i].regi);
	  }
	  else if (strcmp(nodes[i].op, "-") == 0) {
		  hoge("sub", nodes[i].r, 0, nodes[i].regi);
	  }
	  else if (strcmp(nodes[i].op, "*") == 0) {
		  hoge("mul", nodes[i].r, 0, nodes[i].regi);
	  }
	  else if (strcmp(nodes[i].op, "div") == 0) {
		  hoge("div", nodes[i].r, 0, nodes[i].regi);
	  }

//	  Output2("store\tr0, %d\n", var_count + nodes[i].regi);
  }

  for (k = 0; k < 4; k++)
	  register_state[k] = 0;
//  Output2("load\tr0, %d\n", var_count + nodes[i-1].regi);

  DebugOut("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");

  return register_;//atoi(&nodes[i-1].regi);//nodes[i-1].regi;
}
