#include <iostream>
#include "Chip.h"


using namespace std;

int main(int argc, char **argv)
{
	Chip *chip8 = new Chip();
	chip8->load_rom("PONG");
	chip8->initialize();
	chip8->start();
	delete chip8;

	return 0;
}

