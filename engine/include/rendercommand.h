#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

class RenderCommand {

public:
	RenderCommand(void* d) : _data(d) {}
	~RenderCommand() {
		if (_data != nullptr) {
			delete(_data);
			_data = nullptr;
		}
	}
	virtual void exec(LPDIRECT3DDEVICE9 device) {
		// just do nothing
	}
	
protected:
	void* _data;
};


// Text rendering
struct TextDrawRenderCommandInfo {
	std::string text;
	int x;
	int y;
	LPD3DXFONT font;
};

class TextDrawRenderCommand : public RenderCommand {
public:
	TextDrawRenderCommand(void* d) : RenderCommand(d) {}
	
	void exec(LPDIRECT3DDEVICE9 device) override;
};



// Triangle in screen space rendering
struct TriangleScreenSpaceRenderCommandInfo {
	LPDIRECT3DVERTEXBUFFER9 vbuf;
};

class TriangleScreenSpaceRenderCommand : public RenderCommand {
public:
	TriangleScreenSpaceRenderCommand(void* d) : RenderCommand(d) {}

	void exec(LPDIRECT3DDEVICE9 device) override;
};

class QuadScreenSpaceRenderCommand : public RenderCommand {
public:
	QuadScreenSpaceRenderCommand(void* d) : RenderCommand(d) {}

	void exec(LPDIRECT3DDEVICE9 device) override;
};


struct MeshRenderCommandInfo {
	LPDIRECT3DVERTEXBUFFER9 vbuf;
	int numberOfTriangles;
	D3DXVECTOR3 pos;
};

class MeshRenderCommand : public RenderCommand {
public:
	MeshRenderCommand(void* d) : RenderCommand(d) {}

	void exec(LPDIRECT3DDEVICE9 device) override;
};

struct TexturedMeshRenderCommandInfo {
	LPDIRECT3DVERTEXBUFFER9 vbuf;
	int numberOfTriangles;
	D3DXVECTOR3 pos;
	LPDIRECT3DTEXTURE9 tex;
};


class TexturedMeshRenderCommand : public RenderCommand {
public:
	TexturedMeshRenderCommand(void* d) : RenderCommand(d) {}

	void exec(LPDIRECT3DDEVICE9 device) override;
};