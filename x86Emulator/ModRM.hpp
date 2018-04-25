#pragma once
#include <cstdint>
namespace x86 {
	struct ModRMData {
		uint8_t mod;
		union {
			uint8_t opecode;
			uint8_t reg_index;
		};
		uint8_t rm;
		uint8_t sib;
		union {
			int8_t disp8;
			uint32_t disp32;
		};
	};
}