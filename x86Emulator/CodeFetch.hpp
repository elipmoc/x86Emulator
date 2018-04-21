#pragma once
#include <cstdint>
#include "Memory.hpp"

namespace x86 {

	//メモリからデータを読み出すクラス
	class CodeFetch {
		//プログラムカウンタ
		uint32_t eip;

		const Memory& memory;

	public:

		CodeFetch(uint32_t eip, const Memory& memory) :eip(eip), memory(memory) {}

		uint32_t getEip()const noexcept { return eip; }

		void addEip(uint32_t add)noexcept { eip += add; }


		uint32_t getCode8(const int index)const {
			return memory[eip + index];
		}

		int32_t getSignCode8(const int index)const {
			return static_cast<int8_t>(getCode8(index));
		}

		uint32_t getCode32(const int index)const {
			uint32_t ret = 0;

			/* リトルエンディアンでメモリの値を取得する */
			for (int i = 0; i < 4; i++) {
				ret |= getCode8(index + i) << (i * 8);
			}

			return ret;
		}

		int32_t getSignCode32(const int index)const {
			return static_cast<int32_t>(getCode32(index));
		}
	};

}