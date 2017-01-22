#include <rendercommand.h>
#include <fb_graphics_device.h>
#include <vertex_definitions.h>



void MeshRenderCommand::exec(LPDIRECT3DDEVICE9 device) {
	device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	MeshRenderCommandInfo* info = static_cast<MeshRenderCommandInfo*>(_data);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetStreamSource(0, info->vbuf, 0, sizeof(VertexPosColor));
	D3DXMATRIX viewMat;
	D3DXMatrixLookAtLH(&viewMat, &D3DXVECTOR3(0, 0, -5), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	device->SetTransform(D3DTS_VIEW, &viewMat);
	D3DXMATRIX projMat;
	D3DXMatrixPerspectiveFovLH(&projMat, D3DX_PI / 4, (float)4 / 3, 1.0f, 100.f);
	device->SetTransform(D3DTS_PROJECTION, &projMat);
	D3DXMATRIX worldMat;
	D3DXMatrixTranslation(&worldMat, info->pos.x, info->pos.y, info->pos.z);
	device->SetTransform(D3DTS_WORLD, &worldMat);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, info->numberOfTriangles);

}

void TexturedMeshRenderCommand::exec(LPDIRECT3DDEVICE9 device) {
	device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL |D3DFVF_TEX1);
	TexturedMeshRenderCommandInfo* info = static_cast<TexturedMeshRenderCommandInfo*>(_data);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetStreamSource(0, info->vbuf, 0, sizeof(VertexPosColor));
	D3DXMATRIX viewMat;
	D3DXMatrixLookAtLH(&viewMat, &D3DXVECTOR3(0, 0, -5), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	device->SetTransform(D3DTS_VIEW, &viewMat);
	D3DXMATRIX projMat;
	D3DXMatrixPerspectiveFovLH(&projMat, D3DX_PI / 4, (float)4 / 3, 1.0f, 100.f);
	device->SetTransform(D3DTS_PROJECTION, &projMat);
	D3DXMATRIX worldMat;
	D3DXMatrixTranslation(&worldMat, info->pos.x, info->pos.y, info->pos.z);
	device->SetTransform(D3DTS_WORLD, &worldMat);
	HRESULT hr = device->SetTexture(0, info->tex);
	if (FAILED(hr)) {
		printf("failed to set texture\n");
		exit(1);
	}
	hr = device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	if (FAILED(hr)) {
		printf("failed to set texture stage\n");
		exit(1);
	}
	hr = device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	if (FAILED(hr)) {
		printf("failed to set texture stage\n");
		exit(1);
	}
	hr = device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, info->numberOfTriangles);
	if (FAILED(hr)) {
		printf("failed to draw textured mesh\n");
		exit(1);
	}

}


void TriangleScreenSpaceRenderCommand::exec(LPDIRECT3DDEVICE9 device) {
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	TriangleScreenSpaceRenderCommandInfo* info = static_cast<TriangleScreenSpaceRenderCommandInfo*>(_data);
	device->SetStreamSource(0, info->vbuf, 0, sizeof(VertexScreenSpace));
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

}

void QuadScreenSpaceRenderCommand::exec(LPDIRECT3DDEVICE9 device) {
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	TriangleScreenSpaceRenderCommandInfo* info = static_cast<TriangleScreenSpaceRenderCommandInfo*>(_data);
	device->SetStreamSource(0, info->vbuf, 0, sizeof(VertexScreenSpace));
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

}