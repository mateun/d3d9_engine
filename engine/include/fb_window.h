#pragma once
#include <Windows.h>
#include <d3d9_engine_api.h>

/**
 * Engine events to hide from win specific events & messages. 
 */
enum FBEVENT {
    EVT_NONE,
    EVT_CLOSE,
    EVT_MOUSE_DOWN,
    EVT_MOUSE_UP,
};


LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
D3D9_ENGINE_API HWND createMainWindow();

