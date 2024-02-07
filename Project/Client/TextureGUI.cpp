#include "pch.h"
#include "TextureGUI.h"

#include <Engine\Texture.h>

TextureGUI::TextureGUI() : ResourceGUI("Texture", "##TexutreGUI", RESOURCE_TYPE::TEXTURE)
{
}

TextureGUI::~TextureGUI()
{
}

void TextureGUI::RenderUpdate()
{
	Texture* tex = (Texture*)GetResouce();

	ImGui::Image(tex->GetSRV().Get(), ImVec2(150, 150));
}
