#include "pch.h"
#include "GraphicsShaderUI.h"
#include <Engine/CGraphicsShader.h>
#include <Engine/CResMgr.h>

GraphicsShaderUI::GraphicsShaderUI()
    : ResUI(RES_TYPE::GRAPHICS_SHADER)
{
    SetName("GraphicsShader");
}

GraphicsShaderUI::~GraphicsShaderUI()
{
}

int GraphicsShaderUI::render_update()
{
    ResUI::render_update();

    //Key
    ImGui::Text("GraphicsShader");
    ImGui::SameLine();
    Ptr<CGraphicsShader> pGShader = (CGraphicsShader*)GetTargetRes().Get();
    string strKey = string(pGShader->GetKey().begin(), pGShader->GetKey().end());
    ImGui::InputText("##GraphicsShaderUIName", (char*)strKey.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
 
    //RSType
    ImGui::Text("RS Type       ");
    ImGui::SameLine();
    ImGui::InputText("##RS_TYPE", (char*)ToString((RS_TYPE)pGShader->GetRSType()), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    
    //DSType
    ImGui::Text("DS Type       ");
    ImGui::SameLine();
    ImGui::InputText("##DS_TYPE     ", (char*)ToString((DS_TYPE)pGShader->GetDSType()), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    //BSType
    ImGui::Text("BS Type       ");
    ImGui::SameLine();
    ImGui::InputText("##BS_TYPE    ", (char*)ToString((BS_TYPE)pGShader->GetBSType()), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    //Domain
    ImGui::Text("Domain Type   ");
    ImGui::SameLine();
    ImGui::InputText("##Domain_TYPE", (char*)ToString((SHADER_DOMAIN)pGShader->GetDomain()), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


    return 0;
}

