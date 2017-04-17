#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <array>
#include <string>

const std::string get_additional_op(int index);

/// @return 最終的な結果が入るレジスタの番号
int translate(void);

#endif
