#include "chip8.h"
#include <iostream>
using namespace std;
int main(void) 
{
    Chip8 chip8;
    const int screenWidth = 640;
    const int screenHeight = 320;

    InitWindow(screenWidth, screenHeight, "Chip-8 Emulator");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
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
            Chip8.key[0x2] = 0;
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

        chip8.init();
        chip8.chip8Emulate();

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        
        for(int x=0; x < 64; x++){
            for(int y=0; y < 32; y++) {
                if(chip8.draw = 1) {
                    DrawRectangle(x * 10,
                                  y * 10,
                                  10,
                                  10,
                                  BLACK);
                }  
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
};