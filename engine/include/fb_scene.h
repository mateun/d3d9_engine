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
		
		
		TriangleScreenSpaceRenderCommandInfo* info = new TriangleScreenSpaceRenderCommandInfo();
		info->vbuf = vb;
		graphicsDevice.addRenderCommand(std::move(std::make_unique<QuadScreenSpaceRenderCommand>(info)));
	}
private:
	int _w;
	int _h;
	LPDIRECT3DVERTEXBUFFER9 vb;
};


/**
 * This node is a mesh of vertices which are given in model space.
 */
class D3D9_ENGINE_API MeshNode : public SceneNode {
public:
	MeshNode(const MeshNode&) = delete;
	MeshNode& operator=(const MeshNode&) = delete;
	MeshNode(GraphicsDevice& gd, D3DXVECTOR3 pos, std::vector<VertexPosColor> vertices) : SceneNode(pos) {

		
		// Store the incoming original vertices in our node.
		_vertices = std::move(vertices);

		// Fill all vertices of the mesh into our node-local 
		// vertex buffer.
		BYTE* vbStart;
		
		UINT length = _vertices.size() * sizeof(VertexPosColor);
		gd.getD3DDevice()->CreateVertexBuffer(length,
			D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_DIFFUSE,
			D3DPOOL_MANAGED,
			&vb,
			NULL);

		HRESULT hr = vb->Lock(0, 0, (void**)&vbStart, 0);
		if (FAILED(hr)) {
			printf("faild to lock vb\n");
			exit(1);
		}

		memcpy(vbStart, _vertices.data(), length);
		vb->Unlock();
		if (FAILED(hr)) {
			printf("faild to unlock vb\n");
			exit(1);
		}

	}
	virtual ~MeshNode() {
		if (vb != nullptr) {
			vb->Release();
			vb = nullptr;
		}
	}

	virtual void Render(GraphicsDevice& graphicsDevice) override {
		MeshRenderCommandInfo* info = new MeshRenderCommandInfo();
		info->vbuf = vb;
		info->numberOfTriangles = _vertices.size() / 3;
		info->pos = _position;
		graphicsDevice.addRenderCommand(std::move(std::make_unique<MeshRenderCommand>(info)));
	}
private:
	int _w;
	int _h;
	LPDIRECT3DVERTEXBUFFER9 vb;
	std::vector<VertexPosColor> _vertices;
};

class D3D9_ENGINE_API TexturedMeshNode : public SceneNode {
public:
	TexturedMeshNode(const MeshNode&) = delete;
	TexturedMeshNode& operator=(const TexturedMeshNode&) = delete;
	TexturedMeshNode(GraphicsDevice& gd, D3DXVECTOR3 pos, std::vector<VertexPosTexNormal> vertices, const std::string& textureName) : SceneNode(pos) {

		HRESULT hr =D3DXCreateTextureFromFile(gd.getD3DDevice(), textureName.c_str(), &_tex);
		if (FAILED(hr)) {
			printf("Failed to load texture\n");
			exit(1);
		}

		// Store the incoming original vertices in our node.
		_vertices = std::move(vertices);

		// Fill all vertices of the mesh into our node-local 
		// vertex buffer.
		BYTE* vbStart;

		UINT length = _vertices.size() * sizeof(VertexPosTexNormal);
		gd.getD3DDevice()->CreateVertexBuffer(length,
			D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_NORMAL |D3DFVF_TEX1,
			D3DPOOL_MANAGED,
			&vb,
			NULL);

		hr = vb->Lock(0, 0, (void**)&vbStart, 0);
		if (FAILED(hr)) {
			printf("faild to lock vb\n");
			exit(1);
		}

		memcpy(vbStart, _vertices.data(), length);
		vb->Unlock();
		if (FAILED(hr)) {
			printf("faild to unlock vb\n");
			exit(1);
		}

	}
	virtual ~TexturedMeshNode() {
		if (vb != nullptr) {
			vb->Release();
			vb = nullptr;
		}
	}

	virtual void Render(GraphicsDevice& graphicsDevice) override {
		TexturedMeshRenderCommandInfo* info = new TexturedMeshRenderCommandInfo();
		info->vbuf = vb;
		info->numberOfTriangles = _vertices.size() / 3;
		info->pos = _position;
		info->tex = _tex;
		graphicsDevice.addRenderCommand(std::move(std::make_unique<TexturedMeshRenderCommand>(info)));
	}
private:
	int _w;
	int _h;
	LPDIRECT3DVERTEXBUFFER9 vb;
	LPDIRECT3DTEXTURE9 _tex;
	std::vector<VertexPosTexNormal> _vertices;
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

class D3D9_ENGINE_API SceneSimple : public Scene {
public:
	SceneSimple();
	virtual ~SceneSimple();
	SceneSimple(const SceneSimple&) = delete;
	SceneSimple& operator=(const SceneSimple&) = delete;
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