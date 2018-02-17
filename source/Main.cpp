#define SDL_MAIN_HANDLED

#ifdef _WIN32
#include <SDL.h>
#endif

#ifdef __linux
#include <SDL2/SDL.h>
#endif

#include <vector>
#include <iostream>
#include <string>
#include <time.h>

#include "NSSDL.h"
#include "SDLWindow.h"
#include "Memory.h"
#include "CPU.h"
#include "PC.h"
#include "GPU.h"
#include "Assembly.h"
#include "GPUAssembly.h"
#include "Compiler.h"

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

	Memory ram1(16000000);
	Memory hdd1(512 * 512);

	GPU gpu1(4096, 64000, 256, 16384, &pc1W, ram1);

	NSSDL::initSDL(gpu1.screen, width, height);

	CPU cpu1(512, 512, ram1, hdd1, gpu1);

	std::vector<std::string> code;
	std::vector<std::string> gpu_code;

	std::vector<int> vValues;
	std::vector<std::string> vNames;
	int addMem = 0;

	std::vector<std::string> cCode;
	std::vector<std::string> tokens;

	//Compiler::readFile("Program.cl", cCode);

	//Compiler::splitFile(cCode, tokens);

	//Compiler::Parse(tokens);

	Assembly::readFile("CPU_Programs/Program2.sal", code);

	Assembly::Compile(code, cpu1, vValues, vNames, addMem);

	//GPUAssembly::readFile("GPU_Programs/Program.sgal", gpu_code);

	//GPUAssembly::Compile(gpu_code, gpu1, vValues, vNames, addMem);

	gpu1.loadFont();

	PC pc1(cpu1, ram1, hdd1, gpu1.screen);

	bool shift = false;
	bool caps_lock = false;
	bool space = false;

	clock_t t;
	bool printed = false;
	bool measured = false;

	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				break;
			}

		}
		if (measured == false) {
			t = clock();
			measured = true;
		}
		pc1.cpu.tick();
		if (pc1.cpu.halt == true && printed == false) {
			t = clock() - t;
			std::cout << "\n";
			std::cout << "It took: " << ((double)t/CLOCKS_PER_SEC) << " seconds for CPU to finish its task" << '\n';
			printed = true;
		}
		//gpu1.tick();
		//gpu1.updateScreen();
		//NSSDL::updateSDL(pc1.screen);
	}

	SDL_Quit();

	return 0;
}
