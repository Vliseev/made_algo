#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "HuffmanEncoder.h"

void Test1() {
    std::ifstream iff("test", std::ios::binary);
    if (!iff) {
        std::cerr << "cant open file\n";
        return;
    }

    iff.seekg(0, std::ios::end);
    size_t size = iff.tellg();
    iff.seekg(0, std::ios::beg);

    std::vector<byte> data_(size);

    iff.read(reinterpret_cast<char *>(data_.data()), size);

    IInputStream iss(data_);
    IOutputStream oss;

    HuffmanEncoder h_enc(iss, oss);
    h_enc.Compress();

    IInputStream iss_dec(oss.buffer);
    IOutputStream oss_dec;

    HuffmanDecoder h_dec(iss_dec, oss_dec);

    h_dec.Decompress();

    bool flag = true;
    for (int kI = 0; kI < data_.size(); ++kI) {
        if (data_[kI] != oss_dec.buffer[kI]) {
            std::cout << kI << " " << std::hex << (int)data_[kI] << " "
                      << (int)oss_dec.buffer[kI] << std::dec << "\n";
            flag = false;
        }
    }
    std::cout << "flag " << flag << std::endl;
}

int main() {
    try {
        Test1();
    } catch (std::exception &e) {
        std::cerr << e.what();
    }
    return 0;
}