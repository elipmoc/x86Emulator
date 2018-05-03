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
			instructions[0x83] = Instructions::code_83;
			instructions[0x89] = Instructions::mov_rm32_r32;
			instructions[0x8B] = Instructions::mov_r32_rm32;
			instructions[0xC7] = Instructions::mov_rm32_imm32;
			instructions[0xFF] = Instructions::code_ff;
			instructions[0xEB] = Instructions::short_jump;
			instructions[0xE9] = Instructions::near_jump;
			instructions[0xC3] = Instructions::ret;
			instructions[0xE8] = Instructions::call_rel32;
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