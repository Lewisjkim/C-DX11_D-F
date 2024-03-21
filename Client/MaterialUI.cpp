#include "pch.h"
#include "MaterialUI.h"

#include "ParamUI.h"

#include <Engine\CResMgr.h>
#include <Engine\CMaterial.h>

MaterialUI::MaterialUI()
    : ResUI(RES_TYPE::MATERIAL)
{
    SetName("Material");
}

MaterialUI::~MaterialUI()
{
}

int MaterialUI::render_update()
{
    ResUI::render_update();

    // Material 이름
    ImGui::Text("Material");
    ImGui::SameLine();

    Ptr<CMaterial> pMtrl = (CMaterial*)GetTargetRes().Get();
    string strKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
    ImGui::InputText("##MtrlUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    // GraphicsShader 이름
    ImGui::Text("Shader  ");
    ImGui::SameLine();

    Ptr<CGraphicsShader> pShader = pMtrl->GetShader();
    if (nullptr != pShader)
    {
        string strKey = string(pShader->GetKey().begin(), pShader->GetKey().end());
        ImGui::InputText("##ShaderUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    }
    else
    {
        char szEmtpy[10] = {};
        ImGui::InputText("##ShaderUIName", szEmtpy, 10, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    }

    ImGui::NewLine();
    ImGui::Text("Parameter");


    // Shader 에서 요구하는 ScalarParameter 를 UI 에 노출
    if (nullptr == pShader)
        return 0; // shader가 없을 수도 있기에 그런경우 return시킨다

    const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();

    for (size_t i = 0; i < vecScalarParam.size(); ++i)
    {
        switch (vecScalarParam[i].eParam)
        {
            // Shader Parameter 가 Int 타입인 경우
        case INT_0:
        case INT_1:
        case INT_2:
        case INT_3:
        {
            // 현재 머티리얼에 세팅된 값을 전달   
            int data = 0;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Int(vecScalarParam[i].strDesc, &data))
            {
                // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
        break;
        case FLOAT_0:
        case FLOAT_1:
        case FLOAT_2:
        case FLOAT_3:
        {
            // 현재 머티리얼에 세팅된 값을 전달   
            float data = 0;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Float(vecScalarParam[i].strDesc, &data))
            {
                // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
        break;
        case VEC2_0:
        case VEC2_1:
        case VEC2_2:
        case VEC2_3:
        {
            // 현재 머티리얼에 세팅된 값을 전달   
            Vec2 data;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Vec2(vecScalarParam[i].strDesc, &data))
            {
                // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
        break;
        case VEC4_0:
        case VEC4_1:
        case VEC4_2:
        case VEC4_3:
        {
            // 현재 머티리얼에 세팅된 값을 전달   
            Vec4 data;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Vec4(vecScalarParam[i].strDesc, &data))
            {
                // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
            break;
        case MAT_0:
        case MAT_1:
        case MAT_2:
        case MAT_3:
            break;
        }
    }

    // Shader 에서 요구하는 Texture Parameter 를 UI 에 노출
    const vector<tTexParam>& vecTexParam = pShader->GetTexParam(); //texparam vector에 들어있는 parameter들을 참조해와서
    for (size_t i = 0; i < vecTexParam.size(); ++i)
    {
        //param_Tex함수에서 listui만들어 서 클릭시 델리게이트 등록 밑 작동
        ImGui::NewLine();
        Ptr<CTexture> pCurTex = pMtrl->GetTexParam(vecTexParam[i].eParam); //i번째 parameter를 받아와서
        if (ParamUI::Param_Tex(vecTexParam[i].strDesc, pCurTex, this, (UI_DELEGATE_1)&MaterialUI::SelectTexture)) //param 텍스쳐 출력, 리스트 만들어서 항목 출력, 델리게이트 등록
        {
            m_eSelected = vecTexParam[i].eParam; // 해당 param을 선택했다고 기록
        }
        pMtrl->SetTexParam(vecTexParam[i].eParam, pCurTex);//마테리얼이 가지고있는 Texarr에 넣어준다 UpdateData를 하기 위해
    }
    return 0;
}

void MaterialUI::SelectTexture(DWORD_PTR _Key) //(델리게이트 내용) = 선택된 리스트 항목을 받으면 texparam에 등록해 준다. 그럼 다음 틱에 render
{
    string strKey = (char*)_Key;
    Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));//선택된 항목의 텍스쳐를 찾고

    Ptr<CMaterial> pMtrl = (CMaterial*)GetTargetRes().Get();//해당 리소스의 마테리얼을 받아서
    pMtrl->SetTexParam(m_eSelected, pTex);
}