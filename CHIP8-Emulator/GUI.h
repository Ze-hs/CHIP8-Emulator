#pragma once
#include "Chip8.h"
#include <SDL.h>
#include <string>
#include <iostream>


class GUI
{
private:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 320;


public:

	bool quit{};
	SDL_Event e{};
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Starts up SDL and creates window
	bool init();

	//Frees media and shuts down SDL
	void close();

	void clearScreen();
	void update();
	void handleInput(Chip8& emulator);
	void drawGraphics(const std::array<std::uint8_t, 2048>& gfx);
};