#pragma once

#include <cstdint>
#include <memory>

namespace x86 {
	class Memory {

		//ÉÅÉÇÉä(ÉoÉCÉgóÒ)
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
	};
}