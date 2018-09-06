#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

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

HWND getSysListViewHWND(){
    HWND hWndShellDLL = NULL;
    HWND hWndWorkerW = NULL;
    HWND hWndProgramManager = FindWindow("Progman", NULL);
    if (hWndProgramManager != NULL){
        hWndShellDLL = FindWindowEx(hWndProgramManager, NULL, "SHELLDLL_DefView", NULL);
    }
    while(hWndShellDLL == NULL){
        hWndWorkerW = FindWindowEx(NULL, hWndWorkerW, "WorkerW", NULL);
        if(hWndWorkerW == NULL)
            return 0;
        hWndShellDLL = FindWindowEx(hWndWorkerW, NULL, "SHELLDLL_DefView", NULL);
    }


    return FindWindowEx(hWndShellDLL, NULL, "SysListView32", NULL);
}

int main(int argc, char *argv[]){
    HWND hWndSysListView = getSysListViewHWND();
    if (hWndSysListView == NULL){
        printf("Failed find SysListView32.");
        return -1;
    }

    unsigned long pid = 0;
    if (argc == 2){
        pid = atol(argv[1]);
    }
    printf("Looking for program with pid: %ld\n", pid);
    HWND hWnd = getWindowWithPid(pid);
    if (hWnd == NULL){
        printf("Failed to find window.\n");
        return -1;
    } else {
        printf("Window found.\n");
    }

    printf("Attach to SysListView32.\n");
    SetWindowLong(hWnd, GWL_HWNDPARENT, (LONG)hWndSysListView);

    printf("Set window to noactive\n");
    SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_NOACTIVATE);

//    SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
    return 0;
}
