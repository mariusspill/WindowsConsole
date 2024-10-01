#include "stdio.h"
#include "conio.h"
#include "Windows.h"

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 4
#define YELLOW 6
#define WHITE 7

HANDLE hConsole;

void initTerminal(){
    AllocConsole();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
}

void writeChar(COORD pos, char c){
    SetConsoleCursorPosition(hConsole, pos);
    WriteConsole(hConsole, &c, 1, NULL, NULL);
}

void writeString(COORD pos, char* text){
    SetConsoleCursorPosition(hConsole, pos);
    WriteConsole(hConsole, text, strlen(text), NULL, NULL);
}

void drawPixel(int row, int col, int color){
    COORD pos = {row, col};
    SetConsoleTextAttribute(hConsole, (color << 4) | 0); // (BG << 4)| FG
    writeChar(pos, ' ');
    writeChar((COORD){pos.X + 1, pos.Y}, ' ');
    SetConsoleTextAttribute(hConsole, 7);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    initTerminal();
    while(1){
        if(GetAsyncKeyState('Q') & 0x8000){
            break;
        }
    }
    FreeConsole();
    return 0;
}