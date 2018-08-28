#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

#define BUFFER_SIZE 255

#undef LOG_FOREGROUND

using namespace std;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam);
void printTopLevelWindowNames();
void printWindowName(HWND hWnd);
void maximizeWindow(HWND hWnd);
void findWindowWithId(DWORD pid);
HWND GetForegroundWindow();
int GetWindowText(HWND, LPSTR, int);
inline std::string trim(const std::string &s);
DWORD startApplication(LPCTSTR lpApplicationName);
std::string GetLastErrorAsString();

BOOL CALLBACK moveWndProc(HWND hWnd, long lParam);
void moveOtherWndToTopMost();

HWND hWndTarget = NULL;
HWND hWndLastForeground = NULL;

HWND GetShellDLLForWin7(){
    HWND hWndShell = NULL;
    HWND hWndDesktop = GetDesktopWindow();
    if(hWndDesktop == NULL)
        return NULL;
    HWND hWndWorkerW = NULL;
    while(TRUE){
        hWndWorkerW = FindWindowEx(hWndDesktop, hWndWorkerW, "WorkerW", NULL);
        if (hWndWorkerW == NULL)
            break;
        if (GetWindowLong(hWndWorkerW, GWL_STYLE) & WS_VISIBLE){
            break;
        } else {
            continue;
        }
    }
    if (hWndWorkerW != NULL){
        hWndShell = FindWindowEx(hWndWorkerW, NULL, "SHELLDLL_DefView", NULL);
    }
    return hWndShell;
}

BOOL CheckParent(HWND hWnd){
    static HWND s_hWndOldParent = NULL;

    HWND hWndProgram = NULL;
    HWND hWndShellDLL = NULL;
    hWndProgram = FindWindow("Progman", "Program Manager");
    if (hWndProgram != NULL){
        hWndShellDLL = FindWindowEx(hWndProgram, NULL, "SHELLDLL_DefView", NULL);
    }
    if (hWndShellDLL == NULL)
        hWndShellDLL = GetShellDLLForWin7();
    if (hWndShellDLL != NULL && hWndShellDLL != s_hWndOldParent){
        SetWindowLong(hWnd, GWL_HWNDPARENT, (LONG)hWndShellDLL);
        s_hWndOldParent = hWndShellDLL;
        return TRUE;
    }
    return FALSE;
}

int main() {
// Search the Application
//    hWndTarget = FindWindowA(NULL, "Test - Google Chrome");
//    hWndTarget = FindWindowA(NULL, "Test - Mozilla Firefox");
    hWndTarget = FindWindowA(NULL, "Test - Conkeror");
//    hWndTarget = FindWindowA(NULL, "Calculator");
    if (hWndTarget){
        cout << "Found: "; printWindowName(hWndTarget);}
    else
        cout << "Program not found.";

    SetWindowLong(hWndTarget, GWL_STYLE, WS_BORDER);
    SetWindowLong(hWndTarget, GWL_EXSTYLE, WS_EX_NOACTIVATE);

//    SetWindowLong(hWndTarget, GWL_STYLE, WS_OVERLAPPED);
//    SetWindowLong(hWndTarget, GWL_EXSTYLE, WS_EX_CLIENTEDGE);


//    CheckParent(hWndTarget);
    cout << GetLastErrorAsString() << endl;
    SetWindowPos(hWndTarget, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);

//    char buff[BUFFER_SIZE];
//    HWND hWndForeground = NULL;
//
//    HWND hWndStartMenu = FindWindowA(NULL, "Start menu");
//    cout << "Start Menu: ";
//    printWindowName(hWndStartMenu);
//
//    while(true){
//        hWndForeground = GetForegroundWindow();
//        GetWindowText(hWndForeground, (LPSTR)buff, BUFFER_SIZE-1);
//        string foregroundWindowName = trim(string(buff));
//
//        if (hWndForeground &&
//            foregroundWindowName.compare("Start")!=0 &&
//            foregroundWindowName.compare("Program Manager")!=0){
//#ifdef LOG_FOREGROUND
//            cout << "Foreground: " << foregroundWindowName << endl;
//#endif // LOG_FOREGROUND
//            if (hWndForeground == hWndTarget)
//                SetForegroundWindow(hWndLastForeground);
//
//        } else {
//            // Show background app
//            cout << "Found target: " << IsWindow(hWndTarget) << endl;
//            ShowWindow(hWndTarget, SW_HIDE);
//            ShowWindow(hWndTarget, SW_RESTORE);
//            SetForegroundWindow(hWndTarget);
//            SetWindowPos(hWndTarget, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
//        }
//        if (hWndForeground != hWndTarget)
//            hWndLastForeground = hWndForeground;
//
//        Sleep(200);
//    }

    return 0;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam) {
//    char buff[BUFFER_SIZE];
//    if (IsWindowVisible(hWnd)) {
//        GetWindowText(hWnd, (LPSTR) buff, BUFFER_SIZE-1);
//        printWindowName(hWnd);
//        ShowWindow(hWnd, SW_MAXIMIZE);
//    }
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    if (pid == (DWORD)lParam){
        hWndTarget = hWnd;
        return false;
    }
    return true;
}

void findWindowWithId(DWORD pid){
    EnumWindows(EnumWindowsProc, (long)pid);
}


BOOL CALLBACK moveWndProc(HWND hWnd, long lParam) {
    if (!IsWindowVisible(hWnd))
        return true;
    if (hWnd != hWndTarget){
        cout << "Bring Top Most ";
        printWindowName(hWnd);
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
//        ShowWindow(hWnd, SW_SHOW);
        return true;
    } else {
    }
    return true;
}

void moveOtherWndToTopMost(){
    EnumWindows(moveWndProc, 0);
}

void maximizeWindow(HWND hWnd){
    ShowWindow(hWnd, SW_MAXIMIZE);
}

void printWindowName(HWND hWnd){
    char buff[255];
    GetWindowText(hWnd, (LPSTR) buff, 254);
    printf("%s\n", buff);
}

DWORD startApplication(LPCTSTR lpApplicationName)
{
   // additional information
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   // set the size of the structures
   ZeroMemory( &si, sizeof(si) );
   si.cb = sizeof(si);
   ZeroMemory( &pi, sizeof(pi) );

  // start the program up
  CreateProcess( lpApplicationName,   // the path
    NULL,        // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory
    &si,            // Pointer to STARTUPINFO structure
    &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    return pi.dwProcessId;
}

inline std::string trim(const std::string &s)
{
   auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
   auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
   return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

std::string GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}
