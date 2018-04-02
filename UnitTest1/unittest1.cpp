#include "stdafx.h"
#include "CppUnitTest.h"
#include "../x86Emulator/Emulator.hpp"
#include <iostream>
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
				std::istringstream ist("abc");
				emu.Read(std::move(ist));
			
		}
	};
}