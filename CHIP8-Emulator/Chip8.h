#pragma once

#include <iostream>
#include <cstdint>
#include <array>
#include <random>

class Chip8 {
public:
	std::uint16_t opcode{};

	//Index register
	std::int16_t i{};

	//Program Counter
	std::int16_t pc{};

	//Stack Counter
	std::uint16_t sp{};

	std::array<std::uint16_t, 16 > stack{};

	//4K memory
	std::array<std::uint8_t, 4096 >memory{};

	// 16 registers
	std::array<std::uint8_t, 16 >V{};

	// Graphics 
	std::array<std::uint8_t, 64 * 32 > gfx{};

	std::uint8_t delay_timer{};
	std::uint8_t sound_timer{};

	//random number
	std::mt19937 mt{};

	//Keypad
	std::array<bool, 16 >key{};

	std::array<std::uint8_t, 80> fontset{};

	//Draw flag
	bool drawFlag{true};
	bool clearFlag{false};

	bool firstDraw{ false };

	Chip8();
	void cycle();
	void initialize();

	bool loadRom(std::string);
	std::uint8_t genRandom(int start, int end);
};
