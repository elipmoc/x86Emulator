#pragma once
#include "Container.hpp"
#include "ModRM.hpp"
#include <string>

namespace x86 {
	namespace Instructions{

		void mov_r32_imm32(Container &container) {
			const uint8_t reg = container.GetCodeFetch().getCode8(0) - 0xB8;
			const uint32_t value = container.GetCodeFetch().getCode32(1);
			container.GetRegisters().set_register32(reg, value);
			container.GetCodeFetch().addEip(5);
		}
		void mov_rm32_imm32(Container &container) {
			container.GetCodeFetch().addEip(1);
			auto modrm = parseModRM(container.GetCodeFetch());
			uint32_t value = container.GetCodeFetch().getCode32(0);
			container.GetCodeFetch().addEip(4);
			modrm.set_rm32(container.GetRegisters(), container.GetMemory(), value);
		}

		void mov_r32_rm32(Container &container)
		{
			container.GetCodeFetch().addEip(1);
			auto modrm = parseModRM(container.GetCodeFetch());
			uint32_t rm32 = modrm.get_rm32(container.GetRegisters(), container.GetMemory());
			modrm.set_r32(container.GetRegisters(), rm32);
		}

		void mov_rm32_r32(Container &container)
		{
			container.GetCodeFetch().addEip(1);
			auto modrm = parseModRM(container.GetCodeFetch());
			uint32_t r32 = modrm.get_r32(container.GetRegisters());
			modrm.set_rm32(container.GetRegisters(), container.GetMemory(), r32);
		}

		void add_rm32_r32(Container &container)
		{
			container.GetCodeFetch().addEip(1);
			auto modrm = parseModRM(container.GetCodeFetch());
			uint32_t r32 = modrm.get_r32(container.GetRegisters());
			uint32_t rm32 = modrm.get_rm32(container.GetRegisters(), container.GetMemory());
			modrm.set_rm32(container.GetRegisters(), container.GetMemory(), rm32 + r32);
		}

		void sub_rm32_imm8(Container &container, ModRM &modrm)
		{
			uint32_t rm32 = modrm.get_rm32(container.GetRegisters(), container.GetMemory());
			uint32_t imm8 = static_cast<int32_t>(container.GetCodeFetch().getSignCode8(0));
			container.GetCodeFetch().addEip(1);
			modrm.set_rm32(container.GetRegisters(), container.GetMemory(), rm32 - imm8);
		}

		void code_83(Container &container) {
			container.GetCodeFetch().addEip(1);
			auto modrm = parseModRM(container.GetCodeFetch());
			switch (modrm.get_opecode()) {
			case 5:
				sub_rm32_imm8(container,modrm);
				break;
			default:
				throw (std::string("not implemented: 83 /") + std::to_string(modrm.get_opecode())).c_str();
				exit(1);
			}
		}

		void inc_rm32(Container &container,ModRM modrm) {
			uint32_t value = modrm.get_rm32(container.GetRegisters(), container.GetMemory());
			modrm.set_rm32(container.GetRegisters(), container.GetMemory(), value + 1);
		}

		void code_ff(Container &container) {
			container.GetCodeFetch().addEip(1);
			auto modrm = parseModRM(container.GetCodeFetch());
			switch (modrm.get_opecode())
			{
			case 0:
				inc_rm32(container,modrm);
				break;
			default:
				throw (std::string("not implemented: ff /") + std::to_string(modrm.get_opecode())).c_str();
				exit(1);
			}
		}

		void short_jump(Container &container) {
			const int8_t diff = container.GetCodeFetch().getSignCode8(1);
			container.GetCodeFetch().addEip(diff + 2);
		}

		void near_jump(Container &container) {
			const int32_t diff = container.GetCodeFetch().getSignCode32(1);
			container.GetCodeFetch().addEip(diff + 5);
		}

		void push32(Container& container, uint32_t value) {
			const uint32_t address = container.GetRegisters().get_register32(Registers::ESP) - 4;
			container.GetRegisters().set_register32(Registers::ESP, address);
			container.GetMemory().set_memory32(address, value);
		}

		uint32_t pop32(Container& container) {
			const uint32_t address = container.GetRegisters().get_register32(Registers::ESP);
			const uint32_t ret = container.GetMemory().get_memory32(address);
			container.GetRegisters().set_register32(Registers::ESP, address + 4);
			return ret;
		}

		void push_r32(Container& container) {
			const uint8_t reg = container.GetCodeFetch().getCode8(0) - 0x50;
			push32(container, container.GetRegisters().get_register32(reg));
			container.GetCodeFetch().addEip(1);
		};

		void pop_r32(Container& container) {
			const uint8_t reg = container.GetCodeFetch().getCode8(0)-0x58;
			container.GetRegisters().set_register32(reg, pop32(container));
			container.GetCodeFetch().addEip(1);
		}
	};
}