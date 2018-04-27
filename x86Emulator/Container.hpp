#pragma once
#include "Registers.hpp"
#include "Memory.hpp"
#include "CodeFetch.hpp"
namespace x86 {

	class Container {

		//汎用レジスタ
		Registers registers;

		//メモリ
		Memory memory;

		//メモリ読み出しに使用
		CodeFetch codeFetch;

	public:
		Container(size_t size, uint32_t eip, uint32_t esp)
			:memory(size),
			codeFetch(eip, memory)
		{}

		Registers& GetRegisters() { return registers; }
		Memory& GetMemory() { return memory; }
		CodeFetch& GetCodeFetch() { return codeFetch; }

		const Registers& GetRegisters()const { return registers; }
		const Memory& GetMemory()const { return memory; }
		const CodeFetch& GetCodeFetch()const { return codeFetch; }

	};
}