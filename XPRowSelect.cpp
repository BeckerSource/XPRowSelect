/***************************************************
No license and no support.
Use this code/program at your own risk.

v1.1:
> added support for /H argument to hide tray icon
> new 16x16 icon for tray that looks better

v1.0:
> initial release
****************************************************/

#define WINVER 0x0500 // for SetWinEventHook stuff

#include <windows.h>
#include <CommCtrl.h>
#include "resource.h"

#define XPRS_TRAY_NID_UID   100
#define XPRS_TRAY_TITLE     101
#define XPRS_TRAY_SEP       102
#define XPRS_TRAY_VERSION   103
#define XPRS_TRAY_EXIT      104
#define XPRS_TRAY_EVENT     (WM_USER + 1)

#define XPRS_CLASS_NAME     "XPRowSelect"
#define XPRS_VERSION        "XPRowSelect v1.1"
#define XPRS_INFO           "(use '/H' arg to hide tray icon)"
#define LV_CLASS_NAME       "SysListView32"

HMENU g_menu = 0;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
        case XPRS_TRAY_EVENT:
            if (lParam == WM_RBUTTONUP){
                POINT curPoint ;
                GetCursorPos(&curPoint) ;
                SetForegroundWindow(hWnd);
                if (TrackPopupMenu(g_menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hWnd, 0) == XPRS_TRAY_EXIT)
                    PostQuitMessage(0);
            }
            return 0;
        case WM_CREATE:
            g_menu = CreatePopupMenu();
            AppendMenu(g_menu, MF_DISABLED, XPRS_TRAY_VERSION, XPRS_VERSION);
            AppendMenu(g_menu, MF_DISABLED, XPRS_TRAY_VERSION, XPRS_INFO);
            AppendMenu(g_menu, MF_SEPARATOR, XPRS_TRAY_SEP, 0);
            AppendMenu(g_menu, MF_STRING, XPRS_TRAY_EXIT, "Exit");
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

bool UpdateRowSelectStyle(const HWND hWnd){
    bool res = false;

    // check for listview class...
    char class_name[32] = {0};
    if (GetClassName(hWnd, class_name, sizeof(class_name)) &&
        strcmp(class_name, LV_CLASS_NAME) == 0){
        // check for explorer-based parent class...
        if (GetClassName(GetParent(hWnd), class_name, sizeof(class_name)) &&
            (strcmp(class_name, "SHELLDLL_DefView") == 0 ||
             strcmp(class_name, "CtrlNotifySink") == 0)){

            // enforce the full row select style...
            //long style = GetWindowLong(hWnd, GWL_STYLE);
            //if ((style & LVS_REPORT) == LVS_REPORT){
                int style_ex = SendMessage(hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
                if ((style_ex & LVS_EX_FULLROWSELECT) != LVS_EX_FULLROWSELECT)
                    SendMessage(hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, style_ex | LVS_EX_FULLROWSELECT);
                res = true;
            //}
        }
    }

    return res;
}

BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam){
    bool cont = !UpdateRowSelectStyle(hWnd);
    if (cont)
        EnumChildWindows(hWnd, EnumChildProc, 0);
    return cont;
}

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime){
    //if (dwEvent == EVENT_OBJECT_CREATE)
        UpdateRowSelectStyle(hWnd);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){
    WNDCLASS wc = {0};
    wc.lpfnWndProc =        WindowProc;
    wc.hInstance =          hInstance;
    wc.lpszClassName =      XPRS_CLASS_NAME;
    unsigned short res =    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0,                      // extra styles
        XPRS_CLASS_NAME,        // window class
        XPRS_CLASS_NAME,        // title text
        WS_OVERLAPPEDWINDOW,    // visible style
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // size and pos
        0,                    // parent   
        0,                    // menu
        hInstance,            // instance handle
        0                     // extra data
        );

    if (hWnd){
        NOTIFYICONDATA *nid = 0;
        // create / show tray icon...
        if (!(pCmdLine && strlen(pCmdLine) && (strstr(pCmdLine, "/h") || strstr(pCmdLine, "/H")))){
            nid = (NOTIFYICONDATA *) calloc(1, sizeof(NOTIFYICONDATA));
            nid->cbSize = sizeof(NOTIFYICONDATA);
            nid->hWnd = hWnd;
            nid->uID = XPRS_TRAY_NID_UID;
            nid->uCallbackMessage = XPRS_TRAY_EVENT;
            nid->hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
            nid->uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
            LoadString(GetModuleHandle(0), XPRS_TRAY_TITLE, nid->szTip, 128);
            Shell_NotifyIcon(NIM_ADD, nid);    
        }

        // update existing ListViews shown in Explorer...
        HWND hWndStart = 0;
        while ((hWndStart = FindWindowEx(0, hWndStart, "CabinetWClass", 0)))
            EnumChildWindows(0, EnumChildProc, 0);
        while ((hWndStart = FindWindowEx(0, hWndStart, "ExploreWClass", 0)))
            EnumChildWindows(0, EnumChildProc, 0);

        // hook to ListView creation events (alter style of each listview after creation)...
        SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_CREATE, 0, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

        // main loop...
        MSG msg = {0};
        while (GetMessage(&msg, 0, 0, 0)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // destroy tray icon...
        if (nid){
            Shell_NotifyIcon(NIM_DELETE, nid);
            free(nid);
            nid = 0;
        }
    }

    return 0;
}
