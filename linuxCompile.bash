#!/bin/bash
echo -e "This script depends on the SDL2 and SDL2_net development packages to work. Please install these before\nrunning this script."
echo -e "\nRun the compiled executable with ./vPC and put in 0.0.0.0 to skip connecting to a vPC server."
rm vPC
g++ source/*.cpp -o vPC -lSDL2 -lSDL2_net -static-libgcc -static-libstdc++ -Iinclude -I../Libraries/SDL2/include -L../Libraries/SDL2/lib/x86 -I../Libraries/SDL2-NET/include -L../Libraries/SDL2-NET/lib/x86 -lpthread
