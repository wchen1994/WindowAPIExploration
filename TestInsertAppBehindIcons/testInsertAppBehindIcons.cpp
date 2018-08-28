#include <windows.h>

//https://www.bbsmax.com/A/xl56X9yYzr/

#define MAX_LOADSTRING 100
const char g_szClassName[] = "myWindowClass";

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
// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE | WS_EX_NOACTIVATE,
        g_szClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    CheckParent(hwnd);
    SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//	TCHAR szHello[MAX_LOADSTRING];
//	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
//
//	switch (message)
//	{
//	case WM_CREATE:
//		{
//			SetTimer(hWnd, 1, 1000, NULL);
//			DWORD dwStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
//			dwStyleEx |= 0x00080000; //WS_EX_LAYERED
//			SetWindowLong(hWnd, GWL_EXSTYLE, dwStyleEx);
//
//			typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
//			static lpfn pSetLayeredWindowAttributes = NULL;
//			if(pSetLayeredWindowAttributes == NULL)
//			{
//				pSetLayeredWindowAttributes = (lpfn)GetProcAddress(GetModuleHandle(_T("USER32.DLL")), "SetLayeredWindowAttributes");
//			}
//			if(pSetLayeredWindowAttributes != NULL)
//			{
//				pSetLayeredWindowAttributes(hWnd, 0, 100, 0x2/*LWA_ALPHA*/);
//			}
//
//		}
//
//		break;
//
//	case WM_TIMER:
//		CheckParent(hWnd);
//		break;
//
//		case WM_COMMAND:
//			wmId    = LOWORD(wParam);
//			wmEvent = HIWORD(wParam);
//			// Parse the menu selections:
//			switch (wmId)
//			{
//				case IDM_ABOUT:
//				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
//				   break;
//				case IDM_EXIT:
//				   DestroyWindow(hWnd);
//				   break;
//				default:
//				   return DefWindowProc(hWnd, message, wParam, lParam);
//			}
//			break;
//		case WM_PAINT:
//			hdc = BeginPaint(hWnd, &ps);
//			// TODO: Add any drawing code here...
//			RECT rt;
//			GetClientRect(hWnd, &rt);
//			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
//			EndPaint(hWnd, &ps);
//			break;
//		case WM_DESTROY:
//			PostQuitMessage(0);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//   }
//   return 0;
//}
//
//// Mesage handler for about box.
//LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//		case WM_INITDIALOG:
//				return TRUE;
//
//		case WM_COMMAND:
//			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//			{
//				EndDialog(hDlg, LOWORD(wParam));
//				return TRUE;
//			}
//			break;
//	}
//    return FALSE;
//}
