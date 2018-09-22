#include <windows.h>
#include <commctrl.h>
#include <iostream>

using namespace std;

typedef struct {
        unsigned long pid;
        HWND hWnd;
} _WinStruct;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hWnd, &pid);
    _WinStruct *ptr = (_WinStruct*)lParam;
    if (pid == ptr->pid){
        ptr->hWnd = hWnd;
        return false;
    }
    return true;
}

HWND getWindowWithPid(unsigned long pid){
    _WinStruct winStruct;
    winStruct.pid = pid;
    winStruct.hWnd = 0;
    EnumWindows(EnumWindowsProc, (LPARAM)&winStruct);
    return winStruct.hWnd;
}

int main(int argc, char *argv[]){
    HWND hWndDesktop = GetDesktopWindow();
    cout << "Desktop window found: " << hWndDesktop << endl;

    HWND hWndProgman = FindWindow("Progman", NULL);

    SendMessage(hWndProgman, 0x052C, NULL, NULL);
    hWndProgman = FindWindow("Progman", NULL);
    HWND hWndWorkerShell = NULL;
    HWND hWndWorkerW = NULL;
    do{
        hWndWorkerW = FindWindowEx(NULL, hWndWorkerW, "WorkerW", NULL);
        cout << "WorkerW window found: " << hWndWorkerW << endl;
        hWndWorkerShell =  FindWindowEx(hWndWorkerW, NULL, "SHELLDLL_DefView", NULL);
        if (hWndWorkerShell)
            break;
    } while(hWndWorkerW);

    if (hWndWorkerShell) {
        cout << "Shell Worker found: " << hWndWorkerShell << endl;
    } else {
        cout << "Shell Worker not found. Exit" << endl;
        return 1;
    }

    hWndWorkerW = FindWindowEx(NULL, hWndWorkerW, "WorkerW", NULL);
    if (hWndWorkerW) {
        cout << "Final WorkerW found: " << hWndWorkerW << endl;
    } else {
        cout << "Designate Worker not found. Exit" << endl;
        return 1;
    }
    SetWindowLong(hWndWorkerW, GWL_STYLE, WS_DISABLED);

    HWND hWndSysListView = FindWindowEx(hWndWorkerShell, NULL, "SysListView32", NULL);
    if (hWndSysListView){
        cout << "SysListView found:" << hWndSysListView << endl;
    } else {
        cout << "SysListView not found." << endl;
        return -1;
    }

    unsigned long pid = 0;
    if (argc == 2){
        pid = atol(argv[1]);
    } else if (argc == 1){
        cout << "Enter the pid of the program to be attached" << endl;
        cin >> pid;
    } else {
        cout << "Commend don't known. Exit";
        return 1;
    }

    cout << "Looking for application with pid" << endl;
    HWND hWndApp = getWindowWithPid(pid);
    if (!hWndApp){
        cout << "Application not found. Exit" << endl;
        return 1;
    }

    cout << "Attaching application to WorkerW" << endl;
    SetParent(hWndApp,hWndProgman);
    SetWindowLong(hWndProgman, GWL_EXSTYLE, WS_EX_TRANSPARENT);
    return 0;
}
