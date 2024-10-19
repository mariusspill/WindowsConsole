#include "ConsoleManager.h"

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 4
#define YELLOW 6
#define WHITE 7

ConsoleInstance console;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    test();
}

void setConsoleFontSize(int fontSize) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;

    // Get the current font information
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
    
    // Change the font size
    cfi.dwFontSize.Y = fontSize;  // Change this value to set the height
    cfi.dwFontSize.X = cfi.dwFontSize.X; // Keep the width the same
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}

void initTerminal(int length, int height){
    console.length = length;
    console.height = height;
    console.cInfo = malloc((length * height) * sizeof(cellInfo));
    console.cellChar = malloc((2 * length * height));
    int size = length * height;
    for(int i = 0; i < size; i++){
        (console.cInfo + i)->color = 0;
    }
    AllocConsole();
    console.writeHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    console.readHandle = GetStdHandle(STD_INPUT_HANDLE);
    hideCursor();
    SMALL_RECT windowSize = {0, 0, (length * 2) - 1, height - 1};
    if(!SetConsoleWindowInfo(console.writeHandle, TRUE, &windowSize)){
        MessageBox(NULL, "Warning, no Screen change", "Error", MB_OK);
    }
    COORD bufferSize = {length * 2, height};
    if(!SetConsoleScreenBufferSize(console.writeHandle, bufferSize)){
        MessageBox(NULL, "Warning, no Buffer change", "Error", MB_OK);
    }
    SetConsoleMode(console.readHandle, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console.writeHandle, &csbi);
    if (csbi.dwSize.X != bufferSize.X){
        char *error = malloc(100);
        sprintf(error, "buffer: %d, screen: %d", bufferSize.X, csbi.dwSize.X);
        MessageBox(NULL, error, "warning", MB_OK);
        free(error);
    }
    setConsoleFontSize(32);
}

void closeTerminal(){
    free(console.cInfo);
    free(console.cellChar);
}

void hideCursor(){
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(console.writeHandle, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console.writeHandle, &curInfo);
}

void writeChar(COORD pos, char c){
    SetConsoleCursorPosition(console.writeHandle, pos);
    WriteConsole(console.writeHandle, &c, 1, NULL, NULL);
    *(console.cellChar + (pos.Y * 2 * console.length + pos.X)) = c;
    hideCursor();
}

void writeString(COORD pos, char* text){
    SetConsoleCursorPosition(console.writeHandle, pos);
    WriteConsole(console.writeHandle, text, strlen(text), NULL, NULL);
    hideCursor();
    int n = sizeof(text);
    for (int i = 0; i < n; i++){
        *(console.cellChar + (pos.Y * 2 * console.length + pos.X + i)) = *(text + i);        
    }
}

void drawPixel(int col, int row, int color){
    COORD pos = {col * 2, row};
    SetConsoleTextAttribute(console.writeHandle, (color << 4) | 0); // (BG << 4)| FG
    writeChar(pos, ' ');
    writeChar((COORD){pos.X + 1, pos.Y}, ' ');
    SetConsoleTextAttribute(console.writeHandle, 7);
    hideCursor();
    (console.cInfo + (row * console.length + col))->color = color;
}

ConsoleEvent getKey(){
    DWORD eventCount;
    DWORD eventsRead;
    ConsoleEvent result;
    result.type = 0;
    result.leftPressed = 0;
    GetNumberOfConsoleInputEvents(console.readHandle, &eventCount);
    if (eventCount != 0) {
        INPUT_RECORD *eventBuffer = malloc(eventCount * sizeof(INPUT_RECORD));
        ReadConsoleInput(console.readHandle, eventBuffer, eventCount, &eventsRead);
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
    if (result.coord.X % 2 == 0){
        result.coord.X = result.coord.X / 2;
    } else {
        result.coord.X = (result.coord.X - 1) / 2;
    }
    return result;
}

int getCellColor(int col, int row){
    return (console.cInfo + (row * console.length + col))->color;
}

void test(){
    initTerminal(40, 25);
    int eventCounter = 0;
    char* x = malloc(50);
    char* y = malloc(50);
    char* a = malloc(64);

    if (x == NULL || y == NULL || a == NULL) {
        MessageBox(NULL, "Memory allocation failed!", "Error", MB_OK);
    }

    ConsoleEvent event;
    while(1){
        event = getKey();
        if(event.type == 'k' && event.code == 'q'){
            break;
        } else if (event.type == 'k' && event.code == 'w') {
            drawPixel(5, 12, BLUE);
        } else if (event.type == 'm'){
            eventCounter++;
            sprintf(a, "X: %d   , Y: %d   ", event.coord.X, event.coord.Y);
            if (getCellColor(event.coord.X, event.coord.Y) == BLACK){
                drawPixel(event.coord.X, event.coord.Y, BLUE);
            } else if (getCellColor(event.coord.X, event.coord.Y) == BLUE) {
                drawPixel(event.coord.X, event.coord.Y, BLACK);            
            }
        if (event.type != 0 && event.coord.X >= 0 && event.coord.X < console.length &&
            event.coord.Y >= 0 && event.coord.Y < console.height) {
            sprintf(y, "EventType: %c, Color: %d", event.type, getCellColor(event.coord.X, event.coord.Y));
        } else {
            sprintf(y, "EventType: N/A, Color: N/A");
        }      
        }
        sprintf(x, "EventCounter: %d", eventCounter);

        writeString((COORD){0,0}, x);
        writeString((COORD){0,1}, y);
        writeString((COORD){0,2}, a);
    }
    closeTerminal();
    free(x);
    free(y);
    free(a);
    FreeConsole();
}