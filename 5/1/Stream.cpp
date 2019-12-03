//
// Created by vlad on 01.12.2019.
//

#include "Stream.h"
bool IInputStream::Read(byte& value) {
    if (index >= buffer.size()) {
        return false;
    } else {
        value = buffer[index++];
        return true;
    }
}
void IOutputStream::Write(byte value) {
    buffer.push_back(value);
}
