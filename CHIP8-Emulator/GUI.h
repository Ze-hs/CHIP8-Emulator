#pragma once

#include <SDL.h>
#include <string>
#include <iostream>

class GUI
{
private:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

public:
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Starts up SDL and creates window
	bool init();

	//Frees media and shuts down SDL
	void close();

	void drawGraphics(int x, int y, int width, int height);

	void clearScreen();

	void update();
};