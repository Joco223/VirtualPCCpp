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
#include <unordered_map>
#include <chrono>

#include "NSSDL.h"
#include "SDLWindow.h"
#include "Memory.h"
#include "CPU.h"
#include "PC.h"
#include "GPU.h"
#include "Assembly.h"
#include "GPUAssembly.h"
#include "Compiler.h"

int scale = 1;
int width = (640 * scale);
int height = (480 * scale);

bool quit = false;

typedef unsigned char byte;

void runGPUCores (GPU *gpu) {
	while(true){
		gpu->tick();

	}
}

void updateGPUSCreen (GPU *gpu){
	while(true){
		gpu->updateCharacters();
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
	Memory hdd1(4090 * 4090);
	//Sector size ^     ^ number of sectors

	GPU gpu1(16000000, 16, 16, &pc1W, ram1);

	NSSDL::initSDL(gpu1.screen, width, height);

	CPU cpu1(4090, 4090, ram1, hdd1, gpu1);

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

	//std::thread GPU1(runGPUCores, &gpu1);
	//std::thread GPU1S(updateGPUSCreen, &gpu1);

	std::unordered_map<int, int> mapKey = { {39, 0}, { 4,69}, {14,79}, { 24,89}, {224, 99}, {61,109}, {70,119}, {80,129}, {52,11},
											{30, 1}, { 5,70}, {15,80}, { 25,90}, {226,100}, {62,110}, {71,120}, {81,130}, {54,22},
										 	{31, 2}, { 6,71}, {16,81}, { 26,91}, { 44,101}, {63,111}, {72,121}, {79,131}, {55,23},
										 	{32, 3}, { 7,72}, {17,82}, { 27,92}, {230,102}, {64,112}, {73,122}, {53, 36}, {56,24},
											{33, 4}, { 8,73}, {18,83}, { 28,93}, {228,103}, {65,113}, {74,123}, {45, 21},
										 	{34, 5}, { 9,74}, {19,84}, { 29,94}, {229,104}, {66,114}, {75,124}, {46, 30},
										 	{35, 6}, {10,75}, {20,85}, { 40,95}, { 42,105}, {67,115}, {76,125}, {49, 28},
										 	{36, 7}, {11,76}, {21,86}, { 43,96}, { 58,106}, {68,116}, {77,126}, {47, 32},
										 	{37, 8}, {12,77}, {22,87}, { 57,97}, { 59,107}, {69,117}, {78,127}, {48, 33},
										 	{38, 9}, {13,78}, {23,88}, {225,98}, { 60,108}, {41,118}, {82,128}, {51, 27} };

	int ticks = 0;
	int targetTicks = 15;
	bool printed = false;

	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if(mapKey.find(event.key.keysym.scancode) == mapKey.end()){
					std::cout << "Unknown button: " << event.key.keysym.scancode << '\n';
				}else{
					pc1.cpu.interrupted = true;
					pc1.cpu.registers[12] = 1;
					pc1.cpu.interRegisters[0] = mapKey.find(event.key.keysym.scancode)->second;
				}
				break;
			}
		}

		//auto start = std::chrono::steady_clock::now();
		for(int i = 0; i < 10; i++) {
    		pc1.cpu.tick();
    		gpu1.tick();

		}
		/*if(ticks >= targetTicks) {
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double, std::nano> diff = end-start;
			std::cout <<  diff.count() << '\n';
			ticks = 0;
		}
		ticks++;*/

		gpu1.updateScreen();
	}

	//GPU1.join();
	//GPU1S.join();

	SDL_Quit();

	return 0;
}
