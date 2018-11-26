#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

/**
 * ����ؼ�ID
 */
#define IDC_BUTTON_EXIT             30000       //�رհ�ť
#define IDC_COMBOBOX_SPLIST         31000       //�رհ�ť
#define IDCB_COMBOBOX_MODE          31001
/**
 * �����ؼ�����
 */
HWND m_btnExit;                         //�رհ�ť
HWND m_cbxPorts;                        //�رհ�ť

//class CString;

/**
 *
 */
void GetComList_Reg(HWND CCombox) {
    HKEY hkey;
    int result;
//    CString strComName;//��������
    int i = 0;
    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                          TEXT("Hardware\\DeviceMap\\SerialComm"),
                          NULL,
                          KEY_READ,
                          &hkey);

    if (ERROR_SUCCESS == result)   //   �򿪴���ע���
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
                //   ö�ٴ���
                break;   //   commName���Ǵ�������"COM2"
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
    DrawText(hdc, "�������", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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
    m_btnExit = CreateWindow(TEXT("Button"), TEXT("�˳�"),
                             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                             10, 60, 50, 30,
                             hwnd, (HMENU) IDC_BUTTON_EXIT, ((LPCREATESTRUCT) lParam)->hInstance, nullptr);
    m_cbxPorts = CreateWindow(TEXT("ComboBox"), nullptr,
                              WS_CHILD | WS_VISIBLE | LBS_STANDARD,
                              10, 10, 150, 80,
                              hwnd, (HMENU) IDC_COMBOBOX_SPLIST, ((LPCREATESTRUCT) lParam)->hInstance, nullptr);

    ComboBox_AddString(m_cbxPorts, TEXT("No limit"));
    GetComList_Reg(m_cbxPorts);
    MessageBox(nullptr, TEXT("�������ڳɹ�"), TEXT("��ʾ"), MB_OK);
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
    //�ڲ˵���Ӧ����ӵ����ť��Ӧ����
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
    //���д���
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
    const char *lpClassName = "FirstWin", *lpWindowName = TEXT("����");
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wndclass;

    wndclass.cbSize = sizeof(wndclass);       // ָ���ýṹ��Ĵ�С
    wndclass.style = CS_HREDRAW | CS_VREDRAW; // ��ȸ߶ȱ仯ʱ�ػ�
    wndclass.lpfnWndProc = WndProc;           // ���ڻص�����
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);    // nullptr��ʾϵͳͼ�꣬Ĭ��ͼ��
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);      // Ĭ�Ϲ��
    wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1); // ʹ�û�ˢ��䱳��ɫ��������(HBRUSH)GetStockObject(BLACK_BRUSH)
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = lpClassName;
    wndclass.hIconSm = nullptr;

    if (!RegisterClassEx(&wndclass)) {
        MessageBox(nullptr, TEXT("ע����ʧ��"), TEXT("��ʾ"), MB_ICONERROR);
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

    CenterWindow(hwnd);             // ���ô��ھ���
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);             // ���� WM_PAINT����֤����һ������ˢ����ʾ

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
