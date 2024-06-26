#include "Chip8.h"
#include "GUI.h"
#include <iostream>
#include <SDL.h>

int main(int argc, char* args[]) {
	GUI screen{};


	if (!screen.init())
	{
		return 0;
	}

	Chip8 emulator{};
	emulator.initialize();
	emulator.loadRom("pong.rom");

	//Main loop
	while (!screen.quit)
	{
		// Get time before running instructions 
		const uint64_t start_time = SDL_GetPerformanceCounter();

		for (int i = 0; i < 500 / 60; i++) {
			emulator.cycle();
			screen.handleInput(emulator);
		}

		//Delay emulator until it reached the desired hz
		const uint64_t end_time = SDL_GetPerformanceCounter();
		const double time_elapsed = (double)((end_time - start_time) * 1000) / SDL_GetPerformanceFrequency();
		SDL_Delay(16.67f > time_elapsed ? 16.67f - time_elapsed : 0);

		if (emulator.drawFlag)
		{
			screen.clearScreen();
			screen.drawGraphics(emulator.gfx);
		}
		screen.update();
	}


	screen.close();
	return 0;
}