#include "pch.h"
#include "ParamUI.h"

#include "ImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

#include <Engine/CResMgr.h>

// 정적맴버 초기화
UINT ParamUI::g_NextId = 0;


int ParamUI::Param_Int(const string& _strDesc, int* _pData, bool _bDrag)
{
    ImGui::Text(_strDesc.c_str());// 받아온 Desc을 출력
    ImGui::SameLine(100);

    string strIntName = GetNextName("##Param_Int");//뒤에 숨은 iD값을 붙혀준다
    ImGui::SetNextItemWidth(150);

    if (_bDrag)
    {
        if (ImGui::DragInt(strIntName.c_str(), _pData))
        {
            return 1;
        }
    }

    else
    {
        if (ImGui::InputInt(strIntName.c_str(), _pData))//위에 설정한 key값으로 설정된 input 값을 출력 
        {
            return 1;
        }
    }

    return 0;
}

int ParamUI::Param_Float(const string& _strDesc, float* _pData, bool _bDrag)
{
    ImGui::Text(_strDesc.c_str());
    ImGui::SameLine(100);

    string strIntName = GetNextName("##Param_Int");
    ImGui::SetNextItemWidth(150);

    if (_bDrag)
    {
        if (ImGui::DragFloat(strIntName.c_str(), _pData))
        {
            return 1;
        }
    }

    else
    {
        if (ImGui::InputFloat(strIntName.c_str(), _pData))
        {
            return 1;
        }
    }
    return 0;
}

int ParamUI::Param_Vec2(const string& _strDesc, Vec2* _pData, bool _bDrag)
{
    ImGui::Text(_strDesc.c_str());
    ImGui::SameLine(100);

    string strIntName = GetNextName("##Param_Int");
    ImGui::SetNextItemWidth(150);

    float arrFloat[2] = { _pData->x, _pData->y };

    if (_bDrag)
    {
        if (ImGui::DragFloat2(strIntName.c_str(), arrFloat))
        {
            _pData->x = arrFloat[0];
            _pData->y = arrFloat[1];
            return 1;
        }
    }

    else
    {
        if (ImGui::InputFloat2(strIntName.c_str(), arrFloat))
        {
            _pData->x = arrFloat[0];
            _pData->y = arrFloat[1];
            return 1;
        }
    }
    return 0;
}

int ParamUI::Param_Vec4(const string& _strDesc, Vec4* _pData, bool _bDrag)
{
    ImGui::Text(_strDesc.c_str());
    ImGui::SameLine(100);

    string strIntName = GetNextName("##Param_Int");
    ImGui::SetNextItemWidth(200);


    if (_bDrag)
    {
        if (ImGui::DragFloat4(strIntName.c_str(), *_pData))
        {
            return 1;
        }
    }

    else
    {
        if (ImGui::InputFloat4(strIntName.c_str(), *_pData))
        {
            return 1;
        }
    }
    return 0;
}

int ParamUI::Param_Bool(const string& _strDesc, bool* _pData, bool _bDrag)
{
    ImGui::Text(_strDesc.c_str());
    ImGui::SameLine(100);

    string strIntName = GetNextName("##Param_bool");
    ImGui::SetNextItemWidth(200);

    int ibool = *_pData;
    if (_bDrag)
    {
        if (ImGui::DragInt(strIntName.c_str(), &ibool))
        {
            return 1;
        }
    }

    else
    {
        if (ImGui::InputInt(strIntName.c_str(), &ibool))
        {
            return 1;
        }
    }
    return 0;
}

int ParamUI::Param_Tex(const string& _strDesc, Ptr<CTexture>& _Tex, UI* _UI, UI_DELEGATE_1 _Func)
{

    ImGui::Text(_strDesc.c_str()); // 파라미터 Desc 를 문자로 출력

    string strIntName = GetNextName("##Param_Tex");

    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
   
    if (_Tex == nullptr)
    {
        ImGui::Image((ImTextureID)0, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
    }
    else
    {
        ImGui::Text("Atlas Size: %.0fx%.0f", _Tex->Width(), _Tex->Height());
        ImGui::Image((ImTextureID)_Tex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);//텍스쳐 이미지 출력
    }

    // 드랍 체크
    if (ImGui::BeginDragDropTarget())
    {
        // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
        const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource"); //acceptDragDropPayload효과가 뭔지?
        if (pPayLoad)
        {
            TreeNode* pNode = (TreeNode*)pPayLoad->Data;// 드래그된 페이로드의 데이터 주소값을 TreeNode포인터로 전환하고
            CRes* pRes = (CRes*)pNode->GetData();//페이로드의 주소값을 가지고 실질적인 Res 주소값을 가지고온다
            if (RES_TYPE::TEXTURE == pRes->GetType()) // Res의 Type이 Texture라면
            {
                _Tex = (CTexture*)pRes; //&_Tex에 실제 값을 변경해준다
            }
        }

        ImGui::EndDragDropTarget();

        return 0;
    }

    ImGui::SameLine();

    string strBtnName = GetNextName("##TexSelectBtn");
    if (ImGui::Button(strBtnName.c_str(), ImVec2(18, 18)))
    {
        const map<wstring, Ptr<CRes>>& mapTex = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

        ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");//리스트를 만들어서
        pListUI->Reset("Texture", ImVec2(300.f, 500.f));
        for (const auto& pair : mapTex)//리소스에 있는 텍스쳐들을 나열한다
        {
            pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
        }

        // 항목 선택시 호출받을 델리게이트 등록
        pListUI->AddDynamic_Select(_UI, _Func);

        return 1;
    }

    return 0;
}


string ParamUI::GetNextName(const string& _ShowName)
{
    char szKey[255] = {};
    strcpy_s(szKey, 255, _ShowName.c_str());//받아온 값을 넣고

    char szID[50] = "##";
    char* dest = szID;
    _itoa_s(g_NextId++, dest + 2, 50 - 2, 10);//증가시키는 id값을 문자열로 변경하고

    strcat_s(szKey, 50, szID);//붙혀준다

    return szKey;
}
