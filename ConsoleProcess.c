#include "Windows.h"

STARTUPINFO startinfo = {0};
PROCESS_INFORMATION processinfo = {0};

void createProcess(){
    startinfo.cb = sizeof(startinfo);

    char* path = malloc(64);
    strcpy_s(path, 64, "C:\\WINDOWS\\system32\\cmd.exe");

    CreateProcess(NULL, path, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startinfo, &processinfo);

    free(path);
    WaitForSingleObject(processinfo.hProcess, INFINITE);
}

// ClearConsole is not working properly!!!
// ATTENTION
void clearConsole(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Variables for the number of characters written
    DWORD charsWritten;

    // Fill the console with spaces to clear the text
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, (COORD){0, 0}, &charsWritten);

    // Reset the text attributes (color, etc.)
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, (COORD){0, 0}, &charsWritten);

    // Move the cursor to the top-left corner
    SetConsoleCursorPosition(hConsole, (COORD){0, 0});
}

void closeProcess(){
    CloseHandle(processinfo.hProcess);
    CloseHandle(processinfo.hThread);
}

int main(){
    createProcess();
    closeProcess();
}