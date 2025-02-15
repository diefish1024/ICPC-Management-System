#ifndef INPUT_BUFFER_HPP
#define INPUT_BUFFER_HPP

#include <string>

class InputBuffer {
private:
    std::string buffer;
public:
    void readInput();
    const std::string& getBuffer() const;
};

#endif // INPUT_BUFFER_HPP