#pragma once
#include "d3d9_engine_api.h"
#include "vertex_definitions.h"
#include "fb_graphics_device.h"
#include "rendercommand.h"
#include <d3dx9.h>
#include <vector>
#include <memory>

class D3D9_ENGINE_API SceneNode {
public:
	SceneNode(D3DXVECTOR3 pos) : _position(pos) {}
	virtual ~SceneNode() {}
	virtual void Render(GraphicsDevice& device) {

	};

protected:
	D3DXVECTOR3 _position;
};

class D3D9_ENGINE_API QuadNode : public SceneNode {
public:
	QuadNode(D3DXVECTOR3 pos, int width, int height, LPDIRECT3DDEVICE9 device) : SceneNode(pos), _w(width), _h(height) {
		VertexScreenSpaceColor tris[] = {
			{ pos.x,pos.y, 0, 1, 0xFFFFFFFF },
			{ pos.x + width, pos.y + height, 0, 1, 0xFFFFFFFF },
			{ pos.x, pos.y + height, 0, 1, 0xFFFFFFFF },

			{ pos.x, pos.y, 0, 1, 0xFFFFFFFF },
			{ pos.x + width, pos.y, 0, 1, 0xFFFFFFFF },
			{ pos.x + width, pos.y + height, 0, 1, 0xFFFFFFFF },

		};

		
		BYTE* vbStart;
		device->CreateVertexBuffer(sizeof(tris),
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

		

		
		
	}
	virtual ~QuadNode() {
		if (vb != nullptr) {
			vb->Release();
			vb = nullptr;
		}
	}

	virtual void Render(GraphicsDevice& graphicsDevice) override {
		graphicsDevice.getD3DDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		
		TriangleScreenSpaceRenderCommandInfo* info = new TriangleScreenSpaceRenderCommandInfo();
		info->vbuf = vb;
		graphicsDevice.addRenderCommand(std::move(std::make_unique<QuadScreenSpaceRenderCommand>(info)));
	}
private:
	int _w;
	int _h;
	LPDIRECT3DVERTEXBUFFER9 vb;
};

class D3D9_ENGINE_API Scene {
public:
	Scene();
	virtual ~Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual void Render(GraphicsDevice& device) = 0;
	virtual void addNode(std::unique_ptr<SceneNode> n) = 0;
	
};

class D3D9_ENGINE_API SceneSimpleScreenSpace : public Scene {
public:
	SceneSimpleScreenSpace();
	virtual ~SceneSimpleScreenSpace();
	SceneSimpleScreenSpace(const SceneSimpleScreenSpace&) = delete;
	SceneSimpleScreenSpace& operator=(const SceneSimpleScreenSpace&) = delete;
	virtual void Render(GraphicsDevice& device) override {
		for each(const std::unique_ptr<SceneNode>& n in _nodes) {
			n->Render(device);
		}
	}

	virtual void addNode(std::unique_ptr<SceneNode> n) {
		_nodes.push_back(std::move(n));
	}

protected:
	std::vector<std::unique_ptr<SceneNode>> _nodes;
};