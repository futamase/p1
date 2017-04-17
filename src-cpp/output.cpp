#include "output.h"
#include "define.h"
#include <vector>
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
  DebugOut("adding");
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

void output_all(FILE* file){
  delete_waste_load();


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
