#include "GPU.h"


GPU::GPU(int vRamSize, int coreCountX, int coureCountY, SDLWindow* screen_, Memory& ram_)
	:
	vRam(Memory(vRamSize)),
	ram(ram_),
	screen(screen_),
	started(false),
	currentTask(0),
	screenUpdated(false),
	update(false)
{
	for (int x = 0; x < (coreCountX * coureCountY); x++) {
		cores.emplace_back(vRam, screen_, screenUpdated, coureCountY, x, 0);
	}

	for(int y = 0; y < 60; y++) {
		for(int x = 0; x < 106; x++) {
			vRam.memory[y * 106 + x] = 97;
			vRam.memory[x + y * 106 + 12720] = 255;
		}
	}

	characterBuffer = SDL_CreateTexture(screen->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 640 * scale, 480 * scale);
}

void GPU::loadFont() {
	SDL_Surface *bmpSurf = SDL_LoadBMP("Fonts/Basic_Font.bmp");
	font = SDL_CreateTextureFromSurface(screen->renderer, bmpSurf);
	SDL_FreeSurface(bmpSurf);
}

void GPU::setCharID(byte x, byte y, byte id) {
	screenCharacters[y * 106 + x].characterID = id;
}

void GPU::setCharCB(byte x, byte y, byte cB) {
	screenCharacters[y * 106 + x].rB = (cB >> 5) & 0b00000111;
	screenCharacters[y * 106 + x].gB = (cB >> 2) & 0b00111;
	screenCharacters[y * 106 + x].bB = cB & 0b11;
}

void GPU::setCharCF(byte x, byte y, byte cF) {
	screenCharacters[y * 106 + x].r = (cF >> 5) & 0b00000111;
	screenCharacters[y * 106 + x].g = (cF >> 2) & 0b00111;
	screenCharacters[y * 106 + x].b = cF & 0b11;
}

void GPU::updateCharacters() {
	SDL_Rect source;
	SDL_Rect target;
	source.w = 6;
	source.h = 8;
	target.w = 6 * scale;
	target.h = 8 * scale;

	for(int x = 0; x < 106; x++){
		for(int y = 0; y < 60; y++){
			if(x >= 0 && x <= 106 && y >= 0 && y <= 60) {
				source.x = 7 * 6;
				source.y = 6 * 8;

				target.x = x * 6 * scale;
				target.y = y * 8 * scale;

				byte rB = (vRam.memory[x + y * 106 + 6360] >> 5) & 0b00000111;
				byte gB = (vRam.memory[x + y * 106 + 6360] >> 2) & 0b00111;
				byte bB = vRam.memory[x + y * 106 + 6360] & 0b11;

				byte rF = (vRam.memory[x + y * 106 + 12720] >> 5) & 0b00000111;
				byte gF = (vRam.memory[x + y * 106 + 12720] >> 2) & 0b00111;
				byte bF = vRam.memory[x + y * 106 + 12720] & 0b11;

				SDL_SetTextureColorMod(font, rB * 36.428, gB * 36.428, bB * 85);

				if(rB != 0 && gB != 0 && bB != 0) {SDL_RenderCopy(screen->renderer, font, &source, &target);}

				source.x = (vRam.memory[x + y * 106] % 10) * 6;
				source.y = (vRam.memory[x + y * 106] / 10) * 8;

				SDL_SetTextureColorMod(font, rF * 36.428, gF * 36.428, bF * 85);

				if(rF != 0 && gF != 0 && bF != 0) {SDL_RenderCopy(screen->renderer, font, &source, &target);}
			}
		}
	}
}

void GPU::updateScreen() {
	SDL_UpdateTexture(screen->texture, nullptr, screen->pixels, screen->pixelSpace->pitch);
	SDL_RenderCopy(screen->renderer, screen->texture, nullptr, nullptr);
	//updateCharacters();
	SDL_RenderPresent(screen->renderer);
}

void convertByte(int number, byte& b1, byte& b2, byte& b3) {
	b1 = number & 0xff;
	b2 = (byte)(number >> 8);
	b3 = (byte)(number >> 16);
}

void GPU::setPC() {
	for(int i = 0; i < cores.size(); i++) {cores[i].programCounter = targetPC;};
}

void GPU::tick() {
	if(started) {
		for (int i = 0; i < cores.size(); i++) {
			if (cores[i].halt == true) {
				if (currentTask < tasks.size()) {
					cores[i].programCounter = targetPC;
					cores[i].idX = tasks[currentTask].x;
					cores[i].idY = tasks[currentTask].y;
					cores[i].halt = false;
					currentTask++;
				}else{
					tasks.clear();
					currentTask = 0;
				}
			}else{
				cores[i].tick();
			}
		}
	}
}
