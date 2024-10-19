#include "stdio.h"
#include "conio.h"
#include "Windows.h"

struct cellInfo{
    int color;
    char content;
} typedef cellInfo;


struct ConsoleInstance{
    HANDLE writeHandle;
    HANDLE readHandle;
    int length;
    int height;
    // int *cellColor;
    char *cellChar;
    cellInfo *cInfo;
} typedef ConsoleInstance;


struct ConsoleEvent{
    char type;
    char code;
    COORD coord;
    char leftPressed;
} typedef ConsoleEvent;

void test();

void initTerminal(int length, int height);

void closeTerminal();

void setConsoleFontSize(int fontSize);

void hideCursor();

void writeChar(COORD pos, char c);

void writeString(COORD pos, char* text);

void drawPixel(int row, int col, int color);

int getCellColor(int row, int col);

ConsoleEvent getKey();