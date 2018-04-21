#pragma once

#include "Registers.hpp"
#include "Memory.hpp"
#include "CodeFetch.hpp"
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
	
	struct ModRM {
		uint8_t mod;
		union {
			uint8_t opecode;
			uint8_t reg_index;
		};
		uint8_t rm;
		uint8_t sib;
		union {
			int8_t disp8;
			uint32_t disp32;
		};
	};

	class Emulator {
	private:

		const uint32_t start_eip;

		//汎用レジスタ
		Registers registers;

		//EFLAGSレジスタ
		uint32_t eflags;

		//メモリ
		Memory memory;

		//メモリ読み出しに使用
		CodeFetch codeFetch;

		std::function<void()> instructions[256];

		ModRM parseModRM() {
			uint8_t code;
			ModRM modrm;
			memset(&modrm, 0, sizeof(ModRM));
			code = codeFetch.getCode8(0);
			modrm.mod = ((code & 0xC0) >> 6);//    C0=1100 0000
			modrm.opecode = ((code & 0x38) >> 3);//38=0011 1000
			modrm.rm = code & 0x07;//              07=0000 0111
			codeFetch.addEip(1);

			if (modrm.mod != 3 && modrm.rm == 4) {
				modrm.sib = codeFetch.getCode8(0);
				codeFetch.addEip(1);
			}

			if ((modrm.mod == 0 && modrm.rm == 5) || modrm.mod == 2) {
				modrm.disp32 = codeFetch.getSignCode32(0);
				codeFetch.addEip(4);
			}
			else if (modrm.mod == 1) {
				modrm.disp8 = codeFetch.getSignCode8(0);
				codeFetch.addEip(1);
			}


		}

		void set_rm32(const ModRM &modrm, uint32_t value) {
			if (modrm.mod == 3)
				registers.set_register32(modrm.rm, value);
			else {
				uint32_t address = calc_memory_address(modrm);
				memory.set_memory32(address, value);
			}
		}

		uint32_t calc_memory_address(const ModRM &modrm) {
			if (modrm.mod == 0) {
				if (modrm.rm == 4) {
					throw L"not implemeted ModRM mod=0 rm=4 \n";
					exit(0);
				}
				else if (modrm.rm == 5)
					return modrm.disp32;
				else
					return registers.get_register32(modrm.rm);
			}
			else if (modrm.mod == 1) {
				if (modrm.rm == 4) {
					throw L"not implemeted ModRM mod=1 rm=4 \n";
					exit(0);
				}
				else
					return registers.get_register32(modrm.rm) + modrm.disp8;
			}
			else if (modrm.mod == 2) {
				if (modrm.rm == 4) {
					throw L"not implemeted ModRM mod=2 rm=4 \n";
					exit(0);
				}
				else
					return registers.get_register32(modrm.rm) + modrm.disp32;
			}
			else {
				throw L"not implemeted ModRM mod=3 \n";
				exit(0);
			}
		}

		void mov_r32_imm32() {
			const uint8_t reg = codeFetch.getCode8(0) - 0xB8;
			const uint32_t value =codeFetch.getCode32(1);
			registers.set_register32(reg,value);
			codeFetch.addEip(5);
		}
		void mov_rm32_imm32() {
			codeFetch.addEip(1);
			ModRM modrm = parseModRM();
			uint32_t value = codeFetch.getCode32(0);
			codeFetch.addEip(4);
			set_rm32(modrm, value);
		}

		void short_jump() {
			const int8_t diff =codeFetch.getSignCode8(1);
			codeFetch.addEip(diff + 2);
		}

		void near_jump() {
			const int32_t diff = codeFetch.getSignCode32(1);
			codeFetch.addEip(diff + 5);
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
			codeFetch(eip,memory),
			start_eip(eip)
		{
			registers.set_register32(Registers::ESP, esp);

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
			while (codeFetch.getEip() < memory.memorySize) {
				uint8_t code = codeFetch.getCode8(0);
				std::wstringstream ss;
				ss << "EIP = " << std::hex << codeFetch.getEip() << ", Code = " << std::hex << code<<std::endl;
				/* 現在のプログラムカウンタと実行されるバイナリを出力する */
				trace(ss.str());
				if (instructions[code] == false) {
					fail();
					break;
				}

				/*命令の実行*/
				instructions[code]();
				if (codeFetch.getEip()== 0x00) {
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
				ss<<std::wstring(Registers::name[i])<<" = "<<hex08_manip<< registers.get_register32(i)<<std::endl;
			}

			ss<<"EIP = "<< hex08_manip<<codeFetch.getEip()<<std::endl;
			return ss.str();
		}
	};
}