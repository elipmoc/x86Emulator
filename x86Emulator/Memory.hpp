#pragma once

#include <cstdint>
#include <memory>

namespace x86 {
	class Memory {

		//メモリ(バイト列)
		std::unique_ptr<uint8_t[]> memory;

	public:

		const size_t memorySize;

		Memory(size_t size) :
			memory(std::make_unique<uint8_t[]>(size)),
			memorySize(size)
		{}

		uint8_t& operator[](size_t index) {
			return memory[index];
		}

		uint8_t operator[](size_t index)const {
			return memory[index];
		}

		void set_memory8(uint32_t address, uint32_t value) {
			memory[address] = value & 0xFF;
		}

		void set_memory32(uint32_t address, uint32_t value) {
			for (size_t i = 0; i < 4; i++)
				set_memory8(address + i, value >> (i * 8));
		}

		uint32_t get_memory8(uint32_t address)const
		{
			return memory[address];
		}

		uint32_t get_memory32(uint32_t address)const
		{
			int i;
			uint32_t ret = 0;

			/* リトルエンディアンでメモリの値を取得する */
			for (i = 0; i < 4; i++) {
				ret |= get_memory8(address + i) << (8 * i);
			}

			return ret;
		}


	};
}