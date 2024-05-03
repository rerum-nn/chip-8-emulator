#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Cartridge {
public:
    Cartridge(const std::string& path, const std::string& custom_name = "");

    const std::vector<uint8_t>& GetData() const;
    const std::string& GetName() const;

private:
    std::vector<uint8_t> data_;
    std::string program_name_;
};
