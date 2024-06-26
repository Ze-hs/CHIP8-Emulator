#include "Chip8.h"
#include <iostream>
#include <cstdint>
#include <array>
#include <fstream>
#include <random>
#include <algorithm> 
#include <windows.h> 

Chip8::Chip8() :
	opcode{ 0 },
	i{ 0 },
	pc{ 0x200u },
	sp{ 0 }	,
	memory{}
{}

void Chip8::initialize() {
	// Clear display	
	gfx.fill(0);
	// Clear stack
	stack.fill(0);
	// Clear registers V0-VF
	registers.fill(0);
	// Clear memory
	memory.fill(0);
	//reset timers

	fontset = 
	{
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	//Load fontset
	std::copy(fontset.begin(), fontset.end(), memory.begin());

}

bool Chip8::loadRom(std::string file) {
	std::ifstream inf{ file, std::ios::in | std::ios::binary };

	if (!inf) {
		// Print an error and exit
		std::cerr << "Could not open file:" << file << "\n";
		return false;
	}

	int index = 512;
	inf.read(reinterpret_cast<char*>(&memory[512]), memory.size() - 512);
	return true;
}

void Chip8::fetch() {
	opcode = (memory[pc] << 8) | memory[pc + 1];

	//Set helper vars
	N = opcode & 0x000F;
	NN = opcode & 0x00FF;
	NNN = opcode & 0x0FFF;

	X = (opcode & 0x0F00) >> 8;
	Y = (opcode & 0x00F0) >> 4;
}

void Chip8::execute() {
	//Instruction set reference taken from https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set
	switch (opcode & 0xF000) {
	case 0x0000:
		switch (opcode & 0x00FF) {
		case 0x00E0: //0x00E0 Clear screen to do
			drawFlag = true;
			gfx.fill(0);
			break;
		case 0x00EE: // 0x00EE Return 
			--sp;
			pc = stack[sp];
			break;
		default:
			std::cerr << std::hex << opcode << ":" << "no opcode\n";
		}
		break;

	case 0x1000: // 1NNN Jump to address NNN
		pc = opcode & 0x0FFF;
		break;

	case 0x2000: // 2NNN Execute subroutine starting at address NNN

		stack[sp] = pc;
		pc = opcode & 0x0FFF;
		++sp;
		break;

	case 0x3000: // 3XNN Skip the following instruction if the value of register VX equals NN
		if (registers[X] == (opcode & 0x00FF)){
			pc += 2;
		}

		break;

	case 0x4000: // 4XNN Skip the following instruction if the value of register VX is not equal to NN
		if (registers[X] != (opcode & 0x00FF)){
			pc += 2;
		}

		break;

	case 0x5000: // 5XNN Skip the following instruction if the value of register VX is equal to the value of register VY
		if (registers[X] == registers[Y]) {
			pc += 2;
		}
		break;

	case 0x6000: // 6XNN Store number NN in register VX
		registers[X] = static_cast<std::uint8_t>(NN);
		break;

	case 0x7000: // 7XNN Add the value NN to register 
		registers[X] += static_cast<std::uint8_t>(NN);
		break;

	case 0x8000:
		switch (opcode & 0x000F) {
		case 0x0000: // 8XY0 Store the value of register VY in register VX
			registers[X] = registers[Y];
			break;
		case 0x0001: // 8XY1 Set VX to VX | VY
			registers[X] = registers[X] | registers[Y];
			break;
		case 0x0002: // 8XY2 Set VX to VX & VY
			registers[X] = registers[X] & registers[Y];
			break;
		case 0x0003: // 8XY3 Set VX to VX XOR VY
			registers[X] = registers[X] ^ registers[Y];
			break;
		case 0x0004: { // 8XY4 Add the value of register VY to register VX and set VF carry
			uint8_t temp = 1;

			if (registers[X] + registers[Y] > 0xFF) {
				temp = 0x01;
			}
			else {
				temp = 0x00;
			}
			
			registers[X] = registers[X] + registers[Y];
			registers[0xF] = temp;
			break;
		}
		case 0x0005: {
			uint8_t temp = 1;

			if (registers[X] >= registers[Y]) {
				temp = 0x01;
			}
			else {
				temp = 0x00;
			}
			registers[X] = registers[X] - registers[Y];
			registers[0xF] = temp;
			break;
		}
		case 0x0006: {
			uint8_t temp{ registers[Y] & 0x01u };

			registers[X] = registers[Y] >> 1;
			registers[0xF] = temp;
			break;
		}
		case 0x0007: {
			uint8_t temp{ 0 };

			if (registers[X] <= registers[Y]) {
				temp = 0x01;
			}
			else {
				temp = 0x00;
			}
			registers[X] = registers[Y] - registers[X];
			registers[0xF] = temp;
			break;
		}
		case 0x000E: {
			uint8_t temp{ (0x80u & registers[X]) >> 7u };
			registers[X] = registers[Y] << 1;
			registers[0xF] = temp;
			break;
		}
	}
	break;

	case 0x9000:
		if (registers[X] != registers[Y]) {
			pc += 2;
		}
		break;

	case 0xA000: // ANNN Store memory address NNN in register I
		i = opcode & 0x0FFF;
		break;

	case 0xB000:
		pc = NNN + registers[0x00];
		break;

	case 0xC000:
		registers[X] = genRandom(0x00, 0xFF) & NN;;
		break;

	case 0xD000: { // Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
		//Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
		registers[0xF] = 0;
		drawFlag = true;

		int xCoord{};
		int yCoord{};

		for (int row = 0; row < N; row++) {
			//Check memory bit at I + row #
			pixel = memory[i + row];

			for (int bit = 0; bit < 8; bit++) {
				//If bit at i is 0 then switch it
				xCoord = (registers[X] + bit) % 64;
				yCoord = (registers[Y] + row) % 32;
				if ((pixel & (0x80 >> bit)) != 0) {
					if (gfx[yCoord * 64 + xCoord] > 64 * 32) {
						continue;
					}

					if (gfx[yCoord * 64 + xCoord] == 1) {
						registers[0xF] = 1;
					}
					gfx[yCoord * 64 + xCoord] ^= 1;
				}
			}
		}

		break;
	}
	case 0xE000:
		switch (opcode & 0x00FF) {
		case 0x009E:
			if (key[registers[X]]) {
				pc += 2;
				break;
			}
			break;

		case 0x00A1:
			if (!key[registers[X]]) {
				pc += 2;
				break;
			}
			break;
		}
		break;

	case 0xF000:
		switch (opcode & 0x00FF) {
		case 0x0007:
			registers[X] = delay_timer;
			break;
		case 0x000A:
			//Check if previous was 
			for (int i = 0; i < key.size(); i++) {
				if (key[i]) {
					isKeyPressed = true;
					keyPressed = i;
					break;
				}
			}

			if (!isKeyPressed) {
				pc -= 2;
			}
			else {
				if (key[keyPressed]) {
					pc -= 2;
				}
				else {
					registers[X] = keyPressed;
					keyPressed = 0x0;
					isKeyPressed = false;
				}
			}

			break;
		case 0x0015: // Set the delay timer to the value of register VX
			delay_timer = registers[X];
			break;
		case 0x0018: // Set the sound timer to the value of register VX
			sound_timer = registers[X];
			break;
		case 0x001E: // Add the value stored in register VX to register I
			i += registers[X];
			break;
		case 0x0029: // Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
			i = registers[X] * 5;
			break;
		case 0x0033:// Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
			memory[i] = registers[X] / 100;
			memory[i + 1] = registers[X] / 10 % 10;
			memory[i + 2] = registers[X] % 10;
			break;
		case 0x0055: // Store the values of registers V0 to VX inclusive in memory starting at address I is set to I + X + 1 after operation²
			for (int index = 0; index <= X; ++index) {
				memory[i + index] = registers[index];
			}
			i += 16;
			break;
		case 0x0065: // Fill registers V0 to VX inclusive with the values stored in memory starting at address I I is set to I + X + 1 after operation²
			for (int index = 0; index <= X; ++index) {
				registers[index] = memory[i + index];
			}
			i += 16;
			break;

		default:
			std::cerr <<std::hex << "Corresponding Opcode Not Found";
		}

	}
}

void Chip8::cycle() {
	//fetch
	fetch();
	//std::cout << std::hex << opcode << "\n";

	pc += 2;

	//Execute
	execute();

	if (delay_timer > 0){
		--delay_timer;
	}

	if (sound_timer > 0) {
		--sound_timer;
	}
}

std::uint8_t Chip8::genRandom(int start, int end) {
	std::uniform_int_distribution random{ start, end };
	return random(mt);
}

