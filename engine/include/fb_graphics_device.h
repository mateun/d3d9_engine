#pragma once
#include "d3d9_engine_api.h"
#include "fb_window.h"
#include "d3d9.h"

class D3D9_ENGINE_API GraphicsDevice {
public:
    GraphicsDevice(int w, int h, bool fullScreen);
	~GraphicsDevice();
    void Clear();
    void Render();
    FBEVENT PollEvent();

private:
    int _width;
    int _height;
    bool _fullScreen;

    // D3D9 stuff
	LPDIRECT3D9 _d3d;
	LPDIRECT3DDEVICE9 _device;

    
};