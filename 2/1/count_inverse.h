#ifndef INC_1__COUNT_INVERSE_H_
#define INC_1__COUNT_INVERSE_H_

#include <vector>
using Vint = std::vector<int>;
using CVecIt = Vint::const_iterator;
using VecIt = Vint::iterator;

size_t Merge(CVecIt it_1_beg, CVecIt it_1_end, CVecIt it_2_beg, CVecIt it_2_end,
             VecIt out_it);
size_t MergeSort(VecIt begin_it, VecIt end_it);
#endif    // INC_1__COUNT_INVERSE_H_
