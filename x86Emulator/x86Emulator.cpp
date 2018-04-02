// x86Emulator.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Emulator.hpp"

/* メモリは1MB */
static constexpr size_t MEMORY_SIZE = (1024 * 1024);

int main(int argc,char* argv[])
{
	x86::Emulator emu(MEMORY_SIZE, 0x0000, 0x7c00);
    return 0;
}

