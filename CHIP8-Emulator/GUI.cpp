#include "GUI.h"
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
	return false;
}

//Frees media and shuts down SDL
void GUI::close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void GUI::drawGraphics(int x, int y, int width, int height)
{
	SDL_Rect fillRect{ x, y, width, height };
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(gRenderer, &fillRect);

}

void GUI::clearScreen()
{
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(gRenderer);
}

void GUI::update()
{
	//Update screen
	SDL_RenderPresent(gRenderer);
}
