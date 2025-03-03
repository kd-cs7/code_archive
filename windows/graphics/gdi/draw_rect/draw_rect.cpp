#include <windows.h>
#include <iostream>

HINSTANCE hInstance;
HWND hwnd;

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get the window size
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // Get the width and height
            int clientWidth = clientRect.right - clientRect.left;
            int clientHeight = clientRect.bottom - clientRect.top;

            // Size of the rectangle
            int rectWidth = 200;
            int rectHeight = 200;

            // Center position of the window
            int x = (clientWidth - rectWidth) / 2;
            int y = (clientHeight - rectHeight) / 2;

            // Draw the rectangle
            RECT rect = { x, y, x + rectWidth, y + rectHeight };
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);

            EndPaint(hwnd, &ps);
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// Initialize the window
bool InitWindow(HINSTANCE hInstance) {
    const char CLASS_NAME[] = "GDIExampleClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (RegisterClass(&wc) == 0) {
        return false;
    }

    hwnd = CreateWindowEx(
        0, CLASS_NAME, "GDI Example",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return true;
}

int main() {
    hInstance = GetModuleHandle(NULL);

    if (!InitWindow(hInstance)) {
        std::cerr << "Failed to generate a window." << std::endl;
        return -1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

