#ifdef _WIN32
#include <SDL.h>
#endif

#ifdef __linux
#include <SDL2/SDL.h>
#endif

#include <vector>
#include <iostream>
#include <string>

#include "NSSDL.h"
#include "SDLWindow.h"
#include "Memory.h"
#include "CPU.h"
#include "PC.h"
#include "GPU.h"
#include "Assembly.h"
#include "GPUAssembly.h"

int width = 320;
int height = 240;

bool quit = false;

typedef unsigned char byte;

int main(int argc, char* argv[]) {
	SDL_Event event;

	SDL_Window* window2;
	SDL_Surface* surface2;
	SDL_Surface* pixelSpace2;
	SDL_Renderer* renderer2;
	SDL_Texture* texture2;
	unsigned int* pixels2;

	SDLWindow pc1W(window2, surface2, pixelSpace2, renderer2, texture2, pixels2);

	Memory ram1(64000);
	Memory hdd1(128 * 128);

	GPU gpu1(4096, 64000, 256, 16384, &pc1W, ram1);

	NSSDL::initSDL(gpu1.screen, width, height);

	CPU cpu1(128, 128, ram1, hdd1, gpu1);

	std::vector<std::string> code;
	std::vector<std::string> gpu_code;

	std::vector<int> vValues;
	std::vector<std::string> vNames;
	int addMem = 0;

	Assembly::readFile("Program.txt", code);

	Assembly::Compile(code, cpu1, vValues, vNames, addMem);

	GPUAssembly::readFile("GPU_Program.txt", gpu_code);

	GPUAssembly::Compile(gpu_code, gpu1, vValues, vNames, addMem);

	//std::cout << (int)gpu1.vRam.memory[0] << " " << (int)gpu1.vRam.memory[1] << '\n';

	PC pc1(cpu1, ram1, hdd1, gpu1.screen);

	int count = 0;

	bool shift = false;
	bool caps_lock = false;
	bool space = false;

	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_LSHIFT: shift = !shift; std::cout << "Shift is: " << shift << '\n'; break;
					case SDLK_RSHIFT: shift = !shift; std::cout << "Shift is: " << shift << '\n';break;
					case SDLK_CAPSLOCK: caps_lock = !caps_lock; std::cout << "Caps Lock is: " << caps_lock << '\n';break;	
					case SDLK_1: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 10; } else { pc1.cpu.interruptRegister = 1; } break;
					case SDLK_2: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 68; } else { pc1.cpu.interruptRegister = 2; } break;
					case SDLK_3: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 12; } else { pc1.cpu.interruptRegister = 3; } break;
					case SDLK_4: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 13; } else { pc1.cpu.interruptRegister = 4; } break;
					case SDLK_5: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 14; } else { pc1.cpu.interruptRegister = 5; } break;
					case SDLK_6: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 37; } else { pc1.cpu.interruptRegister = 6; } break;
					case SDLK_7: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 15; } else { pc1.cpu.interruptRegister = 7; } break;
					case SDLK_8: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 19; } else { pc1.cpu.interruptRegister = 8; } break;
					case SDLK_9: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 17; } else { pc1.cpu.interruptRegister = 9; } break;
					case SDLK_0: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 18; } else { pc1.cpu.interruptRegister = 0; } break;
					case SDLK_PERIOD: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 29; } else { pc1.cpu.interruptRegister = 23; } break;
					case SDLK_COMMA: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 28; } else { pc1.cpu.interruptRegister = 22; } break;
					case SDLK_a: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 40; } else { pc1.cpu.interruptRegister = 69; } break;
					case SDLK_b: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 41; } else { pc1.cpu.interruptRegister = 70; } break;
					case SDLK_c: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 42; } else { pc1.cpu.interruptRegister = 71; } break;
					case SDLK_d: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 43; } else { pc1.cpu.interruptRegister = 72; } break;
					case SDLK_e: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 44; } else { pc1.cpu.interruptRegister = 73; } break;
					case SDLK_f: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 45; } else { pc1.cpu.interruptRegister = 74; } break;
					case SDLK_g: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 46; } else { pc1.cpu.interruptRegister = 75; } break;
					case SDLK_h: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 47; } else { pc1.cpu.interruptRegister = 76; } break;
					case SDLK_i: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 48; } else { pc1.cpu.interruptRegister = 77; } break;
					case SDLK_j: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 49; } else { pc1.cpu.interruptRegister = 78; } break;
					case SDLK_k: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 50; } else { pc1.cpu.interruptRegister = 79; } break;
					case SDLK_l: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 51; } else { pc1.cpu.interruptRegister = 80; } break;
					case SDLK_m: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 52; } else { pc1.cpu.interruptRegister = 81; } break;
					case SDLK_n: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 53; } else { pc1.cpu.interruptRegister = 82; } break;
					case SDLK_o: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 54; } else { pc1.cpu.interruptRegister = 83; } break;
					case SDLK_p: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 55; } else { pc1.cpu.interruptRegister = 84; } break;
					case SDLK_q: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 56; } else { pc1.cpu.interruptRegister = 85; } break;
					case SDLK_r: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 57; } else { pc1.cpu.interruptRegister = 86; } break;
					case SDLK_s: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 58; } else { pc1.cpu.interruptRegister = 87; } break;
					case SDLK_t: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 59; } else { pc1.cpu.interruptRegister = 88; } break;
					case SDLK_u: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 60; } else { pc1.cpu.interruptRegister = 89; } break;
					case SDLK_v: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 61; } else { pc1.cpu.interruptRegister = 90; } break;
					case SDLK_w: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 62; } else { pc1.cpu.interruptRegister = 91; } break;
					case SDLK_x: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 63; } else { pc1.cpu.interruptRegister = 92; } break;
					case SDLK_y: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 64; } else { pc1.cpu.interruptRegister = 93; } break;
					case SDLK_z: pc1.cpu.interrupted = true; if(caps_lock == true) { pc1.cpu.interruptRegister = 65; } else { pc1.cpu.interruptRegister = 94; } break;
					case SDLK_MINUS: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 66; } else { pc1.cpu.interruptRegister = 21; } break;
					case SDLK_PLUS: pc1.cpu.interrupted = true; if(shift == true) { pc1.cpu.interruptRegister = 30; } else { pc1.cpu.interruptRegister = 20; } break;
					case SDLK_BACKSPACE: pc1.cpu.interrupted = true; pc1.cpu.interruptRegister = 96; break;
					case SDLK_SPACE: pc1.cpu.interrupted = true; pc1.cpu.interruptRegister = 67; space = !space; break;
					case SDLK_RETURN: pc1.cpu.interrupted = true; pc1.cpu.interruptRegister = 95; break;
				}
				break;
			}
	
		}

		if (space) {
			pc1.cpu.tick();
			gpu1.tick();
		}		
		NSSDL::updateSDL(pc1.screen);
	}

	SDL_Quit();

	return 0;
}
