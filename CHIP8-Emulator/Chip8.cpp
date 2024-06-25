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

void Chip8::initialize()
{
	// Clear display	
	gfx.fill(0);
	// Clear stack
	stack.fill(0);
	// Clear registers V0-VF
	V.fill(0);
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

bool Chip8::loadRom(std::string file)
{
	std::ifstream inf{ file, std::ios::in | std::ios::binary };

	if (!inf)
	{
		// Print an error and exit
		std::cerr << "Could not open file:" << file << "\n";
		return false;
	}

	uint8_t temp{};
	int index = 512;


	inf.read(reinterpret_cast<char*>(&memory[512]), memory.size() - 512);

	return true;
}

void Chip8::cycle() 
{
	std::cout << std::hex;
	opcode = (memory[pc] << 8) | memory[pc + 1] ;
	std::cout << opcode;

	int Xindex{ (opcode & 0x0F00) >> 8 };
	int Yindex{ (opcode & 0x00F0)  >> 4};
	int value{};
	int height{};
	bool isKeyPressed{ false };
	std::uint8_t pixel{};

	std::mt19937 gen32{};
	//Decode opcodes
	//Instruction set reference taken from https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set
	//std::cerr << std::hex << std::hex << opcode << " : " << pc << "\n";
	switch (opcode & 0xF000)
	{
	case 0x0000:
		std::cout << std::hex;
		switch (opcode & 0x00FF)
		{
		case 0x00E0: //0x00E0 Clear screen to do
			std::cout << ((memory[pc] << 8)) << ": Clear the screen\n";
			gfx.fill(0);
			pc += 2;
			clearFlag = true;
			break;
		case 0x00EE: // 0x00EE Return 
			std::cout << opcode << ": Return from subroutine; SP Before:" << sp << " stack[sp]: "<< stack[sp] << "\n";
			--sp;
			pc = stack[sp];
			pc += 2;
			break;
		default:
			std::cerr << std::hex << opcode << ":" << "no opcode\n";
			pc += 2;
		}
		break;

	case 0x1000: // 1NNN Jump to address NNN
		pc = opcode & 0x0FFF;
		std::cout << opcode << ": Jump to " << pc  <<"\n";
		break;

	case 0x2000: // 2NNN Execute subroutine starting at address NNN
		std::cout << opcode << ": Subroutine at: " << (opcode & 0x0FFF) << " sp: " << sp;

		stack[sp] = pc;
		pc = opcode & 0x0FFF;
		++sp;
		std::cout << opcode << ": Subroutine at: " << (opcode & 0x0FFF) << "\n";

		break;

	case 0x3000: // 3XNN Skip the following instruction if the value of register VX equals NN
		std::cout << opcode << ": Skip if VX == NN  VX:" << V[Xindex] << "NN" << (opcode & 0x00FF) << "\n";
		if (V[Xindex] == (opcode & 0x00FF))
		{
			pc += 4;
			break;
			std::cout << "Skipped\n";
		}

		pc += 2;
		break;

	case 0x4000: // 4XNN Skip the following instruction if the value of register VX is not equal to NN
		std::cout << opcode << ": Skip if VX != NN  VX:" << V[Xindex] << "NN" << (opcode & 0x00FF) << "\n";
		if (V[Xindex] != (opcode & 0x00FF))
		{
			pc += 4;
			break;
			std::cout << "Skipped\n";
		}

		pc += 2;
		break;

	case 0x5000: // 5XNN Skip the following instruction if the value of register VX is equal to the value of register VY
		std::cout << opcode << ": Skip if VX != VY  VX:" << V[Xindex] << "VY" << V[Yindex] << "\n";
		if (V[Xindex] == V[Yindex])
		{
			pc += 4;
			break;
			std::cout << "Skipped\n";

		}

		pc += 2;
		break;

	case 0x6000: // 6XNN Store number NN in register VX
		std::cout << opcode << ": Store number in VX  Number:" << (opcode & 0x00FF) << " X: " << Xindex;

		value = opcode & 0x00FF;
		V[Xindex] = static_cast<std::uint8_t>(value);
		pc += 2;
		std::cout << " register after: " << V[Xindex] << "\n";
		break;

	case 0x7000: // 7XNN Add the value NN to register 
		std::cout << opcode << ": Add number to VX: " << (opcode & 0x00FF) << " X: " << Xindex << "\n";
		value = opcode & 0x00FF;

		V[Xindex] += static_cast<std::uint8_t>(value);
		pc += 2;
		break;
	
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000: // 8XY0 Store the value of register VY in register VX
			V[Xindex] = V[Yindex];
			pc += 2;
			break;
		case 0x0001: // 8XY1 Set VX to VX | VY
			V[Xindex] = V[Xindex] | V[Yindex];
			pc += 2;
			break;
		case 0x0002: // 8XY2 Set VX to VX & VY
			V[Xindex] = V[Xindex] & V[Yindex];
			pc += 2;
			break;
		case 0x0003: // 8XY3 Set VX to VX XOR VY
			V[Xindex] = V[Xindex] ^ V[Yindex];
			pc += 2;
			break;
		case 0x0004: // 8XY4 Add the value of register VY to register VX and set VF carry
		{
			uint8_t change = 1;

			if (V[Xindex] + V[Yindex] > 0xFF)
			{
				change = 0x01;
			}
			else
			{
				change = 0x00;
			}
			V[Xindex] = V[Xindex] + V[Yindex];
			V[0xF] = change;
			pc += 2;
			break;
		}
		case 0x0005:
		{
			uint8_t change = 1;

			if (V[Xindex] >= V[Yindex])
			{
				change = 0x01;
			}
			else
			{
				change = 0x00;
			}
			V[Xindex] = V[Xindex] - V[Yindex];
			V[0xF] = change;
			pc += 2;
			break;
		}
		case 0x0006:
		{
			uint8_t temp{ V[Yindex] & 0x01u };

			V[Xindex] = V[Yindex] >> 1;
			V[0xF] = temp;
			pc += 2;
			break;
		}
		case 0x0007:
		{
			uint8_t temp{ 0 };

			if (V[Xindex] <= V[Yindex])
			{
				temp = 0x01;
			}
			else
			{
				temp = 0x00;
			}
			V[Xindex] = V[Yindex] - V[Xindex];
			V[0xF] = temp;
			pc += 2;
			break;
		}
		case 0x000E:
		{
			uint8_t temp{ (0x80u & V[Xindex]) >> 7u };
			V[Xindex] = V[Yindex] << 1;

			V[0xF] = temp;
			pc += 2;
			break;
		}
	}
	break;

	case 0x9000:
		if (V[Xindex] != V[Yindex])
		{
			pc += 4;
		}
		else
		{
			pc += 2;
		}
		break;

	case 0xA000: // ANNN Store memory address NNN in register I

		i = opcode & 0x0FFF;
		pc += 2;

		std::cout << opcode << ": Store memory address NNN in register I; I:" << i << "\n";

		break;

	case 0xB000:
		value = opcode & 0x0FFF;
		pc = value + V[0x00];
		break;

	case 0xC000:
		value = opcode & 0x00FF;
		value = genRandom(0x00, 0xFF) & value;
		V[Xindex] = value;
		break;

	case 0xD000: // Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
		//Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
		std::cout << opcode << ": Draw a sprite at position VX, VY;  VX:" << V[Xindex] << " VY:" << V[Yindex] << " Xindex " << Xindex <<"\n";

		V[0xF] = 0;
		height = opcode & 0x000F;

		for (int row = 0; row < height; row++)
		{
			//Check memory bit at I + row #
			pixel = memory[i + row];

			for (int bit = 0; bit < 8; bit++)
			{
				//If bit at i is 0 then switch it
				if ((pixel & (0x80 >> bit)) != 0)
				{
					//std::cout << std::dec << "GFX: " <<  (V[Yindex] + row) * 64 + V[Xindex] + bit << "\n";
					if (gfx[(V[Yindex] + row) * 64 + V[Xindex] + bit] > 64 * 32)
					{
						continue;
					}

					if (gfx[(V[Yindex] + row) * 64 + V[Xindex] + bit] == 1)
					{
						V[0xF] = 1;
						//std::cout << std::hex << opcode << std::dec << ": " << Xindex + row << " : " << (Yindex + bit) << "\n";
					}
					gfx[(V[Yindex ] + row )* 64 + V[Xindex] + bit] ^= 1;
				}


			}
		}

		pc += 2;
		break;

	case 0xE000:
		switch (opcode & 0x00FF)
		{
		case 0x009E:
			if (key[V[Xindex]])
			{
				pc += 4;
				break;
			}
			pc += 2;
			break;

		case 0x00A1:
			if (!key[V[Xindex]])
			{
				pc += 4;
				break;
			}
			pc += 2;
			break;
		}
		break;

	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			V[Xindex] = delay_timer;
			pc += 2;
			break;
		case 0x000A:
			isKeyPressed = false;

			for (int i = 0; i < key.size(); i++)
			{
				if (key[i])
				{
					isKeyPressed = true;
					V[Xindex] = i;
					break;
				}
			}
			
			if(!isKeyPressed)
			{
				pc -= 2;
			}
			pc += 2;
			std::cout << "waiting for press\n";

			break;
		case 0x0015: // Set the delay timer to the value of register VX
			delay_timer = V[Xindex];
			pc += 2;
			break;
		case 0x0018: // Set the sound timer to the value of register VX
			sound_timer = V[Xindex];
			pc += 2;
			break;
		case 0x001E: // Add the value stored in register VX to register I
			i += V[Xindex];
			pc += 2;
			break;
		case 0x0029: // Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
			i = V[Xindex] * 5;
			pc += 2;
			break;
		case 0x0033:// Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
			memory[i] = V[Xindex] / 100;
			memory[i + 1] = V[Xindex] / 10 % 10;
			memory[i + 2] = V[Xindex] % 10;
			pc += 2;
			break;
		case 0x0055: // Store the values of registers V0 to VX inclusive in memory starting at address I is set to I + X + 1 after operation²
			for (int index = 0; index <= Xindex; ++index)
			{
				memory[i + index] = V[index];
			}
			i += 16;
			pc += 2;
			break;
		case 0x0065: // Fill registers V0 to VX inclusive with the values stored in memory starting at address I I is set to I + X + 1 after operation²
			std::cout << opcode << ": Fill registers V0 to VX";

			for (int index = 0; index <= Xindex; ++index)
			{
				V[index] = memory[i + index];
			}
			i += 16;
			pc += 2;
			break;
		default:
			std::cerr << "No opcode found";
		}

	}
}

std::uint8_t Chip8::genRandom(int start, int end)
{
	std::uniform_int_distribution random{ start, end };
	return random(mt);
}

