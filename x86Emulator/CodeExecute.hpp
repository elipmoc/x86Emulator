#pragma once
#include "Container.hpp"
#include "Instructions.hpp"
#include <functional>

namespace x86 {

	class CodeExecute {

		std::function<void(Container&)> instructions[256];

	public:
		CodeExecute() {
			for (int i = 0; i < 8; i++) {
				instructions[0xB8 + i] = Instructions::mov_r32_imm32;
			}
			instructions[0x01] = Instructions::add_rm32_r32;
			instructions[0x3b] = Instructions::cmp_r32_rm32;
			instructions[0x3C] = Instructions::cmp_al_imm8;
			instructions[0x3D] = Instructions::cmp_eax_imm32;
			instructions[0x83] = Instructions::code_83;
			instructions[0x89] = Instructions::mov_rm32_r32;
			instructions[0x88] = Instructions::mov_rm8_r8;
			instructions[0x8A] = Instructions::mov_r8_rm8;
			instructions[0x8B] = Instructions::mov_r32_rm32;
			instructions[0xC7] = Instructions::mov_rm32_imm32;
			instructions[0xFF] = Instructions::code_ff;
			instructions[0xEB] = Instructions::short_jump;
			instructions[0xE9] = Instructions::near_jump;
			instructions[0xC3] = Instructions::ret;
			instructions[0xC9] = Instructions::leave;
			instructions[0xE8] = Instructions::call_rel32;
			instructions[0x68] = Instructions::push_imm32;
			instructions[0x6A] = Instructions::push_imm8;

			instructions[0x70] = Instructions::jx<Eflags::overflow_flag>;
			instructions[0x71] = Instructions::jnx<Eflags::overflow_flag>;
			instructions[0x72] = Instructions::jx<Eflags::carry_flag>;
			instructions[0x73] = Instructions::jnx<Eflags::carry_flag>;
			instructions[0x74] = Instructions::jx<Eflags::zero_flag>;
			instructions[0x75] = Instructions::jnx<Eflags::zero_flag>;
			instructions[0x78] = Instructions::jx<Eflags::sign_flag>;
			instructions[0x79] = Instructions::jnx<Eflags::sign_flag>;
			instructions[0x7C] = Instructions::jl;
			instructions[0x7E] = Instructions::jle;
			instructions[0xEC] = Instructions::in_al_dx;
			instructions[0xEE] = Instructions::out_dx_al;

			for (size_t i = 0; i < 8; i++) {
				instructions[0xB0 + i] = Instructions::mov_r8_imm8;
			}

			for (size_t i = 0; i < 8; i++) {
				instructions[0x40 + i] = Instructions::inc_r32;
			}

			for (size_t i = 0; i < 8; i++) {
				instructions[0x50 + i] = Instructions::push_r32;
			}

			for (size_t i = 0; i < 8; i++) {
				instructions[0x58 + i] = Instructions::pop_r32;
			}

		}


		template<class Success, class Fail, class Trace>
		void Execute(Container& container,Success success, Fail fail, Trace trace) {
			while (container.GetCodeFetch().getEip() < container.GetMemory().memorySize) {
				uint8_t code = container.GetCodeFetch().getCode8(0);
				std::wstringstream ss;
				ss << "EIP = " << std::hex << container.GetCodeFetch().getEip() << ", Code = " << std::hex << code << std::endl;
				/* 現在のプログラムカウンタと実行されるバイナリを出力する */
				trace(ss.str());
				if (instructions[code] == false) {
					fail();
					break;
				}
				/*命令の実行*/
				instructions[code](container);
				if (container.GetCodeFetch().getEip() == 0x00) {
					success();
					break;
				}
			}
		}
	};

}