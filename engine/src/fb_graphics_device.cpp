#include <fb_graphics_device.h>
#include <fb_window.h>
#include <stdio.h>
#include <d3dx9.h>

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
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&pp,
		&_device);
	if (FAILED(hr)) {
		printf("Error while creating device\n");
		exit(1);
	}

	D3DXFONT_DESCA fd;
	ZeroMemory(&fd, sizeof(fd));
	fd.Height = 25;
	fd.MipLevels = 1;
	strcpy(fd.FaceName, "Courier");
	D3DXCreateFontIndirect(_device, &fd, &_defaultFont);
	

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

void GraphicsDevice::DrawQuadInScreenSpace(RECT r) {
	
	VertexScreenSpace tris[] = {
		{ r.left, r.top, 0, 1, 0xFFFFFFFF },
		{ r.right, r.bottom, 0, 1, 0xFFFFFFFF },
		{ r.left, r.bottom, 0, 1, 0xFFFFFFFF },

		{ r.left, r.top, 0, 1, 0xFFFFFFFF },
		{ r.right, r.top, 0, 1, 0xFFFFFFFF },
		{ r.right, r.bottom, 0, 1, 0xFFFFFFFF },

	};

	LPDIRECT3DVERTEXBUFFER9 vb;
	BYTE* vbStart;
	_device->CreateVertexBuffer(sizeof(tris),
		D3DUSAGE_WRITEONLY,
		D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
		D3DPOOL_MANAGED,
		&vb,
		NULL);

	HRESULT hr = vb->Lock(0, 0, (void**)&vbStart, 0);
	if (FAILED(hr)) {
		printf("faild to lock vb\n");
		exit(1);
	}
	memcpy(vbStart, tris, sizeof(tris));
	vb->Unlock();
	if (FAILED(hr)) {
		printf("faild to unlock vb\n");
		exit(1);
	}
	
	_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	TriangleScreenSpaceRenderCommandInfo* info = new TriangleScreenSpaceRenderCommandInfo();
	info->vbuf = vb;
	std::unique_ptr<QuadScreenSpaceRenderCommand> cmd = std::make_unique<QuadScreenSpaceRenderCommand>(info);
	_renderCommands.push_back(std::move(cmd));


	// horrible vb usage!! leaking!!
}

void GraphicsDevice::DrawTriangleInScreenSpace(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3) {
	VertexScreenSpace tris[] = {
		{ p1.x, p1.y, p1.z, 1, 0xFFFFFFFF },
		{ p2.x, p2.y, p2.z, 1, 0xFFFFFFFF },
		{ p3.x, p3.y, p3.z, 1, 0xFFFFFFFF },
	};

	LPDIRECT3DVERTEXBUFFER9 vb;
	BYTE* vbStart;
	_device->CreateVertexBuffer(sizeof(tris),
		D3DUSAGE_WRITEONLY,
		D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
		D3DPOOL_MANAGED,
		&vb,
		NULL);

	HRESULT hr = vb->Lock(0, 0, (void**)&vbStart, 0);
	if (FAILED(hr)) {
		printf("faild to lock vb\n");
		exit(1);
	}
	memcpy(vbStart, tris, sizeof(tris));
	vb->Unlock();
	if (FAILED(hr)) {
		printf("faild to unlock vb\n");
		exit(1);
	}
	

	_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	TriangleScreenSpaceRenderCommandInfo* info = new TriangleScreenSpaceRenderCommandInfo();
	info->vbuf = vb;
	std::unique_ptr<TriangleScreenSpaceRenderCommand> cmd = std::make_unique<TriangleScreenSpaceRenderCommand>(info);
	_renderCommands.push_back(std::move(cmd));

	// TODO 
	// this leaks the vertex buffer!
	// it is horribly inefficient to do this each frame!

}

void GraphicsDevice::DrawText(const std::string& text, int x, int y) {
	TextDrawRenderCommandInfo* info = new TextDrawRenderCommandInfo();
	info->text = text;
	info->x = x;
	info->y = y;
	info->font = _defaultFont;
	std::unique_ptr<TextDrawRenderCommand> cmd(new TextDrawRenderCommand(info));
	_renderCommands.push_back(std::move(cmd));

}

void GraphicsDevice::Render() {
	_device->BeginScene();

	for each(auto &cmd in _renderCommands) {
		cmd->exec(_device);
	}

	_device->EndScene();
	_device->Present(NULL, NULL, 0, 0);

	// Clear the render device list
	_renderCommands.clear();
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