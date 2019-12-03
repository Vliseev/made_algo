//
// Created by vlad on 01.12.2019.
//
#pragma once
#include <utility>
#include <vector>

typedef unsigned char byte;
struct IInputStream {
    int index=0;
    explicit IInputStream(std::vector<byte> buffer)
        : buffer(std::move(buffer)) {
    }
    std::vector<byte> buffer;
    // Возвращает false, если поток закончился
    bool Read(byte& value);
};
struct IOutputStream {
    std::vector<byte> buffer{};
    void Write(byte value);
};
