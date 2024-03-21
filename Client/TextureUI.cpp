#include "pch.h"
#include "TextureUI.h"
#include <Engine/CTexture.h>

TextureUI::TextureUI()
    : ResUI(RES_TYPE::TEXTURE)
{
    SetName("Texture");
}

TextureUI::~TextureUI()
{
}

int TextureUI::render_update()
{
    ResUI::render_update();

    //Key
    ImGui::Text("Texture Key ");
    ImGui::SameLine();
    Ptr<CTexture> texture = (CTexture*)GetTargetRes().Get();
    string strKey = string(texture->GetKey().begin(), texture->GetKey().end());
    ImGui::InputText("##Texture Key ", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    //relativepath
    ImGui::Text("Texture Path");
    ImGui::SameLine();
    string strPath = string(texture->GetRelativePath().begin(), texture->GetRelativePath().end());
    ImGui::InputText("##Texture Path", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    //Texture size
    ImGui::Text("Texture Size");
    ImGui::SameLine();
    int tSize[2] = {texture->Width(), texture->Height()};
    ImGui::InputInt2("##Texture Size", tSize, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    //Texture
    ImGui::NewLine();
    ImGui::SameLine(93);
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
    ImGui::Image((ImTextureID)texture->GetSRV().Get(), ImVec2(texture->Width() * 400 / texture->Width(), texture->Height() * 400 / texture->Width()), uv_min, uv_max, tint_col, border_col);
  
    
    
    return 0;
}
