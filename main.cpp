#include <iostream>
#include <cstdlib>
#include <SFML/Graphic.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

char memory[4096];
int pc;
char V[16]; //data registers
short I; //address register
short stack[24];
int timer;
int soundTimer;
char keyboard[16];
char fontSet[50]; //I don't know how big this is yet.
void opcode();
void draw(char x, char y, char height);
void input();
void clearScreen();

int main() {
    pc = 0x200; 
    while(true) {
        opcode();
        input();
        pc += 2;
    }
    return 0;
}

void clearScreen() {

}

void draw() {

}

void input() {

}
void opcode() {
    unsigned short opcode = memory[pc] << 8 | memory[pc+1];
    switch (opcode & 0xF000) {
        int x = (opcode & 0x0F00) >> 4;
        int y = (opcode & 0x00F0) >> 8;
        int NNN = opcode & 0x0FFF;
        int NN = opcode & 0x00FF;
        case 0x0000:
            switch(opcode & 0x00FF) {
                case 0x00E0:
                    clearScreen();   
                break;
                case 0x00EE:
                    pc= stack[--sp];
                break;
                default:
                    printf("Unknown opcode, %d at program counter number %d", opcode, pc);
            } 
        break;
        case 0x1000:
            pc = opcode & 0x0FFF;
        break;
        case 0x2000:
            stack[sp];
            ++sp;
            pc = opcode & 0x0FFF;
        break;
        case 0x3000:
            if (V[x] == NN) pc += 4;
            else pc += 2;
        break;
        case 0x4000:
            if (V[x] != NN) pc += 4;
            else pc += 2;
        break;
        case 0x5000:
            if (V[x] == V[y]) pc += 4; 
            else pc += 2;
        break;
        case 0x6000:
            V[x] = NN;
            pc += 2;
        break;

        case 0x7000:
            V[x] += NN;
            pc += 2;
        break;

        case 0x8000: // bitwise ops
            switch(opcode & 0x000F) {
                case 0x0000:
                    V[x] = V[y];
                    pc += 2;
                break;

                case 0x0001:
                    V[x] = V[x] | V[y];
                    pc += 2;
                break;

                case 0x0002:
                    V[x] = V[x] & V[y];
                    pc += 2;
                break;
                
                case 0x0003:
                    V[x] = V[x] ^ V[y];
                    pc += 2;
                break;   
                
                case 0x0004:
                    if (V[y] > (0xFF - V[x])) V[0xF] = 1;
                    else V[0xF] = 0;
                    V[x] += V[y];
                    pc += 2;
                break;
                
                case 0x0005:
                    if (V[y] > V[x]) V[0xF] = 0;
                    else V[0xF] = 1;
                    V[x] -= V[y];
                    pc += 2;
                break;    

                case 0x0006:
                    V[0xF] = V[x] & 1;
                    V[x] >> 1;
                    pc += 2;
                break;
                case 0x0007:
                    if (V[x] > V[y]) V[0xF] = 0;
                    else V[0xF] = 1;
                    V[y] -= V[x];
                    pc += 2;
                break; 
                
                case 0x000E:
                    V[0xF] = V[x] & 0xF000;
                    V[x] << 1;
                break;
                default:
                    printf("Unknown opcode, %d at program counter number %d", opcode, pc);
            }
        break;

        case 0x9000:
            if (V[x] != V[y]) pc += 4; 
            else pc += 2;
        break; 

        case 0xA000:
            I = NNN;
            pc += 2;
        break;

        case 0xB000:
            pc = V[0] + NNN; 
        break;

        case 0xC000:
            V[x] = (rand() % 256) & NN;
            pc += 2;
        break;

        case 0xD000:
            draw(V[x], V[y], opcode & 0x000F);
            pc += 2;
        break;
        
        case 0xE000:
            switch(opcode & 0x00FF) {
                case 0x009E:
                    if (keyboard[V[x]] == 1) pc += 4;
                    else pc += 2;
                break;
                case 0x00A1:
                    if (keyboard[V[x]] == 0) pc += 4;
                    else pc += 2;

                break;

                default:
                    printf("Unknown opcode, %d at program counter number %d", opcode, pc);
            }
        break;

        case 0xF000:
            switch(opcode & 0x00FF) {
                case 0x0007:
                    V[x] = timer;
                    pc += 2;
                break;

                case 0x000A:
                   //wait for key 
                break;

                case 0x0015:
                    timer = V[x];
                    pc += 2; 
                break;

                case 0x0018:
                    soundTimer = V[x];
                    pc += 2;
                break;

                case 0x001E:
                    I += V[x];
                    pc += 2;
                break;

                case 0x0029:
                    //complete later
                    pc += 2;
                break;


            }
        break;
        default:
            printf("Unknown opcode, %d at program counter number %d", opcode, pc);

    }
}

