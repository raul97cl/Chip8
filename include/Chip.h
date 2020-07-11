#ifndef CHIP_H
#define CHIP_H

#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <cstring>

class Chip
{
	 unsigned char hexcodes[80] =
	{
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	unsigned char keys[16] =
	{
		SDL_SCANCODE_X, //0
		SDL_SCANCODE_1, //1
		SDL_SCANCODE_2, //2
		SDL_SCANCODE_3, //3
		SDL_SCANCODE_Q, //4
		SDL_SCANCODE_W, //5
		SDL_SCANCODE_E, //6
		SDL_SCANCODE_A, //7
		SDL_SCANCODE_S, //8
		SDL_SCANCODE_D, //9
		SDL_SCANCODE_Z, //A
		SDL_SCANCODE_C, //B
		SDL_SCANCODE_4, //C
		SDL_SCANCODE_R, //D
		SDL_SCANCODE_F, //E
		SDL_SCANCODE_V, //F
	};

	public:
		Chip();
		~Chip();
		void load_rom(std::string);
		void initialize();
		void step();
		int pressed_key(char);
		void screen_conversion(Uint32*);
		void timer_decrement();
		bool check_wait_key();

	private:
		uint8_t memory[4096];		//Banco de memoria para la CPU
		uint16_t pc;				//Contador del programa
		uint16_t stack[16];			//Pila. 16 registros de 16 bits
		uint16_t sp; 				//Puntero de la pila
		uint8_t v[16]; 				//16 registros de uso general
		uint16_t i; 				//Registro especial de dirección I
		uint8_t delay_timer;		//Temporizador
		uint8_t sound_timer;		//Temporizador de sonido
		char wait_key;              //Tecla que estoy espeando
		char screen[2048];			//Pantalla

};

#endif // CHIP_H
