#include "Chip8.h"
#include "Graphics.h"
#include <chrono>
#include <iostream>
#include <cassert>
#include <emscripten.h>

    Graphics display{ "CHIP-8 Emulator", 64, 32 };
    Chip8 chip8;
    int pitch;
    int cycles;
    

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void loadRom(char* rompath, int frequency) {
        
        chip8.clearAll();
        cycles = frequency;
        chip8.load(rompath);
    }
    EMSCRIPTEN_KEEPALIVE
    void stop() {
        emscripten_cancel_main_loop();

        chip8.clearAll();
        display.update(chip8.video, sizeof(chip8.video[0]) * 64);
    }
}

void main_loop() {
    
    display.handleInput(chip8.input);

    for (int i = 0; i < cycles; i++) {
        chip8.Cycle();
    }
    pitch = sizeof(chip8.video[0]) * 64;
    display.update(chip8.video, pitch);
}

int main() {
    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}
