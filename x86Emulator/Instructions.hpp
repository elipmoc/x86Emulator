#pragma once
#include "Container.hpp"
#include "ModRM.hpp"
#include "IO.hpp"
#include "Bios.hpp"
#include <string>
#include <iomanip>


namespace x86 {
	namespace Instructions{

		void mov_r8_imm8(Container& container) {
			uint8_t reg = container.GetCodeFetch().getCode8(0)-0xB0;
			container.GetRegisters().set_register8(reg, container.GetCodeFetch().getCode8(1));
			container.GetCodeFetch().addEip(2);
		}
		void mov_rm8_r8(Container& container) {
			container.GetCodeFetch().addEip(1);
			auto modrm = parseModRM(container.GetCodeFetch());
			uint8_t r8 = modrm.get_r8(container.GetRegisters());
			modrm.set_rm8(container.GetRegisters(),container.GetMemory(), r8);
		}
		void mov_r8_rm8(Container& container)
		{
			container.GetCodeFetch().addEip(1);
			auto modrm=parseModRM(container.GetCodeFetch());
			uint8_t rm8 = modrm.get_rm8(container.GetRegisters(),container.GetMemory());
			modrm.set_r8(container.GetRegisters(), rm8);
		}

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
		void add_rm32_imm8(Container& container, ModRM& modrm) {
			const uint32_t rm32 = modrm.get_rm32(container.GetRegisters(), container.GetMemory());
			uint32_t imm8 = static_cast<int32_t>(container.GetCodeFetch().getSignCode8(0));
			container.GetCodeFetch().addEip(1);
			modrm.set_rm32(container.GetRegisters(), container.GetMemory(), rm32 + imm8);
		}
		void cmp_al_imm8(Container& container)
		{
			uint8_t value = container.GetCodeFetch().getCode8(1);
			uint8_t al =container.GetRegisters().get_register8(Registers::AL);
			uint64_t result = static_cast<uint64_t>(al) - static_cast<uint64_t>(value);
			container.GetEflags().update_eflags_sub(al, value, result);
			container.GetCodeFetch().addEip(2);
		}
		void cmp_eax_imm32(Container& container)
		{
			uint32_t value = container.GetCodeFetch().getCode32(1);
			uint32_t eax = container.GetRegisters().get_register32(Registers::EAX);
			uint64_t result = static_cast<uint64_t>(eax) - static_cast<uint64_t>(value);
			container.GetEflags().update_eflags_sub(eax, value, result);
			container.GetCodeFetch().addEip(5);
		}
		void cmp_r32_rm32(Container& container) {
			container.GetCodeFetch().addEip(1);
			auto modrm = parseModRM(container.GetCodeFetch());
			uint32_t r32 = modrm.get_r32(container.GetRegisters());
			uint32_t rm32 = modrm.get_rm32(container.GetRegisters(), container.GetMemory());
			uint64_t result = static_cast<uint64_t>(r32) - static_cast<uint64_t>(rm32);
			container.GetEflags().update_eflags_sub(r32, rm32, result);
		}
		void cmp_rm32_imm8(Container& container, ModRM& modrm) {
			uint32_t rm32 = modrm.get_rm32(container.GetRegisters(),container.GetMemory());
			uint32_t imm8 = static_cast<int32_t>(container.GetCodeFetch().getSignCode8(0));
			container.GetCodeFetch().addEip(1);
			uint64_t result = static_cast<uint64_t>(rm32) - static_cast<uint64_t>(imm8);
			container.GetEflags().update_eflags_sub(rm32, imm8, result);
		}
		void sub_rm32_imm8(Container &container, ModRM &modrm)
		{
			uint32_t rm32 = modrm.get_rm32(container.GetRegisters(), container.GetMemory());
			uint32_t imm8 = static_cast<int32_t>(container.GetCodeFetch().getSignCode8(0));
			container.GetCodeFetch().addEip(1);
			uint64_t result = static_cast<uint64_t>(rm32) - static_cast<uint64_t>(imm8);
			modrm.set_rm32(container.GetRegisters(), container.GetMemory(), result);
			container.GetEflags().update_eflags_sub(rm32, imm8, result);
		}

		void code_83(Container &container) {
			container.GetCodeFetch().addEip(1);
			auto modrm = parseModRM(container.GetCodeFetch());
			switch (modrm.get_opecode()) {
			case 0:
				add_rm32_imm8(container, modrm);
				break;
			case 5:
				sub_rm32_imm8(container,modrm);
				break;
			case 7:
				cmp_rm32_imm8(container, modrm);
				break;
			default:
				throw (std::string("not implemented: 83 /") + std::to_string(modrm.get_opecode())).c_str();
				exit(1);
			}
		}
		void inc_r32(Container& container)
		{
			uint8_t reg = container.GetCodeFetch().getCode8(0) - 0x40;
			container.GetRegisters().set_register32(reg,container.GetRegisters().get_register32(reg) + 1);
			container.GetCodeFetch().addEip(1);
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
		void push_imm32(Container& container) {
			uint32_t value = container.GetCodeFetch().getCode32(1);
			push32(container, value);
			container.GetCodeFetch().addEip(5);
		}
		void push_imm8(Container& container) {
			uint8_t value = container.GetCodeFetch().getCode8(1);
			push32(container, value);
			container.GetCodeFetch().addEip(2);
		}
		void pop_r32(Container& container) {
			const uint8_t reg = container.GetCodeFetch().getCode8(0)-0x58;
			container.GetRegisters().set_register32(reg, pop32(container));
			container.GetCodeFetch().addEip(1);
		}

		void call_rel32(Container& container) {
			int32_t diff = container.GetCodeFetch().getSignCode32(1);
			push32(container,container.GetCodeFetch().getEip() + 5);
			container.GetCodeFetch().addEip(diff+5);
		}

		void ret(Container& container) {
			container.GetCodeFetch().setEip(pop32(container));
		}

		void leave(Container& container) {
			uint32_t ebp = container.GetRegisters().get_register32(Registers::EBP);
			container.GetRegisters().set_register32(Registers::ESP, ebp);
			container.GetRegisters().set_register32(Registers::EBP, pop32(container));
			container.GetCodeFetch().addEip(1);
		}

		template<int F>
		void jx(Container& container) {
			int diff = container.GetEflags().get<F>() ? container.GetCodeFetch().getSignCode8(1) : 0;
			container.GetCodeFetch().addEip(diff + 2);
		}

		template<int F>
		void jnx(Container& container) {
			int diff = (!container.GetEflags().get<F>()) ? container.GetCodeFetch().getSignCode8(1) : 0;
			container.GetCodeFetch().addEip(diff + 2);
		}

		void jl(Container& container) {
			auto& eflags=container.GetEflags();
			int diff = (eflags.get<Eflags::sign_flag>() != eflags.get<Eflags::overflow_flag>()) ? container.GetCodeFetch().getSignCode8(1) : 0;
			container.GetCodeFetch().addEip(diff + 2);
		}

		void jle(Container& container) {
			auto& eflags = container.GetEflags();
			int diff = (eflags.get<Eflags::zero_flag>() || (eflags.get<Eflags::sign_flag>() != eflags.get<Eflags::overflow_flag>()))
				? container.GetCodeFetch().getSignCode8(1) : 0;
			container.GetCodeFetch().addEip(diff + 2);
		}
		
		void in_al_dx(Container& container) {
			uint16_t address = container.GetRegisters().get_register32(Registers::EDX) & 0xffff;
			uint8_t value = io::io_in8(address);
			container.GetRegisters().set_register8(Registers::AL, value);
			container.GetCodeFetch().addEip(1);
		}

		void out_dx_al(Container& container) {
			uint16_t address = container.GetRegisters().get_register32(Registers::EDX) & 0xffff;
			uint8_t value = container.GetRegisters().get_register8(Registers::AL);
			io::io_out8(address, value);
			container.GetCodeFetch().addEip(1);
		}

		void swi(Container& container) {
			uint8_t int_index = container.GetCodeFetch().getCode8(1);
			container.GetCodeFetch().addEip(2);
			switch (int_index) {
			case 0x10:
				Bios::bios_video(container.GetRegisters());
				break;
			default:
				std::wcerr << "unknown interrupt 0x" << std::setfill(L'0') << std::setw(2) << std::hex << int_index << std::endl;
			}
		}

	};
}