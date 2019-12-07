//
// Created by vlad on 01.12.2019.
//

#include "HuffmanEncoder.h"
#include <array>
#include <bitset>
#include <fstream>
#include <iostream>
#include <unordered_map>

/*!
 * @brief Build Code Tree for Canonical Huffman Code.
 * see Salomon
 * @param counter
 */

using SPair = std::pair<size_t, size_t>;
using Usmap = std::unordered_map<size_t, size_t>;
using CType = std::pair<size_t, Usmap>;
using CVector = std::vector<CType>;

CType MergeCoins(CType& c1, CType& c2) {
    auto w = c1.first + c2.first;
    Usmap d = c1.second;

    for (auto& [k, v] : c2.second) {
        if (!d.count(k))
            d[k] = 0;
        d[k] = std::max(d[k], v);
    }

    return {w, d};
}

void HuffmanEncoder::BuildTree(std::array<size_t, num_lit>& freq) {
    std::vector<size_t> counter;

    std::copy_if(std::begin(freq), std::end(freq), std::back_inserter(counter),
                 [](size_t val) { return val > 0; });

    size_t size = counter.size();
    std::vector<std::pair<size_t, size_t>> sorted_c;
    for (size_t idx = 0; idx < size; ++idx) {
        sorted_c.emplace_back(counter[idx], idx);
    }

    std::sort(std::begin(sorted_c), std::end(sorted_c),
              [](SPair f, SPair s) { return f.first < s.first; });

    CVector coins;

    for (size_t level = MAX_BITS - 1; level > 0; level--) {
        CVector new_coin, prev_coins;
        for (auto& [w, i] : sorted_c) {
            new_coin.emplace_back(
                w, std::unordered_map<size_t, size_t>{{i, level}});
        }

        for (int i = 0; i < coins.size() / 2; ++i) {
            prev_coins.push_back(MergeCoins(coins[2 * i], coins[2 * i + 1]));
        }
        coins.clear();
        std::merge(std::begin(prev_coins), std::end(prev_coins),
                   std::begin(new_coin), std::end(new_coin),
                   std::back_inserter(coins),
                   [](const CType& c1, const CType& c2) {
                       return c1.first < c2.first;
                   });
    }
    std::vector<int> res(counter.size());

    for (int i = 0; i < counter.size() * 2 - 2; ++i) {
        for (auto& [k, v] : coins[i].second) {
            if (res[k] < v)
                res[k] = v;
        }
    }

    table_.lengths_.resize(num_lit);

    for (int idx = 0, num = 0; idx < num_lit; ++idx) {
        if (freq[idx] > 0) {
            table_.lengths_[idx] = res[num++];
        } else {
            table_.lengths_[idx] = 0;
        }
    }
}

// void HuffmanEncoder::BuildTree(std::array<size_t, num_lit>& counter) {
//    size_t n_1, n_2;
//
//    std::vector<size_t> freq;
//    std::copy_if(std::begin(counter), std::end(counter),
//                 std::back_inserter(freq), [](size_t val) { return val > 0;
//                 });
//
//    size_t size = freq.size();
//
//    std::vector<size_t> heap(size * 2);
//    for (size_t i = 0; i < size; ++i) {
//        heap[i] = i + size;
//        heap[size + i] = freq[i];
//    }
//
//    auto heap_cmp = [size, &heap](size_t i, size_t j) {
//        return heap[i] > heap[j];
//    };
//
//    std::make_heap(heap.begin(), heap.begin() + size, heap_cmp);
//    for (size_t idx = 0; idx < size - 1; ++idx) {
//        n_1 = heap[0];
//        std::pop_heap(heap.begin(), heap.begin() + (size - idx), heap_cmp);
//        n_2 = heap[0];
//        std::pop_heap(heap.begin(), heap.begin() + (size - idx - 1),
//        heap_cmp);
//
//        heap[size - idx - 1] = heap[n_1] + heap[n_2];
//        heap[n_1] = heap[n_2] = size - idx - 1;
//        heap[size - idx - 2] = size - idx - 1;
//
//        std::push_heap(heap.begin(), heap.begin() + (size - idx - 1),
//        heap_cmp);
//    }
//
//    table_.lengths_.resize(num_lit);
//
//    for (size_t id = 0, num = 0; id < num_lit; ++id) {
//        if (counter[id] > 0) {
//            int len = 0;
//            size_t idx = num + size;
//            while (idx != 1) {
//                idx = heap[idx];
//                len++;
//            }
//            table_.lengths_[id] = len;
//            num++;
//        } else {
//            table_.lengths_[id] = 0;
//        }
//    }
//}

void HuffmanEncoder::BuildTable() {
    int len_count[MAX_BITS]{};
    uint32_t next_code[MAX_BITS]{};

    table_.codes_.resize(table_.lengths_.size());

    std::for_each(std::begin(table_.lengths_), std::end(table_.lengths_),
                  [&len_count](int len) { len_count[len]++; });

    uint32_t code = 0;

    for (int bits = 1; bits < MAX_BITS; ++bits) {
        if (bits > 1)
            code = (code + len_count[bits - 1]) << 1;
        next_code[bits] = code;
    }

    for (int n = 0; n < table_.lengths_.size(); ++n) {
        auto len = table_.lengths_[n];
        if (len) {
            table_.codes_[n] = next_code[len]++;
        }
    }
}
void HuffmanEncoder::Compress() {
    byte cur_byte;
    while (istream_.Read(cur_byte)) {
        data_.push_back(cur_byte);
    }
    std::array<size_t, num_lit> counter{};

    std::for_each(std::begin(data_), std::end(data_),
                  [&counter](byte val) { counter[val]++; });

    BuildTree(counter);
    BuildTable();

    auto unp_size = GetCompLen(table_, counter);
    if (unp_size > data_.size() + 1) {
        // WriteRawFile
        bit_ostream_.PutByte(0);
        for (unsigned char sym : data_) {
            bit_ostream_.PutByte(sym);
        }
        return;
    }

    WriteHeader(table_, data_.size());

    std::ofstream off("comp");

    int idx = 0;
    for (unsigned char sym : data_) {
        off << table_.codes_[sym] << " " << table_.lengths_[sym] << std::endl;
        bit_ostream_.SendBits(table_.codes_[sym], table_.lengths_[sym]);
        idx++;
    }
    bit_ostream_.Flush();
}

/*!
 * @brief return compressed file in bytes
 * @param table
 * @param counter
 * @return size in bytes
 */
size_t HuffmanEncoder::GetCompLen(const HuffmanEncoder::EncodeTable& table,
                                  const std::array<size_t, num_lit>& counter) {
    const size_t CompFlagSize = 1;
    const size_t FileSize = 4 * 8;
    const size_t HeaderSize = CompFlagSize + 4 * num_lit + FileSize;

    auto size = HeaderSize;

    for (int i = 0; i < num_lit; ++i) {
        size += table.lengths_[i] * counter[i];
    }

    return (size + 8 - 1) / 8;
}

/*!
 * @brief Write Archive Header
 * @param table
 * @param fsize
 */
void HuffmanEncoder::WriteHeader(const EncodeTable& table, uint32_t fsize) {
    bit_ostream_.SendBits(1, 1);    // File is compressed
    bit_ostream_.PutWord(fsize);

    for (int idx = 0; idx < num_lit; ++idx) {
        bit_ostream_.SendBits(table.lengths_[idx] & 0xf, 4);
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

void HuffmanEncoder::BitStreamWriter::PutByte(std::uint8_t c) {
    ostream_.Write(c);
}
void HuffmanEncoder::BitStreamWriter::PutShort(std::uint16_t w) {
    PutByte(w >> 8);
    PutByte(w & 0xff);
}

void HuffmanEncoder::BitStreamWriter::PutWord(std::uint32_t w) {
    SendBits(w >> 24, 8);
    SendBits((w >> 16) & 0xff, 8);
    SendBits((w >> 8) & 0xff, 8);
    SendBits(w & 0xff, 8);
}

void HuffmanEncoder::BitStreamWriter::Flush() {
    PutShort(bit_buf_);
    bit_buf_ = 0;
    cur_size_ = 0;
}
void HuffmanEncoder::BitStreamWriter::SendBits(uint32_t value, int length) {
    if (length + cur_size_ > acc_size_) {
        bit_buf_ |= (std::uint16_t)value >> (length - (acc_size_ - cur_size_));
        PutShort(bit_buf_);
        bit_buf_ = (std::uint16_t)value << (2 * acc_size_ - cur_size_ - length);
        cur_size_ = length - (acc_size_ - cur_size_);
    } else {
        bit_buf_ |= (std::uint16_t)(value) << (acc_size_ - cur_size_ - length);
        cur_size_ += length;
    }
}

HuffmanDecoder::BitStreamReader::BitStreamReader(IInputStream& istream)
    : istream_(istream) {
    istream.Read(w1);
    istream.Read(w2);
    istream.Read(w3);
}
void HuffmanDecoder::BitStreamReader::Addbits(uint bits) {
    bits += in_bit_;
    while (bits >= 8) {
        w1 = w2;
        w2 = w3;
        w3 = 0;
        istream_.Read(w3);
        bits -= 8;
    }
    in_bit_ = bits & 7;
}
uint HuffmanDecoder::BitStreamReader::Getbits() {
    uint32_t BitField = (uint32_t)w1 << 16;
    BitField |= (uint32_t)w2 << 8;
    BitField |= (uint32_t)w3;
    BitField >>= (8 - in_bit_);
    return BitField & 0xffff;
}

void HuffmanDecoder::Decompress() {
    // ReadTable;
    // BuildTable;
    // GetSize

    auto unp_size = 0;
    auto file_size = ReadHeader();
    if (file_size == 0) {
        uint8_t val;
        while (bit_istream_.istream_.Read(val))
            ostream.Write(val);
        //// TODO: RawRead
        return;
    }

    auto unp_lens = ReadTable();
    BuildTable(unp_lens);

    unp_size = 0;

    std::ofstream off("dec");

    while (unp_size < file_size) {
        uint16_t val;
        val = bit_istream_.Getbits() & 0xfffe;

        uint Bits = 15;
        for (uint I = 1; I < 15; I++)
            if (val < table.decode_len_[I]) {
                Bits = I;
                break;
            }

        bit_istream_.Addbits(Bits);

        uint Dist = val - table.decode_len_[Bits - 1];

        Dist >>= (16 - Bits);

        uint Pos = table.decode_pos_[Bits] + Dist;

        auto code = table.decode_num_[Pos];
        ostream.Write(code);

        off << (val >> (16 - Bits)) << " " << Bits << std::endl;

        unp_size++;
    }
    std::cout << "Ok\n\n";
}
size_t HuffmanDecoder::ReadHeader() {
    int bit = bit_istream_.Getbits() & 0x8000;
    if (bit == 0) {
        bit_istream_.Addbits(8);
        ostream.Write(bit_istream_.w1);
        ostream.Write(bit_istream_.w2);
        ostream.Write(bit_istream_.w3);
        return 0;
    } else {
        bit_istream_.Addbits(1);
        uint32_t fsize = 0;
        fsize = ((bit_istream_.Getbits() & 0xff00) >> 8) << 24;
        bit_istream_.Addbits(8);
        fsize |= ((bit_istream_.Getbits() & 0xff00) >> 8) << 16;
        bit_istream_.Addbits(8);
        fsize |= ((bit_istream_.Getbits() & 0xff00) >> 8) << 8;
        bit_istream_.Addbits(8);
        fsize |= ((bit_istream_.Getbits() & 0xff00) >> 8);
        bit_istream_.Addbits(8);
        return fsize;
    }
}

std::vector<int> HuffmanDecoder::ReadTable() {
    std::vector<int> lens;
    for (int i = 0; i < num_lit; ++i) {
        uint32_t len = (bit_istream_.Getbits() >> 12) & 0xf;
        lens.push_back(len);
        bit_istream_.Addbits(4);
    }

    return lens;
}