#pragma once
#include "IO.hpp"
#include "Emulator.hpp"
#include <iostream>
#include <iomanip>

namespace x86 {
	static class Bios {
	public:
		//BIOSの色コードを端末の色コードに変換するテーブル
		static constexpr int bios_to_terminal[8] = { 30,34,32,36,31,35,33,37 };

		static void put_string(const char* s,size_t n) {
			for (size_t i = 0;i<n;i++)
				io::io_out8(0x03f8, s[i]);
		}

		static void bios_video_teletype(const Registers& registers) {
			uint8_t color = registers.get_register8(Registers::BL) & 0x0f;
			uint8_t ch = registers.get_register8(Registers::AL);
			char buf[32];
			int terminal_color = bios_to_terminal[color & 0x07];
			int bright = (color & 0x08) ? 1 : 0;
			int len = sprintf_s( buf,32, "\x1b[%d;%dm%c\x1b", bright, terminal_color, ch);
			put_string(buf, len);
		}

		static void bios_video(const Registers& registers) {
			uint8_t func = registers.get_register8(Registers::AH);
			switch (func) {
			case 0x0e:
				bios_video_teletype(registers);
				break;
			default:
				std::wcerr << "not implemented BIOS video function: 0x" << std::setfill(L'0') << std::setw(2) << std::hex << func << std::endl;
			}
		}
	};
}