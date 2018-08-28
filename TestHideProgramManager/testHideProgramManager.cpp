#include <windows.h>
#include <iostream>

using namespace std;

#define HIDE

int main(){
    HWND hWnd = FindWindow(NULL, "Program Manager");
#ifdef HIDE
    ShowWindow(hWnd, SW_HIDE);
#elif
    ShowWindow(hWnd, SW_RESTORE);
#endif
    return 0;
}

