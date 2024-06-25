#include "Chip8.h"
#include <iostream>
#include <SDL.h>


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
		}
	}

	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
}


int main(int argc, char* args[])
{

	Chip8 emulator{};
	emulator.initialize();
	//::cout << emulator.memory[1118 + 11] << "\n";
	emulator.loadRom("keypad.rom");
	//std::cout << emulator.memory[1118 + 11] << "\n";
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;


			//While application is running
			while (!quit)
			{
				emulator.cycle();
				//std::cout << std::hex << emulator.opcode << ": " << emulator.memory[1118 + 11] << "\n";
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
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
				

				//if (emulator.clearFlag)
				
					SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
					SDL_RenderClear(gRenderer);
					emulator.clearFlag = false;
				
				//Clear screen

				//Render red filled quad
				for (int row = 0; row < 32; row++)
				{
					for (int col = 0; col < 64; col++)
					{
						if (emulator.gfx[row*64 + col] != 0)
						{
							SDL_Rect fillRect = { col*10, row * 10,10 ,10 };
							SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
							SDL_RenderFillRect(gRenderer, &fillRect);
						}
					}
				}

				//Update screen
				SDL_RenderPresent(gRenderer);
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
