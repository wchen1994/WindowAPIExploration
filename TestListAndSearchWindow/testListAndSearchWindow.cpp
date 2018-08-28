
#include <windows.h>
#include <iostream>

using namespace std;

bool listWindowParam(HWND hWnd){
    char buffer[255];
    GetWindowText(hWnd, (LPSTR)buffer, 254);
    cout << buffer << ": ";
    if (IsWindow(hWnd))
        cout << "IsWindow ";
    if (IsWindowVisible(hWnd))
        cout << "IsWindowVisible ";
    cout << endl;
    return true;
}

bool listVisibleWindow(HWND hWnd){
    if (IsWindow(hWnd) && IsWindowVisible(hWnd)){
        char buffer[255];
        GetWindowText(hWnd, (LPSTR)buffer, 254);
        cout << buffer << endl;
    }
    return true;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam){
    return listWindowParam(hWnd);
 //   return listVisibleWindow(hWnd);
}

int main(){
    EnumWindows(EnumWindowsProc, NULL);
    return 0;
}

