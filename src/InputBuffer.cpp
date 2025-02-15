#include "InputBuffer.hpp"
#include <iostream>

void InputBuffer::readInput() {
    std::getline(std::cin, buffer);
}

const std::string& InputBuffer::getBuffer() const {
    return buffer;
}