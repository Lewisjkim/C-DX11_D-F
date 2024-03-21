#include "pch.h"
#include "ComputeShaderUI.h"
#include <Engine/CComputeShader.h>
#include <Engine/CRes.h>
#include <Engine/CResMgr.h>

ComputeShaderUI::ComputeShaderUI()
    : ResUI(RES_TYPE::COMPUTE_SHADER)
{
    SetName("ComputeShader");
}

ComputeShaderUI::~ComputeShaderUI()
{
}

int ComputeShaderUI::render_update()
{
    ResUI::render_update();

  //Key
  ImGui::Text("ComputeShader");
  ImGui::SameLine();
  Ptr<CComputeShader> CShader = (CComputeShader*)GetTargetRes().Get();
  string strKey = string(CShader->GetKey().begin(), CShader->GetKey().end());
  ImGui::InputText("##ComputeShaderKey", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

  //Group Per Thread
  int gpt[3] = { CShader->GetGPTX(),CShader->GetGPTY(),CShader->GetGPTZ() };
  ImGui::Text("GroupPerTread");
  ImGui::SameLine();
  ImGui::InputInt3("##GPT", gpt, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    return 0;
}