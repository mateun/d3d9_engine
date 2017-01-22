#include <rendercommand.h>


void TextDrawRenderCommand::exec(LPDIRECT3DDEVICE9 device) {
	
	TextDrawRenderCommandInfo* info = static_cast<TextDrawRenderCommandInfo*>(_data);
	RECT r = { info->x, info->y, 0, 0 };

	info->font->DrawTextA(NULL, info->text.c_str(), -1, &r, DT_CALCRECT, D3DCOLOR_ARGB(255, 200, 0, 0));
	info->font->DrawTextA(NULL, info->text.c_str(), -1, &r, DT_CENTER, D3DCOLOR_ARGB(255, 200, 0, 0));

}