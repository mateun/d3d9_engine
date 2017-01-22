#include <rendercommand.h>
#include <fb_graphics_device.h>

void TriangleScreenSpaceRenderCommand::exec(LPDIRECT3DDEVICE9 device) {

	TriangleScreenSpaceRenderCommandInfo* info = static_cast<TriangleScreenSpaceRenderCommandInfo*>(_data);
	device->SetStreamSource(0, info->vbuf, 0, sizeof(VertexScreenSpace));
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

}

void QuadScreenSpaceRenderCommand::exec(LPDIRECT3DDEVICE9 device) {

	TriangleScreenSpaceRenderCommandInfo* info = static_cast<TriangleScreenSpaceRenderCommandInfo*>(_data);
	device->SetStreamSource(0, info->vbuf, 0, sizeof(VertexScreenSpace));
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

}