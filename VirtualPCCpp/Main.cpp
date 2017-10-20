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

	Memory ram1(131072);
	Memory hdd1(128 * 128);

	GPU gpu1(8192, 524288, 64, 16384, &pc1W, ram1);

	NSSDL::initSDL(gpu1.screen, width, height);

	CPU cpu1(128, 128, ram1, hdd1, gpu1);

	std::vector<std::string> code;

	Assembly::readFile("Program.txt", code);

	Assembly::Compile(code, cpu1);

	gpu1.functionCounter = 0;

	PC pc1(cpu1, ram1, hdd1, gpu1.screen);

	int count = 0;

	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_0: pc1.cpu.keyboardRegister = 0; break;
					case SDLK_1: pc1.cpu.keyboardRegister = 1; break;
					case SDLK_2: pc1.cpu.keyboardRegister = 2; break;
					case SDLK_3: pc1.cpu.keyboardRegister = 3; break;
					case SDLK_4: pc1.cpu.keyboardRegister = 4; break;
					case SDLK_5: pc1.cpu.keyboardRegister = 5; break;
					case SDLK_6: pc1.cpu.keyboardRegister = 6; break;
					case SDLK_7: pc1.cpu.keyboardRegister = 7; break;
					case SDLK_8: pc1.cpu.keyboardRegister = 8; break;
					case SDLK_9: pc1.cpu.keyboardRegister = 9; break;
					case SDLK_a: pc1.cpu.keyboardRegister = 40; break;
					case SDLK_b: pc1.cpu.keyboardRegister = 41; break;
					case SDLK_c: pc1.cpu.keyboardRegister = 42; break;
					case SDLK_d: pc1.cpu.keyboardRegister = 43; break;
					case SDLK_e: pc1.cpu.keyboardRegister = 44; break;
					case SDLK_f: pc1.cpu.keyboardRegister = 45; break;
					case SDLK_g: pc1.cpu.keyboardRegister = 46; break;
					case SDLK_h: pc1.cpu.keyboardRegister = 47; break;
					case SDLK_i: pc1.cpu.keyboardRegister = 48; break;
					case SDLK_j: pc1.cpu.keyboardRegister = 49; break;
					case SDLK_k: pc1.cpu.keyboardRegister = 50; break;
					case SDLK_l: pc1.cpu.keyboardRegister = 51; break;
					case SDLK_m: pc1.cpu.keyboardRegister = 52; break;
					case SDLK_n: pc1.cpu.keyboardRegister = 53; break;
					case SDLK_o: pc1.cpu.keyboardRegister = 54; break;
					case SDLK_p: pc1.cpu.keyboardRegister = 55; break;
					case SDLK_q: pc1.cpu.keyboardRegister = 56; break;
					case SDLK_r: pc1.cpu.keyboardRegister = 57; break;
					case SDLK_s: pc1.cpu.keyboardRegister = 58; break;
					case SDLK_t: pc1.cpu.keyboardRegister = 59; break;
					case SDLK_u: pc1.cpu.keyboardRegister = 60; break;
					case SDLK_v: pc1.cpu.keyboardRegister = 61; break;
					case SDLK_w: pc1.cpu.keyboardRegister = 62; break;
					case SDLK_x: pc1.cpu.keyboardRegister = 63; break;
					case SDLK_y: pc1.cpu.keyboardRegister = 64; break;
					case SDLK_z: pc1.cpu.keyboardRegister = 65; break;
					case SDLK_SPACE: pc1.cpu.keyboardRegister = 67; break;
					case SDLK_BACKSPACE: pc1.cpu.keyboardRegister = 63000; break;
				}
				break;
			}
	
		}
		pc1.cpu.tick();
		gpu1.executeCommand();
		gpu1.tick();
		NSSDL::updateSDL(pc1.screen);
	}

	NSSDL::cleanSDL(pc1.screen);

	SDL_Quit();

	return 0;
}
