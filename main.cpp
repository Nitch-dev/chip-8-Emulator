#include <iostream>
#include <stdlib.h>
#include "raylib.h"
#define MEM 4096
using namespace std;
class Chip8
{

    public:

    Chip8();
    void init();
    void chip8Emulate();

    unsigned short opcode;
    unsigned char memory[MEM];

    unsigned char V[16];
    unsigned short I;
    unsigned short pc; //always incr by 2 cuz op is 2 bytes

    unsigned char delay_timer;
    unsigned char sound_timer; // beeps when reaches 0

    unsigned short stack[16];
    unsigned short sp;
    unsigned char screen[64 * 32];
    unsigned char key[16]; //0 for no key press 1 for key press

    bool keyPress = false;

    int draw = 0; // dont draw by default

    unsigned char chip8_fontset[80] =
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


};

Chip8::Chip8(){

}


void Chip8::init() {

    FILE *game;
    pc = 0x200;
    opcode = 0;
    I      = 0;
    sp     = 0;



    delay_timer = 0;
    sound_timer = 0;

	// clearing stack & registers
	for (int i = 0; i < 16; ++i){
		stack[i] = 0;
        V[i] = 0;
    };

        // clearing memory
	for (int i = 0; i < MEM; ++i){
		memory[i] = 0;
    }


    // loading fontset in the first 80 bytes of memory
    for(int i = 0; i < 80; ++i){
        memory[i] = chip8_fontset[i];
    }

    //loading game into memory
    game = fopen("PON","rb");
    fread(memory+0x200, 1, MEM-0x200, game);
    fclose(game);
    cout << "game lodded";
};


void Chip8::chip8Emulate()
{

    // fetching the opcode
    opcode = memory[pc] << 8 | memory[pc + 1];  // fetching 2 bytes from memory and adding to make a opcode

    switch (opcode & 0xF000) // this will give us first 4 bytes of opcode
    {
        case 0xA000:  // means set I to other 12 bytes of opcode
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0x0000:    //bcz there are 2 instruction of same 4 bytes we check last 4 bytes
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    for (int i = 0; i < 2048; i++)
                    {
                        screen[i] = 0;
                        pc += 2;
                        draw = 1;
                    };
                    break;
                case 0x000E:
                    --sp;
                    pc = sp & 0xF + 2;
                    break;
                default:
                    printf("Wrong opcode");
            }

        case 0x2000:
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF; // reads the last 3 bytes into pc
            break;
        case 0x0004:
            if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) {
                V[0xF] = 1; //set carry
            }
            else {
                V[0xF] = 0;
                V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                pc += 2;
            }
        case 0x0033:
            memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
            memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
            memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
            pc += 2;
            break;
        case 0xD000: // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short h = opcode & 0x000F;
            unsigned short pixel;
            V[0xF] = 0; //no collision
            for(int yline=0; yline < h; yline++)
            {
                pixel = memory[I + yline]; // pixel value form memory
                for(int xline = 0; xline < 8; xline++)
                {

                    if((pixel & (0x80 >> xline)) != 0) //check if pixel evaluated is 1 or not
                    {
                        if(screen[(x + xline + ((y + yline) * 64))] == 1) //if mem pixel and the current one r same or not if not then collision
                        V[0xF] = 1; //collision occurs
                        screen[x + xline + ((y + yline) * 64)] ^= 1;
                    }

                }

            }
            draw = 1;
            pc += 2;
            break;
        case 0xE000:
            switch(opcode & 0x00FF)
            {
                case 0x009E:
                    if(key[V[(opcode & 0x0F00) >> 8]] != 0)
                    {
                        pc += 4;
                    }
                    else
                    {
                        pc+=2;
                    }
                case 0x00A1:
                    if(key[V[(opcode & 0x0F00) >> 8]] ==0){
                        pc+=4;
                    }
                    else {
                        pc+=2;
                    }
                    break;
                default:
                    printf("wrong opcode");

            }
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
        case 0x3000: //jmp if X == NN
            if(V[(opcode & 0x0FFF) >> 8] == V[(opcode & 0x00FF)]) {
                pc+= 4;
            }
            else {
                pc +=2;
            }
        case 0x4000:
            if(V[(opcode & 0x0FFF) >> 8] != V[(opcode & 0x00FF)]) {
                pc+=4;
            }
            else {
                pc+=2;
            }
        case 0x5000: //skip if VX == VY
             if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
                pc+=4;
             }
             else {
                pc+=2;
             }
        case 0x6000:
            V[(opcode & 0x0FFF) >> 8] = opcode & 0x00FF;
            pc+=2;
            break;
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc +=2;
            break;
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000: // 8XY0: Sets VX to the value of VY
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0001: // 8XY1: Sets VX to VX or VY
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0002: // 8XY2: Sets VX to VX and VY
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0003: // 8XY3: Sets VX to VX xor VY
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0004: // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
                    if(((int)V[(opcode & 0x0F00) >> 8 ] + (int)V[(opcode & 0x00F0) >> 4]) < 256)
                        V[0xF] &= 0;
                    else
                        V[0xF] = 1;

                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0005: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    if(((int)V[(opcode & 0x0F00) >> 8 ] - (int)V[(opcode & 0x00F0) >> 4]) >= 0)
                        V[0xF] = 1;
                    else
                        V[0xF] &= 0;

                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                case 0x0006: // 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 7;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
                    pc += 2;
                    break;

                case 0x0007: // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    if(((int)V[(opcode & 0x0F00) >> 8] - (int)V[(opcode & 0x00F0) >> 4]) > 0)
                        V[0xF] = 1;
                    else
                        V[0xF] &= 0;

                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x000E: // 8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
                    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
                    pc += 2;
                    break;
                default: printf("Wrong opcode: %X\n", opcode);
            };
        case 0x9000:
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
                pc+=4;
            }
            else {
                pc+=2;
            }
        case 0xB000:
            pc = (opcode & 0x0FFF) + V[0];
            break;
        case 0xC000:
            V[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
            pc+=2;
            break;
        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007:
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc+=2;
                    break;
                case 0x000A: // FX0A: A key press is awaited, and then stored in VX


					for(int i = 0; i < 16; ++i)
					{
						if(key[i] != 0)
						{
							V[(opcode & 0x0F00) >> 8] = i;
							keyPress = true;
						}
					}

					// If we didn't received a keypress, skip this cycle and try again.
					if(!keyPress){
                        return;
                    }
					pc += 2;
                break;

				case 0x0015: // FX15: Sets the delay timer to VX
					delay_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
				    break;

				case 0x0018: // FX18: Sets the sound timer to VX
					sound_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
				    break;

				case 0x001E: // FX1E: Adds VX to I
					if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF)	// VF is set to 1 when range overflow (I+VX>0xFFF), and 0 when there isn't.
                    {
                        V[0xF] = 1;
                    }

					else
                    {
                        V[0xF] = 0;
                    }

					I += V[(opcode & 0x0F00) >> 8];
					pc += 2;
				    break;

				case 0x0029: // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
					I = V[(opcode & 0x0F00) >> 8] * 0x5;
					pc += 2;
				    break;
                case 0x0055: // FX55: Stores V0 to VX in memory starting at address I
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						memory[I + i] = V[i];

					// On the original interpreter, when the operation is done, I = I + X + 1.
					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
				    break;

				case 0x0065: // FX65: Fills V0 to VX with values from memory starting at address I
					for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
						V[i] = memory[I + i];

					// On the original interpreter, when the operation is done, I = I + X + 1.
					I += ((opcode & 0x0F00) >> 8) + 1;
					pc += 2;
				    break;
                default:
                    printf("wrong opcode");
            }
        default:
            printf("wrong opcode");


    };
    // timer stuff
    if(delay_timer > 0)
    {
        --delay_timer;
    }
    if(sound_timer == 1)
    {
        printf("BEEEEEEP!!");
        --sound_timer;
    }
};

// main here -->
int main(void)
{
    Chip8 chip8;
    const int screenWidth = 640;
    const int screenHeight = 320;

    InitWindow(screenWidth, screenHeight, "Chip-8 Emulator");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        chip8.init();
        chip8.chip8Emulate();


        if (IsKeyDown(KEY_ONE))
        {
            cout << "key 1" ;
            chip8.key[0x0] = 1;
        }
        else if (IsKeyUp(KEY_ONE))
        {
            chip8.key[0x0] = 0;
        }
        if (IsKeyDown(KEY_TWO))
        {
            cout << "key 2" ;
            chip8.key[0x1] = 1;
        }
        else if (IsKeyUp(KEY_TWO))
        {
            chip8.key[0x1] = 0;
        }
        if (IsKeyDown(KEY_THREE))
        {
            cout << "key 3" << endl;
            chip8.key[0x2] = 1;
        }
        else if (IsKeyUp(KEY_THREE))
        {
            chip8.key[0x2] = 0;
        }
        if (IsKeyDown(KEY_FOUR))
        {
            cout << "key 4" ;
            chip8.key[0x3] = 1;
        }
        else if (IsKeyUp(KEY_FOUR))
        {
            chip8.key[0x3] = 0;
        }
        if (IsKeyDown(KEY_Q))
        {
            cout << "key q" ;
            chip8.key[0x4] = 1;
        }
        else if (IsKeyUp(KEY_Q))
        {
            chip8.key[0x4] = 0;
        }
        if (IsKeyDown(KEY_W))
        {
            cout << "key w" ;
            chip8.key[0x5] = 1;
        }
        else if (IsKeyUp(KEY_W))
        {
            chip8.key[0x5] = 0;
        }
        if (IsKeyDown(KEY_E))
        {
            cout << "key e" ;
            chip8.key[0x6] = 1;
        }
        else if (IsKeyUp(KEY_E))
        {
            chip8.key[0x6] = 0;
        }
        if (IsKeyDown(KEY_R))
        {
            cout << "key r" ;
            chip8.key[0x7] = 1;
        }
        else if (IsKeyUp(KEY_R))
        {
            chip8.key[0x7] = 0;
        }
        if (IsKeyDown(KEY_A))
        {
            cout << "key a" ;
            chip8.key[0x8] = 1;
        }
        else if (IsKeyUp(KEY_A))
        {
            chip8.key[0x8] = 0;
        }
        if (IsKeyDown(KEY_S))
        {
            cout << "key s" ;
            chip8.key[0x9] = 1;
        }
        else if (IsKeyUp(KEY_S))
        {
            chip8.key[0x9] = 0;
        }
        if (IsKeyDown(KEY_D))
        {
            cout << "key d" ;
            chip8.key[0xA] = 1;
        }
        else if (IsKeyUp(KEY_D))
        {
            chip8.key[0xA] = 0;
        }
        if (IsKeyDown(KEY_F))
        {
            cout << "key f" ;
            chip8.key[0xB] = 1;
        }
        else if (IsKeyUp(KEY_F))
        {
            chip8.key[0xB] = 0;
        }
        if (IsKeyDown(KEY_Z))
        {
            cout << "key z" ;
            chip8.key[0xC] = 1;
        }
        else if (IsKeyUp(KEY_Z))
        {
            chip8.key[0xC] = 0;
        }
        if (IsKeyDown(KEY_X))
        {
            cout << "key x" ;
            chip8.key[0xD] = 1;
        }
        else if (IsKeyUp(KEY_X))
        {
            chip8.key[0xD] = 0;
        }
        if (IsKeyDown(KEY_C))
        {
            cout << "key c" ;
            chip8.key[0xE] = 1;
        }
        else if (IsKeyUp(KEY_C))
        {
            chip8.key[0xE] = 0;
        }
        if (IsKeyDown(KEY_V))
        {
            cout << "key v" ;
            chip8.key[0xF] = 1;
        }
        else if (IsKeyUp(KEY_V))
        {
            chip8.key[0xF] = 0;
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        for(int x=0; x < 64; x++)
        {
            for(int y=0; y < 32; y++)
            {
                DrawRectangle(x * 10,
                                y * 10,
                                10,
                                10,
                                BLACK);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
};
