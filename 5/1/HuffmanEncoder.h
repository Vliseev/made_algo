//
// Created by vlad on 01.12.2019.
//

#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>
#include "Stream.h"

namespace {
constexpr int MAX_BITS = 16;
}

class HuffmanEncoder {
   public:
    struct EncodeTable {
        std::vector<int> lengths_;
        std::vector<uint32_t> codes_;
    } table;

    struct BitStreamWriter {
        static constexpr int acc_size_ = 16;
        int cur_size_ = 0;
        uint16_t bit_buf_ = 0;
        IOutputStream& ostream_;

        explicit BitStreamWriter(IOutputStream& ostream) : ostream_(ostream){};

        void PutByte(std::uint8_t c) {
            ostream_.Write(c);
        }

        void PutShort(std::uint16_t w) {
            PutByte(w >> 8);
            PutByte(w & 0xff);
        }

        void Flush() {
            PutShort(bit_buf_);
            bit_buf_ = 0;
            cur_size_ = 0;
        }

        void send_bits(uint32_t value, int length) {
            /* if (length + cur_size_ > acc_size_) {
                 bit_buf_ |= (std::uint16_t)value << cur_size_;
                 PutShort(bit_buf_);
                 bit_buf_ = (std::uint16_t)value >> (acc_size_ - cur_size_);
                 cur_size_ += length - acc_size_;
             } else {
                 bit_buf_ |= (std::uint16_t)(value) << cur_size_;
                 cur_size_ += length;
             }*/

            if (length + cur_size_ > acc_size_) {
                bit_buf_ |=
                    (std::uint16_t)value >> (length - (acc_size_ - cur_size_));
                PutShort(bit_buf_);
                bit_buf_ = (std::uint16_t)value
                           << (2 * acc_size_ - cur_size_ - length);
                cur_size_ = length - (acc_size_ - cur_size_);
            } else {
                bit_buf_ |= (std::uint16_t)(value)
                            << (acc_size_ - cur_size_ - length);
                cur_size_ += length;
            }
        }
    };

    static uint32_t BitReverse(uint32_t code, int len) {
        uint32_t res = 0;
        do {
            res |= code & 1;
            code >>= 1;
            res <<= 1;
        } while (--len > 0);
        return res >> 1;
    }

   public:
    void BuildTree(std::vector<size_t>);
    void BuildTable(std::vector<int>& lens);
};

class HuffmanDecoder {
   public:
    struct DecodeTable {
        std::vector<int> decode_num_;

        std::array<uint32_t, MAX_BITS> decode_len_;
        std::array<uint32_t, MAX_BITS> decode_pos_;
    };
    DecodeTable table;
    void BuildTable(std::vector<int>& lens);
};
