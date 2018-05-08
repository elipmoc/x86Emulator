#pragma once
#include <cinttypes>
namespace x86 {
	class Eflags {

		//EFLAGSÉåÉWÉXÉ^
		uint32_t eflags;

	public:

		static constexpr int carry_flag = 1;
		static constexpr int zero_flag = 1 << 6;
		static constexpr int sign_flag = 1 << 7;
		static constexpr int overflow_flag = 1 << 11;

	private:
		template<int F>
		void set(bool flag) {
			eflags = flag?eflags | F : eflags & ~F;
		}

	public:

		template<int F>
		bool get() const{
			return eflags & F;
		}

		void update_eflags_sub(uint32_t v1, uint32_t v2, uint64_t result) {
			int sign1 = v1 >> 31;
			int sign2 = v2 >> 31;
			int signr = (result >> 31) & 1;
			set<carry_flag>(result >> 32);
			set<zero_flag>(result==0);
			set<sign_flag>(signr);
			set<overflow_flag>(sign1 != sign2 && sign1 != signr);
		}
	};
}