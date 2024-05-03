#include <string>
#include <iostream>
#include <cstring>

#include "kernel.h"

int main(int argc, char* argv[]) {
    std::string filename;
    size_t hertz = 2000;
    size_t scale = 16;
    size_t i = 1;
    while (i < argc) {
        if (std::strcmp(argv[i], "-h") == 0) {
            ++i;
            hertz = atoi(argv[i]);
            if (hertz == 0) {
                std::cout << "The problem with reading the frequency from the command line\n";
                return 1;
            }
        } else if (std::strcmp(argv[i], "-s") == 0) {
            ++i;
            scale = atoi(argv[i]);
            if (scale == 0) {
                std::cout << "the problem with reading the scale from the command line\n";
                return 1;
            }
        } else if (filename.empty()) {
            filename = argv[i];
        } else {
            std::cout << "Unexpected command line option\n";
            return 1;
        }
        ++i;
    }

    if (filename.empty()) {
        std::cout << "./chip-8-emulator /path/to/program.ch8\n";
        return 1;
    }

    Cartridge demo(filename);
    ChipKernel chip8(hertz);
    chip8.LoadProgram(demo);

    chip8.Run();

    return 0;
}
