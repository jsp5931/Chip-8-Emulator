# Chip-8-Emulator
This is a Chip-8 emulator written in C++ and ported to the web using Emscripten. 
In order to to use this emulator, you may clone the repository and run a local host in the web folder.
Building it from scratch requires downloading Emscripten (https://emscripten.org/docs/getting_started/downloads.html)
Once Emscripten is set up, open up the emcmdprompt in your emsdk folder.
Navigate to the repository and enter the following commands:

emcmake cmake -S src -B build

cd build 

emmake make

After this you can run a local server in the build folder to use the emulator
