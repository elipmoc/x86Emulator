// x86Emulator.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Emulator.hpp"
#include <iostream>
#include <fstream>

/* メモリは1MB */
static constexpr size_t MEMORY_SIZE = (1024 * 1024);

int main(int argc,char* argv[])
{
	//Emulatorを生成
	x86::Emulator emu(MEMORY_SIZE, 0x7c00, 0x7c00);

	if (argc != 2) {
		std::cout << "usage: px86 filename"<<std::endl;
		return 1;
	}
	std::ifstream ifs(argv[1],std::ios_base::in|std::ios_base::binary );

	if (!ifs) {
		std::cout << argv[1] << "ファイルが開けません" << std::endl;
		return 1;
	}
	emu.Read(ifs);

    return 0;
}

