#pragma once

#include "Container.hpp"
#include "CodeExecute.hpp"
#include <sstream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace x86 {

	//カスタムマニュピレータ
	std::wostream& hex08_manip(std::wostream& ost) {
		return ost << std::setfill(L'0') << std::setw(5) << std::hex;
	}
	

	class Emulator {
	private:

		const uint32_t start_eip;


		//仮想環境
		Container container;


	public:

		Emulator(size_t size, uint32_t eip, uint32_t esp):
			start_eip(eip),
			container(size,eip,esp)
		{
			container.GetRegisters().set_register32(Registers::ESP, esp);
		}

		template<class CharT,class Traits = std::char_traits<CharT>>
		void Read(std::basic_istream<CharT,Traits>& ifs) {
			ifs.read(reinterpret_cast<char*>(&container.GetMemory()[start_eip]), 512);
		}

		template<class ByteIterator>
		void Read(ByteIterator begin, ByteIterator end) {
			size_t i = start_eip;
			for (auto itr = begin; itr != end && i<512+start_eip; ++itr, i++) {
				container.GetMemory()[i] = *itr;
			}
		}

		template<class Success, class Fail,class Trace>
		void Execute(Success success,Fail fail,Trace trace) {
			CodeExecute codeExecute;
			codeExecute.Execute(container, success, fail, trace);
		}

		/* 汎用レジスタとプログラムカウンタの値を標準出力に出力する */
		std::wstring Dump_registers()const
		{
			std::wstringstream ss;
			for (size_t i = 0; i < Registers::REGISTERS_COUNT; i++) {
				ss<<std::wstring(Registers::name[i])<<" = "<<hex08_manip<<container.GetRegisters().get_register32(i)<<std::endl;
			}

			ss<<"EIP = "<< hex08_manip<<container.GetCodeFetch().getEip()<<std::endl;
			return ss.str();
		}
	};
}