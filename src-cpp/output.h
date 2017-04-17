#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>

void add_op(const std::string& opecode, const std::string& operand_l, const std::string& operand_r);

void output_all(FILE* outfile);

#endif
