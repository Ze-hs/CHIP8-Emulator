#pragma once

#include <iostream>
#include <cstdint>
#include <array>
#include <random>

class Chip8 {
public:
	std::uint16_t opcode{};	// opcode 
	std::int16_t i{};		// Index register
	std::int16_t pc{};		// Program Counter
	std::uint16_t sp{};		// Stack Counter

	std::array<std::uint16_t, 16 > stack{};		// Memory Stack
	std::array<std::uint8_t, 4096 >memory{};	// 4K memory
	std::array<std::uint8_t, 16 > registers{};	// Registers
	std::array<std::uint8_t, 64 * 32 > gfx{};	// Screen buufer array
	std::array<std::uint8_t, 80> fontset{};		// Fontset array
	std::array<bool, 16 >key{};					// Keypad array

	std::uint8_t delay_timer{};		
	std::uint8_t sound_timer{};		

	std::mt19937 mt{}; 		// Random number
	std::uint8_t pixel{};	//

	//Flags
	bool drawFlag{};

	//Helper values extracted from Opcode
	std::uint8_t N{};
	std::uint8_t NN{};
	std::uint16_t NNN{};

	std::uint8_t X{};
	std::uint8_t Y{};

	bool isKeyPressed{ false }; // Checks if key has been pressed
	int keyPressed{ 0 };		// Value of key pressed

	Chip8();
	void cycle();
	void initialize();
	void fetch();
	void execute();
	bool loadRom(std::string);
	std::uint8_t genRandom(int start, int end);
};
