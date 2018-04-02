#pragma once
#include <cstdint>

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
		uint8_t* memory;

		//プログラムカウンタ
		uint32_t eip;
	};
}