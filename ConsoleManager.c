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
HANDLE readHandle;

struct ConsoleEvent{
    char type;
    char code;
    COORD coord;
} typedef ConsoleEvent;

void hideCursor(){
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hConsole, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &curInfo);
}

void initTerminal(){
    AllocConsole();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
    readHandle = GetStdHandle(STD_INPUT_HANDLE);
    CONSOLE_CURSOR_INFO curInfo;
    hideCursor();
    SetConsoleMode(readHandle, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
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
    hideCursor();
}

ConsoleEvent getKey(){
    DWORD eventCount;
    DWORD eventsRead;
    ConsoleEvent result;
    GetNumberOfConsoleInputEvents(readHandle, &eventCount);
    if (eventCount != 0) {
        INPUT_RECORD *eventBuffer = malloc(eventCount * sizeof(INPUT_RECORD));
        ReadConsoleInput(readHandle, eventBuffer, eventCount, &eventsRead);
        for(int i = 0; i < eventsRead; i++){
            if((eventBuffer + i)->EventType == KEY_EVENT){
                result.type = 'k';
                result.code = (eventBuffer + i)->Event.KeyEvent.uChar.AsciiChar;
                break;
            } else if ((eventBuffer + i)->EventType == MOUSE_EVENT) {
                if ((eventBuffer + i)->Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
                    result.type = 'm';
                    result.coord = (eventBuffer + i)->Event.MouseEvent.dwMousePosition;
                    break;
                }
            }
        }
        free(eventBuffer);
    }
    return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    initTerminal();
    drawPixel(3, 5, GREEN);
    while(1){
        ConsoleEvent event = getKey();
        if(event.type == 'k' && event.code == 'q'){
            break;
        } else if (event.type == 'k' && event.code == 'w') {
            drawPixel(5, 12, BLUE);
        } else if (event.type == 'm'){
            drawPixel(event.coord.X, event.coord.Y, BLUE);
        }
    }
    FreeConsole();
    return 0;
}
