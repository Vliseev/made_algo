#ifndef INC_2__PART_H_
#define INC_2__PART_H_
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>

/*
Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию
Partition реализуйте методом прохода двумя итераторами от конца массива к
началу.
*/

template <class It>
It Partition(It begin, It end, size_t pivot_id) {
    auto first_it = begin;
    auto second_it = begin;

    std::iter_swap(begin + pivot_id, end - 1);
    while (second_it != end - 1) {
        if (*second_it > *(end - 1))
            second_it++;
        else {
            std::iter_swap(first_it, second_it);
            first_it++;
            second_it++;
        }
    }
    std::iter_swap(first_it, end - 1);
    return first_it;
}

template <class It>
void Select(It begin, It end, size_t k, int seed = 123) {
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(seed);

    It l_it = begin;
    It r_it = end;
    It i_it;
    while (r_it > l_it) {
        std::uniform_int_distribution<> dis(0, std::distance(l_it, r_it) - 1);
        auto pivot_id = dis(gen);
        i_it = Partition(l_it, r_it, pivot_id);
        if (std::distance(begin, i_it) >= k)
            r_it = i_it;
        if (std::distance(begin, i_it) <= k)
            l_it = i_it + 1;
    }
}

#endif    // INC_2__PART_H_
