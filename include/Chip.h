#ifndef CHIP_H
#define CHIP_H

#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

class Chip
{
	const uint16_t MEMSIZE = 4096;

	public:
		Chip();
		~Chip();
		void load_rom(std::string);
		void initialize();
		void start();
		void screen_conversion(Uint32*);

	private:
		uint8_t memory[4096];		//Banco de memoria para la CPU
		uint16_t pc;				//Contador del programa
		uint16_t stack[16];			//Pila. 16 registros de 16 bits
		uint16_t sp; 				//Puntero de la pila
		uint8_t v[16]; 				//16 registros de uso general
		uint16_t i; 				//Registro especial de dirección I
		uint8_t delay_timer;		//Temporizador
		uint8_t sound_timer;		//Temporizador de sonido
		char screen[2048];			//Pantalla

};

#endif // CHIP_H
