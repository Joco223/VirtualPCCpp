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

	GPU gpu1(8192, 524288, 64, 16384, &pc1W, &ram1);

	NSSDL::initSDL(gpu1.screen, width, height);

	CPU cpu1(8192, ram1, gpu1);

	//Instruction set:
	//LOAD0_C  - Load to register0 from cache,                        1 argument  (index of origin memory in cache)
	//LOAD1_C  - Load to register1 from cache,                        1 argument  (index of origin memory in cache)
	//LOAD0_R  - Load to register0 from ram,                          1 argument  (index of origin memory in ram)
	//LOAD1_R  - Load to register1 from ram,                          1 argument  (index of origin memory in ram)
	//CLR0     - Clear register0,                                     0 arguments
	//CLR1     - Clear register1,                                     0 arguments
	//WRT0_C   - Write from register0 to cache,                       1 argument  (index of target memory in cache)
	//WRT1_C   - Write from register1 to cache,                       1 argument  (index of target memory in cache)
	//WRT0_R   - Write from register0 to ram,                         1 argument  (index of target memory in ram)
	//WRT1_R   - Write from register1 to ram,                         1 argument  (index of target memory in ram)
	//WRTC_R   - Write from cache to ram,                             2 arguments (index of origin memory in cache, index of target memory in ram)
	//WRTR_C   - Write from ram to cache,                             2 arguments (index of origin memory in ram, index of target memory in ram)
	//SUM      - Adds register0 and register1,                        0 arguments
	//SUB      - Subtracts register0 and register1,                   0 arguments
	//MLT      - Multiplies register0 and register1,                  0 arguments
	//DIV      - Divides register0 and register1,                     0 arguments
	//REG0_B   - Check if register0 is bigger than register1,         0 arguments
	//REG0_BE  - Check if register0 is bigger or equal to register1,  0 arguments
	//REG1_B   - Check if register1 is bigger than register0,         0 arguments
	//REG1_BE  - Check if register1 is bigger or equal to register0,  0 arguments
	//REG_EQL  - Check if register0 and register1 are the same size,  0 arguments
	//REG_DIF  - Check if register0 and register1 are different size, 0 arguments
	//JMP      - Sets program counter to target position,             1 argument  (target position)
	//CMP      - Compares register0 to a condition,                   2 arguments (0 or 1 for the condition (false or true), jump position if condition is met)
	//COT0     - Print to console contents of register0,              0 arguments
	//COT1     - Print to console contents of register1,              0 arguments
	//WRT_R_VR - Write from RAM to vRAM,                              3 arguments (start position of memory in ram, end position of memory in ram, start position in memory of vram)
	//WRT_VR_R - Write from vRAM to RAM,                              3 arguments (start position of memory in vram, end position of memory in vram, start position in memory of ram)
	//DRAWP    - Draw rectangle on screen,                            7 arguments (start x position, start y position, end x position, end y position, r value, g value, b value)
	//PRINT    - Print character to screen,                           6 arguments (character position in cpu cache, x position in character space, y position in character space, r value, g value, b value)
	//RGB values are limited from 0 to 63

	cpu1.cache.memory[200] = 47;
	cpu1.cache.memory[201] = 44;
	cpu1.cache.memory[202] = 51;
	cpu1.cache.memory[203] = 51;
	cpu1.cache.memory[204] = 54;

	std::vector<std::string> code;

	Assembly::readFile("Program.txt", code);

	Assembly::Compile(code, cpu1);

	gpu1.functionCounter = 0;

	PC pc1(cpu1, ram1, hdd1, gpu1.screen);

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