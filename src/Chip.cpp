#include "Chip.h"

Chip::Chip()
{
    //ctor
    for(int i = 0; i < 4096; i++)
    {
        memory[i] = 0;
    }
}

Chip::~Chip()
{
    //dtor
}

void Chip::initialize()
{
    this->sp = 0;
    this->i = 0;
    this->delay_timer = 0;
    this->sound_timer = 0;
    this->pc = 0x200;
	this->wait_key = -1;
	memset(memory, 0, 4096);
    memset(this->stack, 0, 32);
    memset(v, 0, 16);
	memcpy(memory + 0x50, hexcodes, 80);
}

void Chip::step()
{
	uint16_t mem_length = (sizeof(memory) / sizeof(memory[0]));

    //Lee dos bytes de la memoria
    uint16_t opcode = (memory[pc] << 8 ) | memory[pc + 1];
    pc += 2;
    if((pc + 2) == mem_length)
    	pc = 0;
    //pc = (pc + 2) & 0xFFF;

    //M�scaras binarias para los opcodes
    uint16_t nnn = opcode & 0x0FFF;
    uint8_t kk = opcode & 0xFF;
    uint8_t n = opcode & 0xF;
    uint8_t x = (opcode >> 8) & 0xF;
    uint8_t y = (opcode >> 4) & 0xF;
    uint8_t p = (opcode >> 12);

    switch(p)
    {
		case 0:
			if(opcode == 0x00E0)
			{
				//CLS
				memset(screen, 0, 2048);
			}
			else if(opcode == 0x00EE)
			{
				//RET
				if(sp > 0)
					pc = stack[--sp];
			}
			break;
		case 1:
			//JP: Pone el contador del programa al valor de nnn
			pc = nnn;
			break;
		case 2:
			//CALL nnn
			if(sp < 16)
				stack[sp++] = pc;
			pc = nnn;
			break;
		case 3:
			//SE x, kk
			if(v[x] == kk)
			{
				if((pc + 2) == mem_length)
					pc = 0;
				else
					pc += 2;
			}
			break;
		case 4:
			//SNE x, kk
			if(v[x] != kk)
			{
				if((pc + 2) == mem_length)
					pc = 0;
				else
					pc += 2;
			}
			break;
		case 5:
			//SE x, y
			if(v[x] == v[y])
			{
				if((pc + 2) == mem_length)
					pc = 0;
				else
					pc += 2;
				pc = (pc + 2) & 0xFFF;
			}
			break;
		case 6:
			//LD x, kk
			v[x] = kk;
			break;
		case 7:
			//ADD x, kk
			//Se le aplica m�scara por si se sale de rango
			v[x] = (v[x] + kk) & 0xFF;
			break;
		case 8:
			switch(n)
			{
				case 0:
					//LD x, y
					v[x] = v[y];
					break;
				case 1:
					//OR x,y
					v[x] |= v[y];
					break;
				case 2:
					//AND x, y
					v[x] &= v[y];
					break;
				case 3:
					//XOR x. y
					v[x] ^= v[y];
					break;
				case 4:
					//ADD x, y
					v[0xF] = (v[x] > v[x] + v[y]);
					v[x] += v[y];
					break;
				case 5:
					//SUB x, y
					v[0xF] = (v[x] > v[y]);
					v[x] -= v[y];
					break;
				case 6:
					//SHR x
					v[0xF] = (v[x] & 1);
					v[x] >>= 1;
					break;
				case 7:
					//SUBN x, y
					v[0xF] = (v[y] > v[x]);
					v[x] = v[y] - v[x];
					break;
				case 0xE:
					//SHL x
					//Si el bit m�s significativo es distinto de 0 lo guarda
					v[0xF] = ((v[x] & 0x80) != 0);
					v[x] <<=  1;
					break;
			}
			break;
		case 9:
			//SNE x, y
			if(v[x] != v[y])
			{
				if((pc + 2) == mem_length)
					pc = 0;
				else
					pc += 2;
			}
			break;
		case 0xA:
			//LD I
			this->i = nnn;
			break;
		case 0xB:
			//JP
			pc = (v[0] + nnn) & 0xFFF;
			break;
		case 0xC:
			//RND x, kk
			v[x] = rand() & kk;
			break;
		case 0xD:
			//DRW x, y ,n
			//Dibuja un sprite en el pixel v[x], v[y]
			//El n�mero de filas se indica con n
			//El sprite se optiene en la direcci�n de memoria [I]
			v[15] = 0;
			for(int j = 0; j < n; j++)
			{
				uint8_t sprite = memory[this->i + j];
				for(int k = 0; k < 8; k++)
				{
					//M�scasras de 1
					int px = (v[x] + k) & 63;
					int py = (v[y] + j) & 31;
					int pos = 64 * py + px;

					//Pone en 1 teniendo en cuenta los desplazamiento debido a que es un array unidimensional
					int pixel = (sprite & (1 << (7 - k))) != 0;
					v[15] |= (screen[pos] & pixel);
					screen[pos] ^= pixel;
				}
			}
			break;
		case 0xE:
			if(kk == 0x9E)
			{
				//SKP x
				if(pressed_key(v[x]))
					pc = (pc + 2) & 0xFFF;
			}
			else if(kk == 0xA1)
			{
				//SKNP
				if(!pressed_key(v[x]))
					pc = (pc + 2) & 0xFFF;
			}
			break;
		case 0xF:
			switch(kk)
			{
				case 0x07:
					//LD x, delay_timer
					v[x] = delay_timer;
					break;
				case 0x0A:
					//LD x, k
					wait_key = x;
					break;
				case 0x15:
					//LD delay_timer, x
					delay_timer = v[x];
					break;
				case 0x18:
					//LD sound_timer, x
					sound_timer = v[x];
					break;
				case 0x1E:
					//ADD I, x
					this->i += v[x];
					break;
				case 0x29:
					//LD F, x
					i = 0x50 + (v[x] & 0xF) * 5;
					break;
				case 0x33:
					//LD I, x
					memory[this->i] = (v[x] / 100);
					memory[this->i + 1] = (v[x] / 10) % 10;
					memory[this->i + 2] = v[x] % 10;
					break;
				case 0x55:
					//LD [I], x
					for(int reg = 0; reg <= x; reg++)
					{
						memory[this->i + reg] = v[reg];
					}
					break;
				case 0x65:
					//LD x, [I]
					for(int reg = 0; reg <= x; reg++)
					{
						v[reg] = memory[this->i + reg];
					}
					break;
			}
			break;
    }
}


void Chip::load_rom(std::string rom)
{
	const char *directory_rom = (rom).c_str();
    FILE *file = fopen(directory_rom, "rb");

    if(file == nullptr)
    {
        std::cerr << "Error loading the file" << std::endl;
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    fseek(file, 0, SEEK_SET);

    if(len > 3584)
	{
		std::cout << "No memory" << std::endl;
	}

    //Guarda a partir de las posici�n 512
    fread(memory + 0x200, len, 1, file);

    fclose(file);
}
void Chip::screen_conversion(Uint32* to)
{
	for(int i = 0; i < 2048; i++)
    {
        to[i] = (screen[i]) ? -1 : 0;
    }
}

int Chip::pressed_key(char key)
{
	const Uint8 *sdl_keys = SDL_GetKeyboardState(nullptr);
	Uint8  real_key = keys[(int)key];
	return sdl_keys[real_key];
}

void Chip::timer_decrement()
{
	if(this->delay_timer)
		this->delay_timer--;
	if(this->sound_timer)
	{
		--this->sound_timer;
	}
}

bool Chip::check_wait_key()
{
	if(this->wait_key != -1)
	{
		for(int key = 0; key <= 15; key++)
		{
			if(this->pressed_key(key))
			{
				this->v[this->wait_key] = key;
						this->wait_key = -1;
			}
		}

		return true;
	}

	return false;
}



