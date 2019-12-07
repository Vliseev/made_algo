//
// Created by vlad on 01.12.2019.
//

#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>
#include "Stream.h"

static const int num_lit = 256;
namespace {
constexpr int MAX_BITS = 16;
}

class HuffmanEncoder {
   public:
    struct EncodeTable {
        std::vector<int> lengths_;
        std::vector<uint32_t> codes_;
    } table_;

    struct BitStreamWriter {
        static constexpr int acc_size_ = 16;
        int cur_size_ = 0;
        uint16_t bit_buf_ = 0;
        IOutputStream& ostream_;

        explicit BitStreamWriter(IOutputStream& ostream) : ostream_(ostream){};

        void PutByte(std::uint8_t c);

        void PutShort(std::uint16_t w);

        void PutWord(std::uint32_t w);

        void Flush();

        void SendBits(uint32_t value, int length);
    } bit_ostream_;

    std::vector<uint8_t> data_;

    IInputStream& istream_;
    HuffmanEncoder(IInputStream& iss, IOutputStream& oss)
        : istream_(iss), bit_ostream_(oss){};

    static size_t GetCompLen(const EncodeTable&,
                             const std::array<size_t, num_lit>&);
    void WriteHeader(const EncodeTable&, uint32_t);

   public:
    void BuildTree(std::array<size_t, num_lit>& counter);
    void BuildTable();

    void Compress();
};

class HuffmanDecoder {
   public:
    struct DecodeTable {
        std::vector<int> decode_num_;

        std::array<uint32_t, MAX_BITS> decode_len_;
        std::array<uint32_t, MAX_BITS> decode_pos_;
    };

    struct BitStreamReader {
        int in_bit_{};    // Current bit position in the current byte.

        uint8_t w1{};
        uint8_t w2{};
        uint8_t w3{};

        IInputStream& istream_;
        std::vector<uint8_t> InBuf;

        explicit BitStreamReader(IInputStream& istream);
        void Addbits(uint bits);
        uint Getbits();
    } bit_istream_;
    IOutputStream& ostream;
    DecodeTable table;

    HuffmanDecoder(IInputStream& iss, IOutputStream& oss)
        : bit_istream_(iss), ostream(oss){};

    std::vector<int> ReadTable();
    void BuildTable(std::vector<int>& lens);
    size_t ReadHeader();
    void Decompress();
};
