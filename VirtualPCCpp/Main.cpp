#include <SDL.h>
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

	Memory ram1(4096);
	Memory hdd1(2048);

	GPU gpu1(256, 4096, 32, 1024, &pc1W, &ram1);

	NSSDL::initSDL(gpu1.screen, width, height);

	CPU cpu1(1024, ram1, gpu1);

	//Characters:
	//0:  0, 1:  1, 2:  2, 3:  3, 4:  4, 5:  5, 6:  6, 7:  7, 8:  8, 9:  9,
	//!: 10, #: 11, %: 12, (: 13, ): 14, `: 15, -: 16, [: 17, ]: 18, ^: 19,
	//_: 20, {: 21, }: 22, +: 23, =: 24, >: 25, <: 26, \: 27, |: 28, /: 29,
	//:: 30, ;: 31, *: 32, &: 33, ?: 34, A: 35, B: 36, C: 37: D: 38, E: 39,
	//F: 40, G: 41, H: 42, I: 43, J: 44, K: 45, L: 46, M: 47, N: 48, O: 49,
	//P: 50, Q: 51, R: 52, S: 53, T: 54, U: 55, V: 56, W: 57, X: 58, Y: 59,
	//Z: 60, x: 61

	//Instruction set:
	//LOAD0_C - Load to register0 from cache,                        1 argument  (index of origin memory in cache)
	//LOAD1_C - Load to register1 from cache,                        1 argument  (index of origin memory in cache)
	//LOAD0_R - Load to register0 from ram,                          1 argument  (index of origin memory in ram)
	//LOAD1_R - Load to register1 from ram,                          1 argument  (index of origin memory in ram)
	//CLR0    - Clear register0,                                     0 arguments
	//CLR1    - Clear register1,                                     0 arguments
	//WRT0_C  - Write from register0 to cache,                       1 argument  (index of target memory in cache)
	//WRT1_C  - Write from register1 to cache,                       1 argument  (index of target memory in cache)
	//WRT0_R  - Write from register0 to ram,                         1 argument  (index of target memory in ram)
	//WRT1_R  - Write from register1 to ram,                         1 argument  (index of target memory in ram)
	//WRTC_R  - Write from cache to ram,                             2 arguments (index of origin memory in cache, index of target memory in ram)
	//WRTR_C  - Write from ram to cache,                             2 arguments (index of origin memory in ram, index of target memory in ram)
	//SUM     - Adds register0 and register1,                        0 arguments
	//SUB     - Subtracts register0 and register1,                   0 arguments
	//MLT     - Multiplies register0 and register1,                  0 arguments
	//DIV     - Divides register0 and register1,                     0 arguments
	//REG0_B  - Check if register0 is bigger than register1,         0 arguments
	//REG0_BE - Check if register0 is bigger or equal to register1,  0 arguments
	//REG1_B  - Check if register1 is bigger than register0,         0 arguments
	//REG1_BE - Check if register1 is bigger or equal to register0,  0 arguments
	//REG_EQL - Check if register0 and register1 are the same size,  0 arguments
	//REG_DIF - Check if register0 and register1 are different size, 0 arguments
	//JMP     - Sets program counter to target position,             1 argument  (target position)
	//CMP     - Compares register0 to a condition,                   2 arguments (0 or 1 for the condition (false or true), jump position if condition is met)
	//COT0    - Print to console contents of register0,              0 arguments
	//COT1    - Print to console contents of register1,              0 arguments


	//Assembly example

	//Set memory
	cpu1.cache.memory[100] = 8;
	cpu1.cache.memory[101] = 5;

	//Loads register0 and register1, sums them, prints contents of register0
	std::vector<std::string> code = { "LOAD0_C", "100", 
									  "LOAD1_C", "101", 
									  "SUM", 
									  "COT0",
									  "DRAWP", "50", "50", "100", "100", "60", "60", "60"};

	//Compiles Assembly code
	Assembly::Compile(code, cpu1);

	//End of Assembly example

	PC pc1(cpu1, ram1, hdd1, gpu1.screen);

	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
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