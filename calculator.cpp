#define UNICODE
#include <windows.h>
#include <string>
#include <sstream>

#define ID_BUTTON_0 100
#define ID_BUTTON_1 101
#define ID_BUTTON_2 102
#define ID_BUTTON_3 103
#define ID_BUTTON_4 104
#define ID_BUTTON_5 105
#define ID_BUTTON_6 106
#define ID_BUTTON_7 107
#define ID_BUTTON_8 108
#define ID_BUTTON_9 109
#define ID_BUTTON_ADD 110
#define ID_BUTTON_SUB 111
#define ID_BUTTON_MUL 112
#define ID_BUTTON_DIV 113
#define ID_BUTTON_EQ 114
#define ID_BUTTON_CLEAR 115

HWND hDisplay;
double num1 = 0, num2 = 0;
WCHAR op = 0;
bool isOpPressed = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Create display
            hDisplay = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_READONLY,
                                    10, 10, 200, 30, hwnd, NULL, NULL, NULL);

            // Create buttons
            CreateWindow(L"BUTTON", L"7", WS_CHILD | WS_VISIBLE, 10, 50, 40, 40, hwnd, (HMENU)ID_BUTTON_7, NULL, NULL);
            CreateWindow(L"BUTTON", L"8", WS_CHILD | WS_VISIBLE, 60, 50, 40, 40, hwnd, (HMENU)ID_BUTTON_8, NULL, NULL);
            CreateWindow(L"BUTTON", L"9", WS_CHILD | WS_VISIBLE, 110, 50, 40, 40, hwnd, (HMENU)ID_BUTTON_9, NULL, NULL);
            CreateWindow(L"BUTTON", L"/", WS_CHILD | WS_VISIBLE, 160, 50, 40, 40, hwnd, (HMENU)ID_BUTTON_DIV, NULL, NULL);

            CreateWindow(L"BUTTON", L"4", WS_CHILD | WS_VISIBLE, 10, 100, 40, 40, hwnd, (HMENU)ID_BUTTON_4, NULL, NULL);
            CreateWindow(L"BUTTON", L"5", WS_CHILD | WS_VISIBLE, 60, 100, 40, 40, hwnd, (HMENU)ID_BUTTON_5, NULL, NULL);
            CreateWindow(L"BUTTON", L"6", WS_CHILD | WS_VISIBLE, 110, 100, 40, 40, hwnd, (HMENU)ID_BUTTON_6, NULL, NULL);
            CreateWindow(L"BUTTON", L"*", WS_CHILD | WS_VISIBLE, 160, 100, 40, 40, hwnd, (HMENU)ID_BUTTON_MUL, NULL, NULL);

            CreateWindow(L"BUTTON", L"1", WS_CHILD | WS_VISIBLE, 10, 150, 40, 40, hwnd, (HMENU)ID_BUTTON_1, NULL, NULL);
            CreateWindow(L"BUTTON", L"2", WS_CHILD | WS_VISIBLE, 60, 150, 40, 40, hwnd, (HMENU)ID_BUTTON_2, NULL, NULL);
            CreateWindow(L"BUTTON", L"3", WS_CHILD | WS_VISIBLE, 110, 150, 40, 40, hwnd, (HMENU)ID_BUTTON_3, NULL, NULL);
            CreateWindow(L"BUTTON", L"-", WS_CHILD | WS_VISIBLE, 160, 150, 40, 40, hwnd, (HMENU)ID_BUTTON_SUB, NULL, NULL);

            CreateWindow(L"BUTTON", L"0", WS_CHILD | WS_VISIBLE, 10, 200, 40, 40, hwnd, (HMENU)ID_BUTTON_0, NULL, NULL);
            CreateWindow(L"BUTTON", L"C", WS_CHILD | WS_VISIBLE, 60, 200, 40, 40, hwnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);
            CreateWindow(L"BUTTON", L"=", WS_CHILD | WS_VISIBLE, 110, 200, 40, 40, hwnd, (HMENU)ID_BUTTON_EQ, NULL, NULL);
            CreateWindow(L"BUTTON", L"+", WS_CHILD | WS_VISIBLE, 160, 200, 40, 40, hwnd, (HMENU)ID_BUTTON_ADD, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            int id = LOWORD(wParam);
            WCHAR buffer[256];
            GetWindowText(hDisplay, buffer, 256);
            std::wstring displayText = buffer;

            if (id >= ID_BUTTON_0 && id <= ID_BUTTON_9) {
                if (isOpPressed) {
                    displayText = L"";
                    isOpPressed = false;
                }
                displayText += L'0' + (id - ID_BUTTON_0);
                SetWindowText(hDisplay, displayText.c_str());
            } else if (id == ID_BUTTON_ADD || id == ID_BUTTON_SUB || id == ID_BUTTON_MUL || id == ID_BUTTON_DIV) {
                if (!displayText.empty()) {
                    num1 = _wtof(displayText.c_str());
                    op = (id == ID_BUTTON_ADD) ? L'+' : (id == ID_BUTTON_SUB) ? L'-' : (id == ID_BUTTON_MUL) ? L'*' : L'/';
                    isOpPressed = true;
                }
            } else if (id == ID_BUTTON_EQ) {
                if (!displayText.empty() && op != 0) {
                    num2 = _wtof(displayText.c_str());
                    double result = 0;
                    switch (op) {
                        case L'+': result = num1 + num2; break;
                        case L'-': result = num1 - num2; break;
                        case L'*': result = num1 * num2; break;
                        case L'/': if (num2 != 0) result = num1 / num2; break;
                    }
                    std::wostringstream oss;
                    oss << result;
                    SetWindowText(hDisplay, oss.str().c_str());
                    op = 0;
                    isOpPressed = true;
                }
            } else if (id == ID_BUTTON_CLEAR) {
                SetWindowText(hDisplay, L"");
                num1 = num2 = 0;
                op = 0;
                isOpPressed = false;
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW),
                      (HBRUSH)(COLOR_WINDOW + 1), NULL, L"CalculatorClass", NULL };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(L"CalculatorClass", L"Calculator", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 230, 280,
                             NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}