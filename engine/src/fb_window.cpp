#include <fb_window.h>
#include <stdio.h>



D3D9_ENGINE_API HWND createMainWindow() {

    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wndClass = {
        sizeof(WNDCLASSEX),
        CS_DBLCLKS | CS_OWNDC |
        CS_HREDRAW | CS_VREDRAW,
        MessageHandler,
        0, 
        0, 
        hInstance,
        LoadIcon(NULL, IDI_WINLOGO),
        LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)GetStockObject(WHITE_BRUSH),
        NULL,
        "WindowClass",
        LoadIcon(NULL, IDI_WINLOGO)
    };

    RegisterClassEx(&wndClass);

    return CreateWindowEx(
        NULL,
        "WindowClass",
        "D3D9EngineWindow",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        //WS_BORDER | WS_SYSMENU | WS_VISIBLE,
        200, 100,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL);
}

LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch(msg) {
        //case WM_CREATE: return 0;
        //case WM_CLOSE: printf("receive close\n"); return 0;
        case WM_DESTROY: PostQuitMessage(0); return 0;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}

