#pragma once

#include "stdio.h"
#include "conio.h"
#include "Windows.h"


enum ConsoleColor{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    RED = 4,
    YELLOW = 6,
    WHITE = 7
};

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

void writeStringColor(COORD pos, char* text,enum ConsoleColor fgColor);

void drawPixel(int col, int row, int color);

int getCellColor(int row, int col);

ConsoleEvent getKey();