#include "kernel.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 0;
    }
    Cartridge demo(argv[1]);
    ChipKernel chip8;
    chip8.LoadProgram(demo);

    chip8.Run();

    return 0;
}
