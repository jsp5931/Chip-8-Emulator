#include "Chip8.h"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <random>


Chip8::Chip8()
	: generator(std::chrono::system_clock::now().time_since_epoch().count())
{
	randNum = std::uniform_int_distribution<unsigned int>(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max());
	fillFonts();

	_InstructionsTable[0x0] = &Chip8::find_0Instruction;
	_InstructionsTable[0x1] = &Chip8::Instruction_1nnn;
	_InstructionsTable[0x2] = &Chip8::Instruction_2nnn;
	_InstructionsTable[0x3] = &Chip8::Instruction_3xkk;
	_InstructionsTable[0x4] = &Chip8::Instruction_4xkk;
	_InstructionsTable[0x5] = &Chip8::Instruction_5xy0;
	_InstructionsTable[0x6] = &Chip8::Instruction_6xkk;
	_InstructionsTable[0x7] = &Chip8::Instruction_7xkk;
	_InstructionsTable[0x8] = &Chip8::find_8Instruction;
	_InstructionsTable[0x9] = &Chip8::Instruction_9xy0;
	_InstructionsTable[0xA] = &Chip8::Instruction_Annn;
	_InstructionsTable[0xB] = &Chip8::Instruction_Bnnn;
	_InstructionsTable[0xC] = &Chip8::Instruction_Cxkk;
	_InstructionsTable[0xD] = &Chip8::Instruction_Dxyn;
	_InstructionsTable[0xE] = &Chip8::find_EInstruction;
	_InstructionsTable[0xF] = &Chip8::find_FInstruction;


	_0InstructionsTable[0x0] = &Chip8::Instruction_00E0;
	_0InstructionsTable[0xE] = &Chip8::Instruction_00EE;

	_8InstructionsTable[0x0] = &Chip8::Instruction_8xy0;
	_8InstructionsTable[0x1] = &Chip8::Instruction_8xy1;
	_8InstructionsTable[0x2] = &Chip8::Instruction_8xy2;
	_8InstructionsTable[0x3] = &Chip8::Instruction_8xy3;
	_8InstructionsTable[0x4] = &Chip8::Instruction_8xy4;
	_8InstructionsTable[0x5] = &Chip8::Instruction_8xy5;
	_8InstructionsTable[0x6] = &Chip8::Instruction_8xy6;
	_8InstructionsTable[0x7] = &Chip8::Instruction_8xy7;
	_8InstructionsTable[0xE] = &Chip8::Instruction_8xyE;

	_EInstructionsTable[0x1] = &Chip8::Instruction_ExA1;
	_EInstructionsTable[0xE] = &Chip8::Instruction_Ex9E;


	_FInstructionsTable[0x07] = &Chip8::Instruction_Fx07;
	_FInstructionsTable[0x0A] = &Chip8::Instruction_Fx0A;
	_FInstructionsTable[0x15] = &Chip8::Instruction_Fx15;
	_FInstructionsTable[0x18] = &Chip8::Instruction_Fx18;
	_FInstructionsTable[0x1E] = &Chip8::Instruction_Fx1E;
	_FInstructionsTable[0x29] = &Chip8::Instruction_Fx29;
	_FInstructionsTable[0x33] = &Chip8::Instruction_Fx33;
	_FInstructionsTable[0x55] = &Chip8::Instruction_Fx55;
	_FInstructionsTable[0x65] = &Chip8::Instruction_Fx65;
}

void Chip8::fillFonts() {
	uint8_t fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, 
		0x20, 0x60, 0x20, 0x20, 0x70, 
		0xF0, 0x10, 0xF0, 0x80, 0xF0, 
		0xF0, 0x10, 0xF0, 0x10, 0xF0, 
		0x90, 0x90, 0xF0, 0x10, 0x10, 
		0xF0, 0x80, 0xF0, 0x10, 0xF0, 
		0xF0, 0x80, 0xF0, 0x90, 0xF0, 
		0xF0, 0x10, 0x20, 0x40, 0x40, 
		0xF0, 0x90, 0xF0, 0x90, 0xF0, 
		0xF0, 0x90, 0xF0, 0x10, 0xF0, 
		0xF0, 0x90, 0xF0, 0x90, 0x90, 
		0xE0, 0x90, 0xE0, 0x90, 0xE0, 
		0xF0, 0x80, 0x80, 0x80, 0xF0, 
		0xE0, 0x90, 0x90, 0x90, 0xE0, 
		0xF0, 0x80, 0xF0, 0x80, 0xF0, 
		0xF0, 0x80, 0xF0, 0x80, 0x80  
	};

	for (unsigned int i = 0; i < 80; i++)
	{
		ram[0x50 + i] = fontset[i];
	}
}

uint8_t Chip8::getThirdByte(uint16_t num) {
	return (num & 0x0F00) >> 8;
}

uint8_t Chip8::getSecondByte(uint16_t num) {
	return (num & 0x00F0) >> 4;
}

uint8_t Chip8::getFinal2Bytes(uint16_t num) {
	return num & 0x00FF;
}
void Chip8::clearAll() {
		stackPointer = 0;
		indexRegister = 0;
		programCounter = 0x200;
		delayTimer = 0;
		soundTimer = 0;
		instruction = 0;
		ram.fill(0);
		stack.fill(0);
		registers.fill(0);
		input.fill(0);
		fillFonts();
		std::fill(video, video + (64 * 32), 0);
	}
void Chip8::load(char* romLocation)
{

	using filepath = std::ifstream;
	filepath file(romLocation, std::ios::binary);

	if (!file)
	{
		throw std::runtime_error("CANNOT OPEN FILE");
	}
	else {
		
		file.seekg(0, filepath::end);
		std::streampos size = file.tellg();
		char* gameData = new char[size];
		file.seekg(0, std::ios::beg);
		file.read(gameData, size);
		file.close();

		for (int i = 0; i < size; i++)
		{
			ram[0x200 + i] = gameData[i];
		}
		delete[] gameData;
	}
}

void Chip8::Cycle()
{
	instruction = (ram[programCounter] << 8) | ram[programCounter+ 1];

	programCounter+= 2;

	(this->*(_InstructionsTable[(instruction & 0xF000) >> 12]))();

	if (delayTimer > 0)
	{
		--delayTimer;
	}

	if (soundTimer > 0)
	{
		--soundTimer;
	}
}

void Chip8::find_0Instruction()
{
	(this->*(_0InstructionsTable[instruction & 0x000F]))();

}

void Chip8::find_8Instruction()
{
	(this->*(_8InstructionsTable[instruction & 0x000F]))();
}

void Chip8::find_EInstruction()
{
	(this->*(_EInstructionsTable[instruction & 0x000F]))();
}

void Chip8::find_FInstruction()
{
	(this->*(_FInstructionsTable[instruction & 0x00FF]))();
}

void Chip8::Instruction_DO_NOTHING()
{
}

void Chip8::Instruction_00E0()
{
	std::fill(video, video + (64 * 32), 0);
}

void Chip8::Instruction_00EE()
{
	stackPointer--;
	programCounter= stack[stackPointer];

}

void Chip8::Instruction_1nnn()
{
	uint16_t address = instruction & 0x0FFF;
	programCounter = address;
}

void Chip8::Instruction_2nnn()
{
	uint16_t address = instruction & 0x0FFF;
	stack[stackPointer] = programCounter;
	stackPointer++;
	programCounter = address;
}

void Chip8::Instruction_3xkk(){
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t data = getFinal2Bytes(instruction);
	if (registers[x_Register] == data)
	{
		programCounter+= 2;
	}
}

void Chip8::Instruction_4xkk()
{
	uint8_t data = getFinal2Bytes(instruction);
	uint8_t x_Register = getThirdByte(instruction);

	if (registers[x_Register] != data)
	{
		programCounter+= 2;
	}
}

void Chip8::Instruction_5xy0()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);

	if (registers[x_Register] == registers[y_Register])
	{
		programCounter+= 2;
	}
}

void Chip8::Instruction_6xkk()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t data = getFinal2Bytes(instruction);

	registers[x_Register] = data;
}

void Chip8::Instruction_7xkk()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t data = getFinal2Bytes(instruction);

	registers[x_Register] += data;
}

void Chip8::Instruction_8xy0()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);

	registers[x_Register] = registers[y_Register];
}

void Chip8::Instruction_8xy1()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);

	registers[x_Register] |= registers[y_Register];
}

void Chip8::Instruction_8xy2()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);

	registers[x_Register] &= registers[y_Register];
}

void Chip8::Instruction_8xy3()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);

	registers[x_Register] ^= registers[y_Register];
}

void Chip8::Instruction_8xy4()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);

	uint16_t sum = registers[x_Register] + registers[y_Register];

	if (sum > 255U)
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[x_Register] = sum & 0xFF;
}

void Chip8::Instruction_8xy5()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);

	if (registers[x_Register] > registers[y_Register])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[x_Register] -= registers[y_Register];
}

void Chip8::Instruction_8xy6()
{
	uint8_t x_Register = getThirdByte(instruction);

	registers[0xF] = (registers[x_Register] & 0x1);

	registers[x_Register] >>= 1;
}

void Chip8::Instruction_8xy7()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);

	if (registers[y_Register] > registers[x_Register])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[x_Register] = registers[y_Register] - registers[x_Register];
}

void Chip8::Instruction_8xyE()
{
	uint8_t x_Register = getThirdByte(instruction);

	registers[0xF] = (registers[x_Register] & 0x80) >> 7;

	registers[x_Register] <<= 1;
}

void Chip8::Instruction_9xy0()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);

	if (registers[x_Register] != registers[y_Register])
	{
		programCounter+= 2;
	}
}

void Chip8::Instruction_Annn()
{
	uint16_t address = instruction & 0x0FFFu;

	indexRegister = address;
}

void Chip8::Instruction_Bnnn()
{
	uint16_t address = instruction & 0x0FFF;

	programCounter= registers[0] + address;
}

void Chip8::Instruction_Cxkk()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t data = getFinal2Bytes(instruction);

	registers[x_Register] = randNum(generator) & data;
}

void Chip8::Instruction_Dxyn()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t y_Register = getSecondByte(instruction);
	uint8_t height = instruction & 0x000F;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; row++)
	{
		uint8_t spriteByte = ram[indexRegister + row];

		for (unsigned int col = 0; col < 8; col++)
		{
			uint8_t spritePixel = spriteByte & (0x80 >> col);
			uint8_t screenX = (registers[x_Register] % 64 + col);
			uint8_t screenY = (registers[y_Register] % 32 + row);
			uint32_t* displayPixel = &video[screenY * 64 + screenX];

			if (spritePixel)
			{
				if (*displayPixel == 0xFFFFFFFF) {
					registers[0xF] = 1;
				}
				*displayPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void Chip8::Instruction_Ex9E()
{
	uint8_t x_Register = getThirdByte(instruction);

	uint8_t target = registers[x_Register];

	if (input[target])
	{
		programCounter+= 2;
	}
}

void Chip8::Instruction_ExA1()
{
	uint8_t x_Register = getThirdByte(instruction);

	uint8_t target = registers[x_Register];

	if (!input[target])
	{
		programCounter+= 2;
	}
}

void Chip8::Instruction_Fx07()
{
	uint8_t x_Register = getThirdByte(instruction);

	registers[x_Register] = delayTimer;
}

void Chip8::Instruction_Fx0A()
{
	uint8_t x_Register = getThirdByte(instruction);
	bool inputDetected = false;
	for (int i = 0;i < 16;i++) {
		if (input[i]) {
			registers[x_Register] = i;
			inputDetected = true;
		}
	}
	if (!inputDetected) {
		programCounter -= 2;
	}
}

void Chip8::Instruction_Fx15()
{
	uint8_t x_Register = getThirdByte(instruction);

	delayTimer = registers[x_Register];
}

void Chip8::Instruction_Fx18()
{
	uint8_t x_Register = getThirdByte(instruction);

	soundTimer = registers[x_Register];
}

void Chip8::Instruction_Fx1E()
{
	uint8_t x_Register = getThirdByte(instruction);

	indexRegister += registers[x_Register];
}

void Chip8::Instruction_Fx29()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t digit = registers[x_Register];

	indexRegister = 0x50 + (5 * digit);
}

void Chip8::Instruction_Fx33()
{
	uint8_t x_Register = getThirdByte(instruction);
	uint8_t value = registers[x_Register];

	
	ram[indexRegister + 2] = value % 10;
	value /= 10;

	ram[indexRegister + 1] = value % 10;
	value /= 10;

	ram[indexRegister] = value % 10;
}

void Chip8::Instruction_Fx55()
{
	uint8_t x_Register = getThirdByte(instruction);

	for (uint8_t i = 0; i <= x_Register; i++)
	{
		ram[indexRegister + i] = registers[i];
	}
}

void Chip8::Instruction_Fx65()
{
	uint8_t x_Register = getThirdByte(instruction);

	for (uint8_t i = 0; i <= x_Register; i++)
	{
		registers[i] = ram[indexRegister + i];
	}
}