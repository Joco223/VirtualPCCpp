#include "GPU.h"


GPU::GPU(int vRamSize, int coreCountX, int coureCountY, SDLWindow* screen_, Memory& ram_)
	:
	vRam(Memory(vRamSize)),
	ram(ram_),
	screen(screen_),
	started(true),
	currentTask(0),
	screenUpdated(false)
{
	for (int x = 0; x < (coreCountX * coureCountY); x++) {
		cores.emplace_back(vRam, progMem, screen_, screenUpdated, coureCountY, x, 0);
	}

	for(int y = 0; y < 60; y++) {
		for(int x = 0; x < 106; x++) {
			screenCharacter tmp;
			tmp.x = x;
			tmp.y = y;
			tmp.r = 7;
			tmp.g = 7;
			tmp.b = 3;
			tmp.rB = 0;
			tmp.gB = 0;
			tmp.bB = 0;
			tmp.characterID = 97;
			screenCharacters.push_back(tmp);
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
	charactersNUpdate.push_back({x, y});
}

void GPU::setCharCB(byte x, byte y, byte cB) {
	screenCharacters[y * 106 + x].rB = (cB >> 5) & 0b00000111;
	screenCharacters[y * 106 + x].gB = (cB >> 2) & 0b00111;
	screenCharacters[y * 106 + x].bB = cB & 0b11;
	charactersNUpdate.push_back({x, y});
}

void GPU::setCharCF(byte x, byte y, byte cF) {
	screenCharacters[y * 106 + x].r = (cF >> 5) & 0b00000111;
	screenCharacters[y * 106 + x].g = (cF >> 2) & 0b00111;
	screenCharacters[y * 106 + x].b = cF & 0b11;
	charactersNUpdate.push_back({x, y});
}

void GPU::updateCharacters() {
	SDL_SetRenderTarget(screen->renderer, characterBuffer);

	SDL_Rect source;
	SDL_Rect target;
	source.w = 6;
	source.h = 8;
	target.w = 6 * scale;
	target.h = 8 * scale;

	for(auto& i : charactersNUpdate) {
		if(i.x > 0 && i.x < 106 && i.y > 0 && i.y < 60) {
			source.x = 7 * 6;
			source.y = 6 * 8;

			target.x = i.x * 6 * scale;
			target.y = i.y * 8 * scale;

			SDL_SetTextureColorMod(font, screenCharacters[i.y * 106 + i.x].rB * 36.428, screenCharacters[i.y * 106 + i.x].gB * 36.428, screenCharacters[i.y * 106 + i.x].bB * 85);

			SDL_RenderCopy(screen->renderer, font, &source, &target);

			source.x = (screenCharacters[i.y * 106 + i.x].characterID % 10) * 6;
			source.y = (screenCharacters[i.y * 106 + i.x].characterID / 10) * 8;

			SDL_SetTextureColorMod(font, screenCharacters[i.y * 106 + i.x].r * 36.428, screenCharacters[i.y * 106 + i.x].g * 36.428, screenCharacters[i.y * 106 + i.x].b * 85);

			SDL_RenderCopy(screen->renderer, font, &source, &target);
		}
	}

	SDL_SetRenderTarget(screen->renderer, nullptr);

	if(charactersNUpdate.size() > 0){
		charactersNUpdate.erase(charactersNUpdate.begin(), charactersNUpdate.end());
	}
}

void GPU::updateScreen() {
	SDL_RenderCopy(screen->renderer, characterBuffer, nullptr, nullptr);
	SDL_RenderPresent(screen->renderer);
}

void convertByte(int number, byte& b1, byte& b2, byte& b3) {
	b1 = number & 0xff;
	b2 = (byte)(number >> 8);
	b3 = (byte)(number >> 16);
}

void GPU::startCores() {
	for (int i = 0; i < cores.size(); i++) {
		if (tasks.size() > 0) {
			cores[i].programCounter = 0;
			cores[i].idX = tasks[0].x;
			cores[i].idY = tasks[0].y;
			tasks.erase(tasks.begin());
			cores[i].halt = false;
		}else{
			cores[i].idX = 0;
			cores[i].idY = 0;
		}
	}
}

void GPU::tick() {
	if(started) {

		for (int i = 0; i < cores.size(); i++) {
			if (cores[i].halt == true) {
				if (currentTask < tasks.size()) {
					cores[i].programCounter = 0;
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
