#pragma once
#include "d3d9_engine_api.h"
#include "fb_window.h"
#include "d3d9.h"
#include <d3dx9.h>
#include <string>
#include <rendercommand.h>
#include <vector>
#include <memory>

struct VertexScreenSpace {
	FLOAT x, y, z, rhw;
	DWORD color;
};

class D3D9_ENGINE_API GraphicsDevice {
public:
    GraphicsDevice(int w, int h, bool fullScreen);
	~GraphicsDevice();
	GraphicsDevice(const GraphicsDevice&) = delete;
	GraphicsDevice& operator=(const GraphicsDevice&) = delete;
	void Clear();
    void Render();
	void DrawText(const std::string& text, int x, int y);
	void DrawTriangleInScreenSpace(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3);
	void DrawQuadInScreenSpace(RECT r);
    FBEVENT PollEvent();

	void addRenderCommand(std::unique_ptr<RenderCommand> cmd) {
		_renderCommands.push_back(std::move(cmd));
	}
	LPDIRECT3DDEVICE9 getD3DDevice() {
		return _device;
	}

private:
    int _width;
    int _height;
    bool _fullScreen;
	
    // D3D9 stuff
	LPDIRECT3D9 _d3d;
	LPDIRECT3DDEVICE9 _device;
	LPD3DXFONT _defaultFont;
	std::vector<std::unique_ptr<RenderCommand>> _renderCommands;
	

    
};