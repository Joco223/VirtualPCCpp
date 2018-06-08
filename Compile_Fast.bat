del vPC.exe
g++ source/*.cpp -o vPC.exe -Iinclude -I../Libraries/SDL2/include -L../Libraries/SDL2/lib/x86 -I../Libraries/SDL2-NET/include -L../Libraries/SDL2-NET/lib/x86 -lSDL2 -lSDL2_net -static-libgcc -static-libstdc++ -O3
