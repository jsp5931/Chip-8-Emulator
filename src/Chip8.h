#pragma once
#include <iostream>
#include <cstdint>
#include <random>
#include <array>
#include <string>


using std::array;
class Chip8
{
public:
	Chip8();
	void load(char* romLocation);
	void Cycle();
	void clearAll();
	array<uint8_t, 16> input{};
	uint32_t video[64 * 32]{};

private:

	array<uint8_t,4096> ram{};
	array<uint16_t, 16> stack{};
	uint8_t stackPointer{};
	array<uint8_t, 16> registers{};
	uint16_t indexRegister{};
	uint16_t programCounter{ 0x200 };
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	uint16_t instruction{};

	std::default_random_engine generator;
	std::uniform_int_distribution<unsigned int> randNum;

	void fillFonts();
	uint8_t getThirdByte(uint16_t num);
	uint8_t getSecondByte(uint16_t num);
	uint8_t getFinal2Bytes(uint16_t num);
	
	void Instruction_DO_NOTHING();

	void Instruction_00E0();
	void Instruction_00EE();

	void Instruction_1nnn();
	void Instruction_2nnn();
	void Instruction_3xkk();
	void Instruction_4xkk();
	void Instruction_5xy0();
	void Instruction_6xkk();
	void Instruction_7xkk();
	void Instruction_9xy0();
	void Instruction_Annn();
	void Instruction_Bnnn();
	void Instruction_Cxkk();
	void Instruction_Dxyn();

	void Instruction_8xy0();
	void Instruction_8xy1();
	void Instruction_8xy2();
	void Instruction_8xy3();
	void Instruction_8xy4();
	void Instruction_8xy5();
	void Instruction_8xy6();
	void Instruction_8xy7();
	void Instruction_8xyE();

	void Instruction_Ex9E();
	void Instruction_ExA1();

	void Instruction_Fx07();
	void Instruction_Fx0A();
	void Instruction_Fx15();
	void Instruction_Fx18();
	void Instruction_Fx1E();
	void Instruction_Fx29();
	void Instruction_Fx33();
	void Instruction_Fx55();
	void Instruction_Fx65();

	void find_0Instruction();
	void find_8Instruction();
	void find_EInstruction();
	void find_FInstruction();


	typedef void (Chip8::* Instructions)();
	Instructions _InstructionsTable[0xF + 1]{ &Chip8::Instruction_DO_NOTHING };
	Instructions _0InstructionsTable[0xE + 1]{ &Chip8::Instruction_DO_NOTHING };
	Instructions _8InstructionsTable[0xE + 1]{ &Chip8::Instruction_DO_NOTHING };
	Instructions _EInstructionsTable[0xE + 1]{ &Chip8::Instruction_DO_NOTHING };
	Instructions _FInstructionsTable[0x65 + 1]{ &Chip8::Instruction_DO_NOTHING };
};