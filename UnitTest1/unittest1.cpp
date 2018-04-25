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
		
		TEST_METHOD(TestMethod1)
		{
			/* ÉÅÉÇÉäÇÕ1MB */
			static constexpr size_t MEMORY_SIZE = (1024 * 1024);
				//EmulatorÇê∂ê¨
				x86::Emulator emu(MEMORY_SIZE, 0x0000, 0x7c00);
				auto ifs=file_load_helper("../testAsm/helloworld.bin");
				emu.Read(ifs);
				auto success = []() {
					Logger::WriteMessage("end of program");
				};
				auto fail = []() {
					Assert::Fail(L"Not Implemented");
				};
				auto trace = [](std::wstring str) {
					Logger::WriteMessage(str.c_str());
				};
				emu.Exeute(success,fail,trace);
				Logger::WriteMessage(emu.Dump_registers().c_str());
		}

		TEST_METHOD(TestMethod2)
		{
			/* ÉÅÉÇÉäÇÕ1MB */
			static constexpr size_t MEMORY_SIZE = (1024 * 1024);
			//EmulatorÇê∂ê¨
			x86::Emulator emu(MEMORY_SIZE, 0x7c00, 0x7c00);
			auto ifs = file_load_helper("../testAsm/helloworld2.bin");
			emu.Read(ifs);
			auto success = []() {
				Logger::WriteMessage("end of program");
			};
			auto fail = []() {
				Assert::Fail(L"Not Implemented");
			};
			auto trace = [](std::wstring str) {
				Logger::WriteMessage(str.c_str());
			};
			emu.Exeute(success, fail, trace);
			Logger::WriteMessage(emu.Dump_registers().c_str());
		}

		TEST_METHOD(TestMethod3)
		{
			/* ÉÅÉÇÉäÇÕ1MB */
			static constexpr size_t MEMORY_SIZE = (1024 * 1024);
			//EmulatorÇê∂ê¨
			x86::Emulator emu(MEMORY_SIZE, 0x7c00, 0x7c00);
			auto ifs = file_load_helper("../testAsm/modrm-test.bin");
			emu.Read(ifs);
			auto success = []() {
				Logger::WriteMessage("end of program");
			};
			auto fail = []() {
				Assert::Fail(L"Not Implemented");
			};
			auto trace = [](std::wstring str) {
				Logger::WriteMessage(str.c_str());
			};
			emu.Exeute(success, fail, trace);
			Logger::WriteMessage(emu.Dump_registers().c_str());
		}
	};
}