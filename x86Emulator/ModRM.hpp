#pragma once
#include <cstdint>
namespace x86 {
	struct ModRMData {
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


	class ModRM {
		const ModRMData modrmData;
	public:
		ModRM(const ModRMData &modrmData)
			:modrmData(modrmData){}

		uint8_t get_opecode() { return modrmData.opecode; }

		//modrmDataから使用するレジスタを識別してそのレジスタの値を取得する
		uint32_t get_r32(const Registers &registers)const {
			return registers.get_register32(modrmData.reg_index);
		}

		uint8_t get_r8(const Registers& registers)const {
			return registers.get_register8(modrmData.reg_index);
		}

		//modrmDataから使用するレジスタを識別してそのレジスタに値をセットする
		void set_r32(Registers &registers,uint32_t value) {
			registers.set_register32(modrmData.reg_index, value);
		}
		void set_r8(Registers &registers, uint8_t value) {
			registers.set_register8(modrmData.reg_index, value);
		}

		void set_rm32(Registers &registers,Memory &memory,uint32_t value) {
			if (modrmData.mod == 3)
				registers.set_register32(modrmData.rm, value);
			else {
				uint32_t address = calc_memory_address(registers);
				memory.set_memory32(address, value);
			}
		}

		void set_rm8(Registers &registers, Memory &memory, uint8_t value) {
			if (modrmData.mod == 3)
				registers.set_register8(modrmData.rm, value);
			else {
				uint32_t address = calc_memory_address(registers);
				memory.set_memory8(address, value);
			}
		}

		uint32_t get_rm32(const Registers &registers,const Memory &memory)const
		{
			if (modrmData.mod == 3)
				return registers.get_register32(modrmData.rm);
			else {
				uint32_t address = calc_memory_address(registers);
				return memory.get_memory32(address);
			}
		}

		uint8_t get_rm8(const Registers &registers, const Memory &memory)const
		{
			if (modrmData.mod == 3)
				return registers.get_register8(modrmData.rm);
			else {
				uint32_t address = calc_memory_address(registers);
				return memory.get_memory8(address);
			}
		}

	private:

		uint32_t calc_memory_address(const Registers &registers) const{
			if (modrmData.mod == 0) {
				if (modrmData.rm == 4) {
					throw L"not implemeted ModRM mod=0 rm=4 \n";
					exit(0);
				}
				else if (modrmData.rm == 5)
					return modrmData.disp32;
				else
					return registers.get_register32(modrmData.rm);
			}
			else if (modrmData.mod == 1) {
				if (modrmData.rm == 4) {
					throw L"not implemeted ModRM mod=1 rm=4 \n";
					exit(0);
				}
				else
					return registers.get_register32(modrmData.rm) + modrmData.disp8;
			}
			else if (modrmData.mod == 2) {
				if (modrmData.rm == 4) {
					throw L"not implemeted ModRM mod=2 rm=4 \n";
					exit(0);
				}
				else
					return registers.get_register32(modrmData.rm) + modrmData.disp32;
			}
			else {
				throw L"not implemeted ModRM mod=3 \n";
				exit(0);
			}
		}
	};

	ModRM parseModRM(CodeFetch &codeFetch) {
		uint8_t code;
		ModRMData modrmData;
		memset(&modrmData, 0, sizeof(ModRMData));
		code = codeFetch.getCode8(0);
		modrmData.mod = ((code & 0xC0) >> 6);//    C0=1100 0000
		modrmData.opecode = ((code & 0x38) >> 3);//38=0011 1000
		modrmData.rm = code & 0x07;//              07=0000 0111
		codeFetch.addEip(1);

		if (modrmData.mod != 3 && modrmData.rm == 4) {
			modrmData.sib = codeFetch.getCode8(0);
			codeFetch.addEip(1);
		}

		if ((modrmData.mod == 0 && modrmData.rm == 5) || modrmData.mod == 2) {
			modrmData.disp32 = codeFetch.getSignCode32(0);
			codeFetch.addEip(4);
		}
		else if (modrmData.mod == 1) {
			modrmData.disp8 = codeFetch.getSignCode8(0);
			codeFetch.addEip(1);
		}
		return ModRM(modrmData);
	}
}