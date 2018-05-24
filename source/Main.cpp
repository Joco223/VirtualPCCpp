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
#include <thread>
#include <unordered_map>
#include <chrono>
#include <SDL_net.h>
#include <sstream>
#include <fstream>

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

void runvGPU(GPU* gpu1){
	while(true){
		gpu1->tick();
	}

}

void runVPC (PC* pc1, TCPsocket* clientSocket) {
	int ticks = 0;
	int targetTicks = 25;
	long int totalTicks = 0;
	int numOfLoops = 0;
	int ticks2 = 0;
	int targetTicks2 = 1000;

	auto start = std::chrono::steady_clock::now();

	while(true){
		pc1->cpu.tick(clientSocket);
		//totalTicks++;
		/*if(ticks >= targetTicks) {
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double> diff = end-start;
			if(diff.count() - (10 * numOfLoops) >= 10){
				std::cout << (totalTicks / 10) << " t/s" << '\n';
				totalTicks = 0;
				numOfLoops++;
			}
			ticks = 0;
		}
		ticks++;*/
	}
}

int main(int argc, char* argv[]) {

	SDLNet_Init();

	int port = 52239;
	IPaddress serverIP;
	char* pBuffer;
	pBuffer = new char[9];
	std::string IP;
	std::cout << "Type in server IP address" << '\n';
	std::cin >> IP;
	bool OM = false;
	std::string address;

	SDLNet_SocketSet socketSet;
	socketSet = SDLNet_AllocSocketSet(2);

	TCPsocket clientSocket;

	if(IP != "0.0.0.0"){
		int success = SDLNet_ResolveHost(&serverIP, IP.c_str(), port);

		clientSocket = SDLNet_TCP_Open(&serverIP);
		std::cout << SDLNet_GetError() << '\n';
		SDLNet_TCP_AddSocket(socketSet, clientSocket);

		int activeSockets = SDLNet_CheckSockets(socketSet, 1000);
		int gotServerResponse = SDLNet_SocketReady(clientSocket);
		int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, pBuffer, 3);

		address.push_back(pBuffer[0]);
		address.push_back(pBuffer[1]);
		address.push_back(pBuffer[2]);

		std::cout << "Successfully connected. vPC address is: " << (int)address[0] << "." << (int)address[1] << "." << (int)address[2] << '\n';
		OM = true;
	}

 	/*int input;
	std::cout << "Chose run mode (0 to send a message, 1 to recieve)" << '\n';
	std::cin >> input;

	if(input == 1){
		std::cout << "Waiting for message..." << '\n';
		while(true){
			int c = SDLNet_TCP_Recv(clientSocket, pBuffer, 9);
			if(c > 0){
				std::cout << pBuffer << '\n';
			}
		}
	}else{
		std::cout << "Sending message..." << '\n';
		std::cout << "Type in target vPC address:" << '\n';
		std::string target;
		std::cin >> target;
		std::string data;
		std::cout << "Type in desired data (5 characters): " << '\n';
		std::cin >> data;

		std::string message = "0" + address + target + data;

		SDLNet_TCP_Send(clientSocket, message.c_str(), 12);
		std::cout << "Message sent..." << '\n';
	}*/

	SDL_Event event;

	SDL_Window* window2;
	SDL_Surface* surface2;
	SDL_Surface* pixelSpace2;
	SDL_Renderer* renderer2;
	SDL_Texture* texture2;
	unsigned int* pixels2;

	SDLWindow pc1W(window2, surface2, pixelSpace2, renderer2, texture2, pixels2);

	Memory ram1(67108864); //64MB Ram
	Memory hdd1(268435456); //256MB HDD

	NSSDL::initSDL(&pc1W, width, height);

	GPU gpu1(134217728, 32, 32, &pc1W, ram1); //128MB vRam

	CPU cpu1(16384, 16384, ram1, hdd1, gpu1);

	cpu1.onlineMode = OM;
	cpu1.vPCaddress = address;

	std::vector<std::string> code;
	std::vector<std::string> gpu_code;

	std::vector<int> vValues;
	std::vector<std::string> vNames;
	int addMem = 0;

	std::vector<std::string> cCode;
	std::vector<std::string> tokens;

	std::vector<Assembly::variable> tmp2;

	/*Assembly::readFile("CPU_Programs/Program2.sal", code);

	Assembly::Compile(code, cpu1, gpu1, tmp2, addMem);

	GPUAssembly::readFile("GPU_Programs/Program.sgal", gpu_code);

	std::vector<GPUAssembly::variable> tmp;

	for(int i = 0; i < tmp2.size(); i++) {
		GPUAssembly::variable tempo;
		tempo.name = tmp2[i].name;
		tempo.size = tmp2[i].size;
		tempo.value = tmp2[i].position;
		tempo.sDepth = 0;
		tmp.push_back(tempo);
	};

	GPUAssembly::Compile(gpu_code, gpu1, tmp);*/

	gpu1.loadFont();

	PC pc1(cpu1, ram1, hdd1, gpu1.screen);

	std::string name;
	std::cout << "Name of the program you want to run: ";
	std::cin >> name;

	std::string line;
	std::ifstream myfile(name);
	int pos = 0;

	if (myfile.is_open()){
		while (getline (myfile, line)) {
			if (!(line[0] == '/' && line[1] == '/')) {
				if (line != "") {
					std::string buf;
					std::stringstream ss(line);

					while (ss >> buf) {
						if(pos == 0){
							pc1.cpu.programCounter = std::stoi(buf);
							pos++;
						}else if(pos == 1) {
							pc1.cpu.interPos = std::stoi(buf);
							pos++;
						}else if(pos == 2) {
							pc1.cpu.interFinishPos = std::stoi(buf);
							pos++;
						}else if(pos == 3) {
							pc1.cpu.interTartgetPos = std::stoi(buf);
							pos++;
						}else{
							pc1.cpu.ram.memory[(pos - 4)] = std::stoi(buf);
							pos++;
						}
					}
				}
			}
		}
	}

	bool shift = false;
	bool interCheck = false;


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



	bool update = true;
	int keyCode = 0;
	bool input = false;
	bool firstMsg = true;

	std::thread rVPC1(runVPC, &pc1, &clientSocket);
	std::thread rVGPU1(runvGPU, &gpu1);

	auto start2 = std::chrono::steady_clock::now();
	int numOfLoops2 = 0;

	//for(int i = 0; i < 8; i++) {pBuffer[i] = 0;}

	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				input = true;
				keyCode = event.key.keysym.scancode;
				break;
			}
		}

		if(OM){
			int c = SDLNet_TCP_Recv(clientSocket, pBuffer, 8);
			if(c == 8){
				if(pc1.cpu.sockets[pBuffer[8]] == true){
					pc1.cpu.registers[12] = 3;
					pc1.cpu.interRegisters[0] = (unsigned int)pBuffer[0];
					pc1.cpu.interRegisters[1] = (unsigned int)pBuffer[1];
					pc1.cpu.interRegisters[2] = (unsigned int)pBuffer[2];
					pc1.cpu.interRegisters[3] = (unsigned int)pBuffer[3];
					pc1.cpu.interRegisters[4] = (unsigned int)pBuffer[4];
					pc1.cpu.interRegisters[5] = (unsigned int)pBuffer[5];
					pc1.cpu.interRegisters[6] = (unsigned int)pBuffer[6];
					pc1.cpu.interRegisters[7] = (unsigned int)pBuffer[7];
					pc1.cpu.interrupted = true;
				}
			}
		}
		
		auto end2 = std::chrono::steady_clock::now();
		std::chrono::duration<double, std::milli> diff2 = end2-start2;
		if((int)diff2.count() % 16 == 0){
			pc1.cpu.currentFrame++;
			gpu1.updateCharacters();
			gpu1.updateScreen();;
		}

		if(input == true){
			if(mapKey.find(keyCode) == mapKey.end()){
				std::cout << "Unknown button: " << keyCode << '\n';
			}else{
				pc1.cpu.interrupted = true;
				pc1.cpu.registers[12] = 1;
				pc1.cpu.interRegisters[0] = mapKey.find(keyCode)->second;
			}
			input = false;
		}

	}

	rVPC1.join();
	rVGPU1.join();

	if(OM){
		SDLNet_TCP_Close(clientSocket);
		SDLNet_FreeSocketSet(socketSet);
	}
	delete pBuffer;

	SDL_Quit();

	return 0;
}
