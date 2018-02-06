del vPC.exe
g++ source/*.cpp -o vPC.exe -Iinclude -I../Libraries/SDL2/include -L../Libraries/SDL2/lib/x86 -lSDL2 -O3