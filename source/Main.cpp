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
#include <thread>

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

void runGPUCores (GPU *gpu) {
	while(true){
		gpu->tick();

	}
}

void updateGPUSCreen (GPU *gpu){
	while(true){
		gpu->updateScreen();
	}
}

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
	//Sector size ^     ^ number of sectors

	GPU gpu1(4096, 64000, 16, 16, 16384, &pc1W, ram1);

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
	std::vector<Assembly::variable> tmp2;

	Assembly::readFile("CPU_Programs/Program2.sal", code);

	Assembly::Compile(code, cpu1, gpu1, tmp2, addMem);

	GPUAssembly::readFile("GPU_Programs/Program.sgal", gpu_code);

	std::vector<GPUAssembly::variable> tmp;

	for(int i = 0; i < tmp2.size(); i++) {
		GPUAssembly::variable tempo;
		tempo.name = tmp2[i].name;
		tempo.size = tmp2[i].size;
		tempo.value = tmp2[i].value;
		tempo.sDepth = 0;
		tmp.push_back(tempo);
	};

	GPUAssembly::Compile(gpu_code, gpu1, tmp);

	gpu1.loadFont();

	PC pc1(cpu1, ram1, hdd1, gpu1.screen);

	bool shift = false;
	bool caps_lock = false;
	bool space = false;

	clock_t t;
	bool printed = false;
	bool measured = false;

	std::thread GPU1 (runGPUCores, &gpu1);
	std::thread GPU1S (updateGPUSCreen, &gpu1);

	auto now = std::chrono::high_resolution_clock::now();
	auto lastFrame = std::chrono::high_resolution_clock::now();
	int ups = 1000; //How may updates per second there is

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

		now = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds delta = now - lastFrame;
		lastFrame = now;

		if(delta.count() < (1e9/ups)){
			long int wait = ((1e9/ups) - delta.count());
			std::this_thread::sleep_for(std::chrono::nanoseconds(wait));
		}

		pc1.cpu.tick();

		/*if (gpu1.started == false && printed == false) {
			t = clock() - t;
			std::cout << "\n";
			std::cout << "It took: " << ((double)t/CLOCKS_PER_SEC) << " seconds for GPU to finish its task" << '\n';
			printed = true;
		}*/
	}

	GPU1.join();
	GPU1S.join();

	SDL_Quit();

	return 0;
}
