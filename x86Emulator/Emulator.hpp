#pragma once
#include <cstdint>
#include <memory>
#include <cstring>
#include <fstream>

namespace x86 {


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

	public:

		Emulator(size_t size, uint32_t eip, uint32_t esp)
			:memory(std::make_unique<uint8_t[]>(size)),
			//レジスタの初期値を指定されたものにする
			eip(eip)
		{
			//汎用レジスタの初期値を全て0にする
			std::memset(registers, 0, sizeof(registers));
			this->registers[Register::ESP] = esp;
		}
		template<class CharT,class Traits = std::char_traits<CharT>>
		void Read(std::basic_istream<CharT,Traits>& ifs) {
			ifs.read(reinterpret_cast<char*>(memory.get()), 512);
		}
	};
}