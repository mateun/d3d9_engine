#include <fb_graphics_device.h>
#include <fb_window.h>
#include <stdio.h>

GraphicsDevice::GraphicsDevice(int w, int h, bool fullScreen) :
    _width(w), _height(h), _fullScreen(fullScreen) {

    printf("in ctr. of GraphicsDevice\n");
    printf("setting up window\n");
    HWND hw = createMainWindow();
    if (hw == 0) {
        printf("error creating window\n");
        exit(1);
    }

	// Setup D3D9 device
	_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == _d3d) {
		printf("failed to create d3d9\n");
		exit(1);
	}

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.hDeviceWindow = hw;
	pp.Windowed = !fullScreen;
	pp.BackBufferWidth = _width;
	pp.BackBufferHeight = _height;
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;

	HRESULT hr = _d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hw,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&pp,
		&_device);
	if (FAILED(hr)) {
		printf("Error while creating device\n");
		exit(1);
	}

	

}

GraphicsDevice::~GraphicsDevice() {
	if (_device != nullptr) {
		_device->Release();
		_device = nullptr;
	}

	if (_d3d != nullptr) {
		_d3d->Release();
		_d3d = nullptr;
	}
}

void GraphicsDevice::Render() {
	_device->BeginScene();
	_device->EndScene();
	_device->Present(NULL, NULL, 0, 0);
}

void GraphicsDevice::Clear() {
	// temp: set clear color to blue
	D3DCOLOR clearColor = D3DCOLOR_XRGB(10, 180, 0xef);

	_device->Clear(0, 0,
		D3DCLEAR_TARGET,
		clearColor, 0, 0);
}

FBEVENT GraphicsDevice::PollEvent() {
    MSG msg;
    if (PeekMessage(&msg,0,0,0,PM_REMOVE))
    {
        //printf("peeked message: %d\n", msg.message);
        if (msg.message == WM_QUIT)
        {
            return EVT_CLOSE;
        }

        // Translate and dispatch message
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    
    return EVT_NONE;

}