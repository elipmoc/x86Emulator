#pragma once
#include <cstdint>
#include <memory>

namespace x86 {

	//汎用レジスタをまとめるクラス
	class Registers {
	public:
		static constexpr unsigned EAX = 0;
		static constexpr unsigned ECX = 1;
		static constexpr unsigned EDX = 2;
		static constexpr unsigned EBX = 3;
		static constexpr unsigned ESP = 4;
		static constexpr unsigned EBP = 5;
		static constexpr unsigned ESI = 6;
		static constexpr unsigned EDI = 7;
		static constexpr unsigned REGISTERS_COUNT = 8;
		static constexpr unsigned AL = EAX;
		static constexpr unsigned CL = ECX;
		static constexpr unsigned DL = EDX;
		static constexpr unsigned BL = EBX;
		static constexpr unsigned AH=AL+4;
		static constexpr unsigned CH=CL+4;
		static constexpr unsigned DH=DL+4;
		static constexpr unsigned BH=BL+4;
		static constexpr wchar_t* name[] = {
			L"EAX", L"ECX", L"EDX", L"EBX", L"ESP", L"EBP", L"ESI", L"EDI" };
	private:
		uint32_t registers[REGISTERS_COUNT];
	public:
		Registers() {
			//汎用レジスタの初期値を全て0にする
			std::memset(registers, 0, sizeof(registers));
		}

		uint32_t get_register32(int index)const
		{
			return registers[index];
		}

		void set_register32(int index, uint32_t value)
		{
			registers[index] = value;
		}

		uint8_t get_register8(int index)const  {
			if (index < 4) {
				return registers[index] & 0xff;
			}
			else {
				return (registers[index - 4] >> 8) & 0xff;
			}
		}

		void set_register8(int index, uint8_t value) {
			if(index<4){
				uint32_t r = registers[index] & 0xffffff00;
				registers[index] = r | value;
			}
			else {
				uint32_t r = registers[index-4] & 0xffff00ff;
				registers[index-4] = r | (static_cast<uint32_t>(value)<<8);
			}
		}
	};

}