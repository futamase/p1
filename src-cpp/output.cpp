#include "output.h"
#include "define.h"
#include <vector>
#include <unordered_map>
#include <iostream>

struct Command{
  std::string opecode;
  std::string operand_l;
  std::string operand_r;
  Command(const std::string& op, const std::string& l, const std::string& r)
    : opecode(op), operand_l(l), operand_r(r)
  {}
};

static std::vector<Command> command_list;

void add_op(const std::string& opecode, const std::string& operand_l, const std::string& operand_r)
{
  command_list.emplace_back(opecode, operand_l, operand_r);
}

static void delete_waste_load(){
  for(auto ite = command_list.begin(); ite != command_list.end()-1; ++ite){
    std::cout << ite->opecode << " " << ite->operand_l << " " << ite->operand_r <<std::endl;
    if(ite->operand_l == (ite+1)->operand_l &&
      ite->operand_r == (ite+1)->operand_r &&
      ite->opecode == "store" && (ite+1)->opecode == "load")
    {
      DebugOut("まじかよ命令消してるぜあいつ");
        ite = command_list.erase(ite+1);
    }
  }
      DebugOut("まじかよ命令消しおわったぜ");
}


void erase_waste_store(const std::string& regi){
  for(auto ite = command_list.begin(); ite != command_list.end(); ++ite){
    if(ite->opecode == "store" && ite->operand_r == regi){
      command_list.erase(ite);
      return; // 消すのはひとつだけ
    }
  }
}

// storeが一回しかされていないものは
// store されてもloadがないものはいらない
void replace_something(){
  std::unordered_map<std::string, int> var_store_list;
  std::unordered_map<std::string, int> var_load_list;

  for(int i = 0; i < command_list.size(); i++){
    if(command_list.at(i).opecode == "store"){
      var_store_list[command_list.at(i).operand_r]++;
    }
    else if(command_list.at(i).opecode == "load"){
      var_load_list[command_list.at(i).operand_r]++;
    }
  }

  for(auto&& i : var_store_list){
      if(var_load_list.find(i.first) == var_load_list.end()){
        erase_waste_store(i.first);
      }
  }


  std::cout << "###########################" <<std::endl;
  for(auto&& i : var_store_list){
    std::cout << i.first << " " << i.second  << std::endl;
  }
  std::cout << "###########################" <<std::endl;
  for(auto&& i : var_load_list){
    std::cout << i.first << " " << i.second  << std::endl;
  }
  std::cout << "###########################" <<std::endl;
}

void output_all(FILE* file){
  delete_waste_load();

//  replace_something();

  for(auto&& command : command_list){
    fprintf(file, "%s", command.opecode.c_str());

    if(command.operand_l.empty()){
      fprintf(file, "\n");
      continue;
    }

    fprintf(file, "\t%s", command.operand_l.c_str());

    if(command.operand_r.empty()){
      fprintf(file, "\n");
      continue;
    }

    fprintf(file, ", %s\n", command.operand_r.c_str());
  }
}
