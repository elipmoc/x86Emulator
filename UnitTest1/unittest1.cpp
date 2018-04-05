#include "stdafx.h"
#include "CppUnitTest.h"
#include "../x86Emulator/Emulator.hpp"
#include <iostream>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			/* ÉÅÉÇÉäÇÕ1MB */
			static constexpr size_t MEMORY_SIZE = (1024 * 1024);
				//EmulatorÇê∂ê¨
				x86::Emulator emu(MEMORY_SIZE, 0x0000, 0x7c00);
				std::vector<uint8_t> code = { 0xb8,0x29,0x00,0x00,0x00,0xeb,0xf9 };
				emu.Read(code.cbegin(),code.cend());
				auto success = []() {
					Logger::WriteMessage("end of program");
				};
				auto fail = []() {
					Assert::Fail(L"Not Implemented");
				};
				emu.exeute(success,fail);
		}
	};
}