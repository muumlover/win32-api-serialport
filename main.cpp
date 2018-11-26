#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

/**
 * 定义控件ID
 */
#define IDC_BUTTON_EXIT             30000       //关闭按钮
#define IDC_COMBOBOX_SPLIST         31000       //关闭按钮
#define IDCB_COMBOBOX_MODE          31001
/**
 * 声明控件变量
 */
HWND m_btnExit;                         //关闭按钮
HWND m_cbxPorts;                        //关闭按钮

//class CString;

/**
 *
 */
void GetComList_Reg(HWND CCombox) {
    HKEY hkey;
    int result;
//    CString strComName;//串口名称
    int i = 0;
    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                          TEXT("Hardware\\DeviceMap\\SerialComm"),
                          NULL,
                          KEY_READ,
                          &hkey);

    if (ERROR_SUCCESS == result)   //   打开串口注册表
    {
        TCHAR portName[0x100], commName[0x100];

        DWORD dwLong, dwSize;

//        CCombox->ResetContent();
        ComboBox_ResetContent(CCombox);
        do {
            dwSize = sizeof(portName) / sizeof(TCHAR);
            dwLong = dwSize;
            result = RegEnumValue(hkey, i, portName, &dwLong, NULL, NULL, (LPBYTE) commName, &dwSize);
            if (ERROR_NO_MORE_ITEMS == result) {
                //   枚举串口
                break;   //   commName就是串口名字"COM2"
            }
//            strComName = commName;
//            CCombox->AddString(strComName);
            ComboBox_AddString(CCombox, commName);
            i++;

        } while (true);

        RegCloseKey(hkey);
    }

//    CCombox->SetCurSel(0);
    ComboBox_SetCurSel(CCombox, 0);
}

/**
 *
 * @param hwnd
 * @param message
 * @param wParam
 * @param lParam
 */
void OnPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    hdc = BeginPaint(hwnd, &ps);
    GetClientRect(hwnd, &rect);
    DrawText(hdc, "世界你好", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    EndPaint(hwnd, &ps);
}

/**
 *
 * @param hwnd
 * @param message
 * @param wParam
 * @param lParam
 */
void OnCreate(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    m_btnExit = CreateWindow(TEXT("Button"), TEXT("退出"),
                             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                             10, 60, 50, 30,
                             hwnd, (HMENU) IDC_BUTTON_EXIT, ((LPCREATESTRUCT) lParam)->hInstance, nullptr);
    m_cbxPorts = CreateWindow(TEXT("ComboBox"), nullptr,
                              WS_CHILD | WS_VISIBLE | LBS_STANDARD,
                              10, 10, 150, 80,
                              hwnd, (HMENU) IDC_COMBOBOX_SPLIST, ((LPCREATESTRUCT) lParam)->hInstance, nullptr);

    ComboBox_AddString(m_cbxPorts, TEXT("No limit"));
    GetComList_Reg(m_cbxPorts);
    MessageBox(nullptr, TEXT("创建窗口成功"), TEXT("提示"), MB_OK);
}

/**
 *
 * @param hwnd
 * @param message
 * @param wParam
 * @param lParam
 */
void OnCommand(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmId, wmEvent;
    //在菜单响应中添加点击按钮响应函数
    wmId = LOWORD(wParam);
    wmEvent = HIWORD(wParam);
    switch (wmId) {
        case IDC_BUTTON_EXIT:
            DestroyWindow(hwnd);
            break;
        case IDC_COMBOBOX_SPLIST:
            break;
    }
}

/**
 *
 * @param hwnd
 * @param message
 * @param wParam
 * @param lParam
 * @return
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_PAINT:
            OnPaint(hwnd, message, wParam, lParam);
            break;
        case WM_CREATE:
            OnCreate(hwnd, message, wParam, lParam);
            break;
        case WM_COMMAND:
            OnCommand(hwnd, message, wParam, lParam);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

/**
 *
 * @param hwnd
 * @return
 */
int CenterWindow(HWND hwnd) {
    //居中窗口
    int scrWidth, scrHeight;
    RECT rect;

    scrWidth = GetSystemMetrics(SM_CXSCREEN);
    scrHeight = GetSystemMetrics(SM_CYSCREEN);
    GetWindowRect(hwnd, &rect);
    SetWindowPos(hwnd,
                 HWND_TOPMOST,
                 (scrWidth - rect.right) / 2,
                 (scrHeight - rect.bottom) / 2,
                 rect.right - rect.left,
                 rect.bottom - rect.top,
                 SWP_SHOWWINDOW);
}

/**
 *
 * @param hInstance
 * @param hPrevInstance
 * @param lpCmdLine
 * @param nCmdShow
 * @return
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    const char *lpClassName = "FirstWin", *lpWindowName = TEXT("标题");
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wndclass;

    wndclass.cbSize = sizeof(wndclass);       // 指定该结构体的大小
    wndclass.style = CS_HREDRAW | CS_VREDRAW; // 宽度高度变化时重绘
    wndclass.lpfnWndProc = WndProc;           // 窗口回调函数
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);    // nullptr表示系统图标，默认图标
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);      // 默认光标
    wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1); // 使用画刷填充背景色，还可以(HBRUSH)GetStockObject(BLACK_BRUSH)
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = lpClassName;
    wndclass.hIconSm = nullptr;

    if (!RegisterClassEx(&wndclass)) {
        MessageBox(nullptr, TEXT("注册类失败"), TEXT("提示"), MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(lpClassName,
                        lpWindowName,
                        WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME), //style
                        CW_USEDEFAULT,                                           // x
                        CW_USEDEFAULT,                                           // y
                        600,                                                     // width
                        400,                                                     // height
                        nullptr,                                                // hWndParent
                        nullptr,                                                // hMenu
                        hInstance,                                               // hInstance
                        nullptr);

    CenterWindow(hwnd);             // 设置窗口居中
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);             // 发送 WM_PAINT，保证窗口一定可以刷新显示

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
