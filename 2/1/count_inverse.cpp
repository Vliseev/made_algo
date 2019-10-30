#include "count_inverse.h"
#include <algorithm>
#include <cassert>
#include <utility>

size_t Merge(CVecIt it_1_beg, CVecIt it_1_end, CVecIt it_2_beg, CVecIt it_2_end,
             VecIt out_it) {
    size_t num_inverse = 0;
    CVecIt mid_id = it_2_beg;

    while (it_1_beg != it_1_end && it_2_beg != it_2_end) {
        if (*it_1_beg <= *it_2_beg) {
            *(out_it++) = *(it_1_beg++);
        } else {
            *(out_it++) = *(it_2_beg++);
            num_inverse += std::distance(it_1_beg, mid_id);
        }
    }

    while (it_1_beg != it_1_end)
        *(out_it++) = *(it_1_beg++);
    while (it_2_beg != it_2_end)
        *(out_it++) = *(it_2_beg++);
    return num_inverse;
}

size_t MergeSort(VecIt begin_it, VecIt end_it) {
    using std::min;
    size_t num_inverse = 0;
    assert(begin_it <= end_it);

    size_t size = distance(begin_it, end_it);
    auto tmp_vec = Vint(size);

    for (size_t cur_size = 1; cur_size < size; cur_size *= 2) {
        for (size_t l_b = 0; l_b < size; l_b += 2 * cur_size) {
            size_t m = min(l_b + cur_size, size);
            size_t r = min(l_b + 2 * cur_size, size);

            num_inverse += Merge(begin_it + l_b, begin_it + m, begin_it + m,
                                 begin_it + r, tmp_vec.begin());
            auto size_part = r - l_b;
            move(tmp_vec.begin(), tmp_vec.begin() + size_part, begin_it + l_b);
        }
    }
    return num_inverse;
}
