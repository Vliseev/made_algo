//
// Created by vlad on 01.12.2019.
//

#include "HuffmanEncoder.h"
#include <iostream>

void HuffmanEncoder::BuildTree(std::vector<size_t> freq) {
    size_t n_1, n_2;
    size_t size = freq.size();
    /* std::vector<int> heap(size * 2);
     for (int i = 0; i < size; ++i) {
         heap[i] = i + size + 1;
         heap[size + i] = freq[i];
     }

     auto heap_cmp = [size, &heap](const int& i, const int& j) {
         return heap[i - 1] > heap[j - 1];
     };

     std::make_heap(heap.begin(), heap.begin() + size, heap_cmp);
     for (int idx = 0; idx < size - 1; ++idx) {
         n_1 = heap[0];
         std::pop_heap(heap.begin(), heap.begin() + (size - idx), heap_cmp);
         n_2 = heap[0];
         std::pop_heap(heap.begin(), heap.begin() + (size - idx - 1), heap_cmp);

         heap[size - idx - 1] = heap[n_1 - 1] + heap[n_2 - 1];
         heap[n_1 - 1] = heap[n_2 - 1] = size - idx;
         heap[size - idx - 2] = size - idx;

         std::push_heap(heap.begin(), heap.begin() + (size - idx - 1),
     heap_cmp);
     }*/

    std::vector<size_t> heap(size * 2);
    for (size_t i = 0; i < size; ++i) {
        heap[i] = i + size;
        heap[size + i] = freq[i];
    }

    auto heap_cmp = [size, &heap](size_t i, size_t j) {
        return heap[i] > heap[j];
    };

    std::make_heap(heap.begin(), heap.begin() + size, heap_cmp);
    for (size_t idx = 0; idx < size - 1; ++idx) {
        n_1 = heap[0];
        std::pop_heap(heap.begin(), heap.begin() + (size - idx), heap_cmp);
        n_2 = heap[0];
        std::pop_heap(heap.begin(), heap.begin() + (size - idx - 1), heap_cmp);

        heap[size - idx - 1] = heap[n_1] + heap[n_2];
        heap[n_1] = heap[n_2] = size - idx - 1;
        heap[size - idx - 2] = size - idx - 1;

        std::push_heap(heap.begin(), heap.begin() + (size - idx - 1), heap_cmp);
    }

    for (size_t num = 0; num < size; ++num) {
        int len = 0;
        size_t idx = num + size;
        while (idx != 1) {
            idx = heap[idx];
            len++;
        }
        table.lengths_[num] = len;
    }
}
void HuffmanEncoder::BuildTable(std::vector<int>& lens) {
    int len_count[MAX_BITS]{};
    uint32_t next_code[MAX_BITS]{};

    table.lengths_ = lens;    // TODO: delete

    table.codes_.resize(table.lengths_.size());

    std::for_each(std::begin(table.lengths_), std::end(table.lengths_),
                  [&len_count](int len) { len_count[len]++; });

    uint32_t code = 0;

    for (int bits = 1; bits < MAX_BITS; ++bits) {
        code = (code + len_count[bits - 1]) << 1;
        next_code[bits] = code;
    }

    for (int n = 0; n < table.lengths_.size(); ++n) {
        auto len = table.lengths_[n];
        if (len) {
            table.codes_[n] =
                next_code[len]++;    // BitReverse(next_code[len]++, len);
        }
    }
}
void HuffmanDecoder::BuildTable(std::vector<int>& lens) {
    uint32_t code = 0;

    int len_count[MAX_BITS]{};
    uint32_t next_code[MAX_BITS]{};

    std::for_each(std::begin(lens), std::end(lens),
                  [&len_count](int len) { len_count[len]++; });

    len_count[0] = 0;
    table.decode_pos_[0] = 0;
    table.decode_len_[0] = 0;
    uint32_t upper_limit = 0;

    for (int bits = 1; bits < MAX_BITS; bits++) {
        upper_limit += len_count[bits];

        uint32_t left_aligment = upper_limit << (MAX_BITS - bits);
        upper_limit *= 2;

        table.decode_len_[bits] = left_aligment;
        table.decode_pos_[bits] =
            table.decode_pos_[bits - 1] + len_count[bits - 1];
    }

    std::array<uint32_t, MAX_BITS> tmp_decode_pos{};
    std::copy(std::begin(table.decode_pos_), std::end(table.decode_pos_),
              std::begin(tmp_decode_pos));

    table.decode_num_.resize(lens.size());
    for (int I = 0; I < lens.size(); ++I) {
        uint8_t cur_bit_len = lens[I];
        if (cur_bit_len) {
            uint32_t las_pos = tmp_decode_pos[cur_bit_len];
            table.decode_num_[las_pos] = I;
            tmp_decode_pos[cur_bit_len]++;
        }
    }
}
