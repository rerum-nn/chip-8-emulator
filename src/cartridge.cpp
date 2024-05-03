#include "cartridge.h"

#include <cassert>
#include <fstream>

Cartridge::Cartridge(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::in);
    assert(file);
    char byte;
    while (file.get(byte)) {
        data_.push_back(byte);
    }
}
const std::vector<uint8_t>& Cartridge::GetData() const {
    return data_;
}
