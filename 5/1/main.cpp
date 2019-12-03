#include <algorithm>
#include <iostream>
#include <vector>
#include "HuffmanEncoder.h"

/*

for code,size in l:
    print(f"{code:0{size}b}, {size}")
*/

struct BitStreamReader {
    int in_bit_{};    // Current bit position in the current byte.

    uint8_t w1{};
    uint8_t w2{};

    IInputStream& istream_;
    std::vector<uint8_t> InBuf;

    explicit BitStreamReader(IInputStream& istream) : istream_(istream) {
        istream.Read(w1);
        istream.Read(w2);
    };

    void Addbits(uint bits) {
        bits += in_bit_;
        if (bits >= 8) {
            w1 = w2;
            istream_.Read(w2);
        }
        in_bit_ = bits & 7;
    }

    // Return 16 bits from current position in the buffer.
    // Bit at (in_addr_,in_bit_) has the highest position in returning data.
    uint Getbits() {
        uint16_t BitField = w1 << 8;
        BitField |= w2;
        BitField <<= in_bit_;
        return BitField & 0xffff;
    }
};

void Test1() {
    std::vector<int> vi = {3, 3, 3, 3, 3, 2, 4, 4};
    HuffmanEncoder huffman_encoder;
    IOutputStream oss;
    HuffmanEncoder::BitStreamWriter boss(oss);
    HuffmanDecoder decoder;
    decoder.BuildTable(vi);

    huffman_encoder.BuildTable(vi);

    const auto& table = huffman_encoder.table;

    for (int i = 0; i < 8; i++) {
        std::cout << "(" << table.codes_[i] << "," << table.lengths_[i]
                  << "),\n";

        boss.send_bits(table.codes_[i], table.lengths_[i]);
    }

    boss.Flush();

    IInputStream iss(oss.buffer);
    BitStreamReader bis(iss);

    const auto& d_table = decoder.table;

    for (int num_dec = 0; num_dec < 8; ++num_dec) {
        uint16_t val;
        val = bis.Getbits() & 0xfffe;

        uint Bits = 15;
        for (uint I = 1; I < 15; I++)
            if (val < d_table.decode_len_[I]) {
                Bits = I;
                break;
            }

        bis.Addbits(Bits);

        // Calculate the distance from the start code for current bit length.
        uint Dist = val - d_table.decode_len_[Bits - 1];

        // Start codes are left aligned, but we need the normal right aligned
        // number. So we shift the distance to the right.
        Dist >>= (16 - Bits);

        // Now we can calculate the position in the code list. It is the sum
        // of first position for current bit length and right aligned distance
        // between our bit field and start code for current bit length.
        uint Pos =
            d_table.decode_pos_[Bits] + Dist;    // Dec->DecodePos[Bits]+Dist;

        // Out of bounds safety check required for damaged archives.

        // Convert the position in the code list to position in alphabet
        // and return it.
        auto code = d_table.decode_num_[Pos];

        std::cout << code << std::endl;
    }
}

int main() {
    Test1();
    return 0;
}