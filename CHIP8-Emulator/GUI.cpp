#include "GUI.h"
#include "Chip8.h"
#include <SDL.h>
#include <string>
#include <iostream>

bool GUI::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Could not initialize SDL, SDL Error:  " << SDL_GetError() << "\n";
		return false;
	}

	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		std::cerr << "Window could not be created! SDL Error:  " << SDL_GetError() << "\n";
		return false;
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		std::cerr << "Renderer could not be created! SDL Error:  " << SDL_GetError() << "\n";
		return false;
	}

	//Set default white color:
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	return true;
}

//Frees media and shuts down SDL
void GUI::close() {
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void GUI::clearScreen() {
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(gRenderer);
}

void GUI::update() {
	SDL_RenderPresent(gRenderer);
}

void GUI::handleInput(Chip8& emulator) {
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			//Select surfaces based on key press
			switch (e.key.keysym.sym)
			{
			case SDLK_1: emulator.key[1] = true; break;
			case SDLK_2: emulator.key[2] = true; break;
			case SDLK_3: emulator.key[3] = true; break;
			case SDLK_4: emulator.key[12] = true; break;
			case SDLK_q: emulator.key[4] = true; break;
			case SDLK_w: emulator.key[5] = true; break;
			case SDLK_e: emulator.key[6] = true; break;
			case SDLK_r: emulator.key[13] = true; break;
			case SDLK_a: emulator.key[7] = true; break;
			case SDLK_s: emulator.key[8] = true; break;
			case SDLK_d: emulator.key[9] = true; break;
			case SDLK_f: emulator.key[14] = true; break;
			case SDLK_z: emulator.key[10] = true; break;
			case SDLK_x: emulator.key[0] = true; break;
			case SDLK_c: emulator.key[11] = true; break;
			case SDLK_v: emulator.key[15] = true; break;
			default:
				break;
			}
		}
		else if (e.type == SDL_KEYUP)
		{
			switch (e.key.keysym.sym)
			{
				//Select surfaces based on key press
			case SDLK_1: emulator.key[1] = false; break;
			case SDLK_2: emulator.key[2] = false; break;
			case SDLK_3: emulator.key[3] = false; break;
			case SDLK_4: emulator.key[12] = false; break;
			case SDLK_q: emulator.key[4] = false; break;
			case SDLK_w: emulator.key[5] = false; break;
			case SDLK_e: emulator.key[6] = false; break;
			case SDLK_r: emulator.key[13] = false; break;
			case SDLK_a: emulator.key[7] = false; break;
			case SDLK_s: emulator.key[8] = false; break;
			case SDLK_d: emulator.key[9] = false; break;
			case SDLK_f: emulator.key[14] = false; break;
			case SDLK_z: emulator.key[10] = false; break;
			case SDLK_x: emulator.key[0] = false; break;
			case SDLK_c: emulator.key[11] = false; break;
			case SDLK_v: emulator.key[15] = false; break;
			default:
				break;
			}
		}
	}
	
}

void GUI::drawGraphics(const std::array<std::uint8_t, 2048>& gfx) {
	for (int row = 0; row < 32; row++)
	{
		for (int col = 0; col < 64; col++)
		{
			if (gfx[row * 64 + col] != 0)
			{
				SDL_Rect fillRect = { col * 10, row * 10,10 ,10 };
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderFillRect(gRenderer, &fillRect);
			}
		}
	}

}