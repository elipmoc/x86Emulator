#pragma once
#include <cstdint>
#include <memory>
#include <cstring>
#include <fstream>
#include <functional>
#include <sstream>
#include <iomanip>

namespace x86 {


	std::wostream& hex08_manip(std::wostream& ost) {
		return ost << std::setfill(L'0') << std::setw(5) << std::hex;
	}

	struct Register
	{
		static constexpr unsigned EAX = 0;
		static constexpr unsigned ECX = 1;
		static constexpr unsigned EDX = 2;
		static constexpr unsigned EBX = 3;
		static constexpr unsigned ESP = 4;
		static constexpr unsigned EBP = 5;
		static constexpr unsigned ESI = 6;
		static constexpr unsigned EDI = 7;
		static constexpr unsigned REGISTERS_COUNT = 8;
		static constexpr wchar_t* name[] = {
			L"EAX", L"ECX", L"EDX", L"EBX", L"ESP", L"EBP", L"ESI", L"EDI" };
	};


	class Emulator {
	private:
		//汎用レジスタ
		uint32_t registers[Register::REGISTERS_COUNT];

		//EFLAGSレジスタ
		uint32_t eflags;

		//メモリ(バイト列)
		std::unique_ptr<uint8_t[]> memory;

		//プログラムカウンタ
		uint32_t eip;

		std::function<void()> instructions[256];

		const size_t memorySize;

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

		void InitInstructions() {
			for (int i = 0; i < 8; i++) {
				instructions[0xB8 + i] = [this](){this->mov_r32_imm32(); };
			}
			instructions[0xEB] = [this]() {this->short_jump(); };
		}

	public:

		Emulator(size_t size, uint32_t eip, uint32_t esp)
			:memory(std::make_unique<uint8_t[]>(size)),
			//レジスタの初期値を指定されたものにする
			eip(eip),
			memorySize(size)
		{
			//汎用レジスタの初期値を全て0にする
			std::memset(registers, 0, sizeof(registers));
			this->registers[Register::ESP] = esp;

			InitInstructions();
		}

		template<class CharT,class Traits = std::char_traits<CharT>>
		void Read(std::basic_istream<CharT,Traits>& ifs) {
			ifs.read(reinterpret_cast<char*>(memory.get()), 512);
		}

		template<class ByteIterator>
		void Read(ByteIterator begin, ByteIterator end) {
			size_t i = 0;
			for (auto itr = begin; itr != end && i<512; ++itr, i++) {
				*(memory.get() + i ) = *itr;
			}
		}

		template<class Success, class Fail,class Trace>
		void Exeute(Success success,Fail fail,Trace trace) {
			while (eip < memorySize) {
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
			for (size_t i = 0; i < Register::REGISTERS_COUNT; i++) {
				ss<<std::wstring(Register::name[i])<<" = "<<hex08_manip<< registers[i]<<std::endl;
			}

			ss<<"EIP = "<< hex08_manip<<eip<<std::endl;
			return ss.str();

		}

	};


}