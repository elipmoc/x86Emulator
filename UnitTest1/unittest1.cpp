#include "stdafx.h"
#include "CppUnitTest.h"
#include "../x86Emulator/Emulator.hpp"
#include <iostream>
#include <vector>
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{

		std::ifstream file_load_helper(std::string str) {
			std::ifstream ifs(str, std::ios_base::in | std::ios_base::binary);
			return ifs;
		}

	public:

		void testCreate(const std::string str, uint32_t eip) {
			/* ÉÅÉÇÉäÇÕ1MB */
			static constexpr size_t MEMORY_SIZE = (1024 * 1024);
			//EmulatorÇê∂ê¨
			x86::Emulator emu(MEMORY_SIZE, eip, 0x7c00);
			auto ifs = file_load_helper(str);
			emu.Read(ifs);
			auto success = []() {
				Logger::WriteMessage("end of program\n");
			};
			auto fail = []() {
				Assert::Fail(L"Not Implemented");
			};
			auto trace = [](std::wstring str) {
				Logger::WriteMessage(str.c_str());
			};
			emu.Execute(success, fail, trace);
			Logger::WriteMessage(emu.Dump_registers().c_str());
		}
		
		TEST_METHOD(TestMethod1)
		{
			testCreate("../testAsm/helloworld.bin", 0x0000);
		}

		TEST_METHOD(TestMethod2)
		{
			testCreate("../testAsm/helloworld2.bin", 0x7c00);
		}

		TEST_METHOD(TestMethod3)
		{
			testCreate("../testAsm/modrm-test.bin", 0x7c00);
		}

		TEST_METHOD(TestMethod4)
		{
			testCreate("../testAsm/call-test.bin", 0x7c00);
		}
		TEST_METHOD(TestMethod5)
		{
			testCreate("../testAsm/leave-test.bin", 0x7c00);
		}
		TEST_METHOD(TestMethod6)
		{
			testCreate("../testAsm/arg-test.bin", 0x7c00);
		}
		TEST_METHOD(TestMethod7)
		{
			testCreate("../testAsm/if-test.bin", 0x7c00);
		}
		TEST_METHOD(TestMethod8)
		{
			testCreate("../testAsm/while-test.bin", 0x7c00);
		}

		TEST_METHOD(TestMethod9)
		{
			testCreate("../testAsm/out.bin", 0x7c00);
		}
	};
}