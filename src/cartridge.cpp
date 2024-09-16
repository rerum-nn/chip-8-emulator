#include "cartridge.h"

#include <cassert>
#include <fstream>

Cartridge::Cartridge(const std::string& path, const std::string& custom_name)
    : program_name_(custom_name) {
    std::ifstream file(path, std::ios::binary | std::ios::in);
    assert(file);
    char byte;
    while (file.get(byte)) {
        data_.push_back(byte);
    }

    if (program_name_.empty()) {
        size_t filename_pos = path.find_last_of("/\\");
        program_name_ = (filename_pos != std::string::npos) ? path.substr(filename_pos + 1) : path;
        size_t ext_pos = program_name_.find_last_of('.');
        program_name_ =
            (ext_pos != std::string::npos) ? program_name_.substr(0, ext_pos) : program_name_;
    }
}
const std::vector<uint8_t>& Cartridge::GetData() const {
    return data_;
}
const std::string& Cartridge::GetName() const {
    return program_name_;
}
