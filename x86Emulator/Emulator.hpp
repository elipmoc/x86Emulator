#pragma once

#include "Registers.hpp"
#include "Memory.hpp"
#include <cstring>
#include <fstream>
#include <functional>
#include <sstream>
#include <iomanip>

namespace x86 {


	//カスタムマニュピレータ
	std::wostream& hex08_manip(std::wostream& ost) {
		return ost << std::setfill(L'0') << std::setw(5) << std::hex;
	}

	class Emulator {
	private:

		const uint32_t start_eip;

		//汎用レジスタ
		Registers registers;

		//EFLAGSレジスタ
		uint32_t eflags;

		//メモリ(バイト列)
		Memory memory;

		//プログラムカウンタ
		uint32_t eip;

		std::function<void()> instructions[256];

		uint32_t getCode8(const int index)const {
			return memory[eip + index];
		}

		int32_t getSignCode8(const int index)const {
			return static_cast<int8_t>(getCode8(index));
		}

		uint32_t getCode32(const int index)const {
			uint32_t ret = 0;

			/* リトルエンディアンでメモリの値を取得する */
			for (int i = 0; i < 4; i++) {
				ret |= getCode8(index + i) << (i * 8);
			}

			return ret;
		}

		int32_t getSignCode32(const int index)const {
			return static_cast<int32_t>(getCode32(index));
		}

		void mov_r32_imm32() {
			const uint8_t reg = getCode8(0) - 0xB8;
			const uint32_t value = getCode32(1);
			registers[reg] = value;
			eip += 5;
		}

		void short_jump() {
			const int8_t diff = getSignCode8(1);
			eip += (diff + 2);
		}

		void near_jump() {
			const int32_t diff = getSignCode32(1);
			eip += (diff + 5);
		}

		void InitInstructions() {
			for (int i = 0; i < 8; i++) {
				instructions[0xB8 + i] = [this](){this->mov_r32_imm32(); };
			}
			instructions[0xEB] = [this]() {this->short_jump(); };
			instructions[0xE9] = [this]() {this->near_jump(); };
		}

	public:

		Emulator(size_t size, uint32_t eip, uint32_t esp)
			:memory(size),
			//レジスタの初期値を指定されたものにする
			eip(eip),
			start_eip(eip)
		{
			registers[Registers::ESP] = esp;

			InitInstructions();
		}

		template<class CharT,class Traits = std::char_traits<CharT>>
		void Read(std::basic_istream<CharT,Traits>& ifs) {
			ifs.read(reinterpret_cast<char*>(&memory[start_eip]), 512);
		}

		template<class ByteIterator>
		void Read(ByteIterator begin, ByteIterator end) {
			size_t i = start_eip;
			for (auto itr = begin; itr != end && i<512+start_eip; ++itr, i++) {
				memory[i] = *itr;
			}
		}

		template<class Success, class Fail,class Trace>
		void Exeute(Success success,Fail fail,Trace trace) {
			while (eip < memory.memorySize) {
				uint8_t code = getCode8(0);
				std::wstringstream ss;
				ss << "EIP = " << std::hex << eip << ", Code = " << std::hex << code<<std::endl;
				/* 現在のプログラムカウンタと実行されるバイナリを出力する */
				trace(ss.str());
				if (instructions[code] == false) {
					fail();
					break;
				}

				/*命令の実行*/
				instructions[code]();
				if (eip == 0x00) {
					success();
					break;
				}
			}
		}

		/* 汎用レジスタとプログラムカウンタの値を標準出力に出力する */
		std::wstring Dump_registers()const
		{
			std::wstringstream ss;
			for (size_t i = 0; i < Registers::REGISTERS_COUNT; i++) {
				ss<<std::wstring(Registers::name[i])<<" = "<<hex08_manip<< registers[i]<<std::endl;
			}

			ss<<"EIP = "<< hex08_manip<<eip<<std::endl;
			return ss.str();
		}
	};
}