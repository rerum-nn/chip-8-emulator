#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Cartridge {
public:
    Cartridge(const std::string& path);

    const std::vector<uint8_t>& GetData() const;

private:
    std::vector<uint8_t> data_;
};
