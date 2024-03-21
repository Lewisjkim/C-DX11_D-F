#include "pch.h"
#include "AddMaterial.h"
#include <Engine/ptr.h>
#include <Engine\components.h>
#include <Engine/CResMgr.h>

AddMaterial::AddMaterial() :
	UI("##AddMaterial")
{
	SetName("AddMaterial");
}

AddMaterial::~AddMaterial()
{
}

void AddMaterial::init()
{
}

void AddMaterial::tick()
{
   
}

int AddMaterial::render_update()
{
    //Material 이름
    
    ImGui::Text("Material Name");
    ImGui::SameLine();
    ImGui::InputText("##materialname", m_name, IM_ARRAYSIZE(m_name));
    string mname = m_name;
    wstring wmname;
    wmname.assign(mname.begin(), mname.end());

    //shader 이름으로 선택
    ImGui::Text("Shader Name  ");
    ImGui::SameLine();
    ImGui::InputText("##shadername", s_name, IM_ARRAYSIZE(s_name));
    string sname = s_name;
    wstring wsname;
    wsname.assign(sname.begin(), sname.end());

    //ScalarParam에 값 추가
    ImGui::Text("Add Int      ");
    ImGui::SameLine();
    ImGui::InputInt("##ScalarParamInt", &m_int, 0, 0);
    ImGui::Text("INT_         ");
    ImGui::SameLine();
    ImGui::InputInt("##ParamNum", &iParam);
    SCALAR_PARAM selectiParam;
    if (0 == iParam)
        selectiParam = SCALAR_PARAM::INT_0;
    else if(1 == iParam)
        selectiParam = SCALAR_PARAM::INT_1;
    else if (2 == iParam)
        selectiParam = SCALAR_PARAM::INT_2;
    else if (3 == iParam)
        selectiParam = SCALAR_PARAM::INT_3;

    ImGui::Text("Add Float    ");
    ImGui::SameLine();
    ImGui::InputFloat("##ScalarParamFloat", &m_float, 0, 0);
    ImGui::Text("FLOAT_       ");
    ImGui::SameLine();
    ImGui::InputInt("##fParamNum", &fParam);
    SCALAR_PARAM fselectiParam;
    if (0 == fParam)
        fselectiParam = SCALAR_PARAM::FLOAT_0;
    else if (1 == fParam)
       fselectiParam = SCALAR_PARAM::FLOAT_1;
    else if (2 == fParam)
        fselectiParam = SCALAR_PARAM::FLOAT_2;
    else if (3 == fParam)
        fselectiParam = SCALAR_PARAM::FLOAT_3;

    ImGui::Text("Add Vec2     ");
    ImGui::SameLine();
    ImGui::InputFloat2("##ScalarParamVec2", a_vec2);
    ImGui::Text("VEC2_        ");
    ImGui::SameLine();
    ImGui::InputInt("##v2ParamNum", &Vec2Param);
    SCALAR_PARAM v2selectiParam;
    if (0 == Vec2Param)
        v2selectiParam = SCALAR_PARAM::VEC2_0;
    else if (1 == Vec2Param)
        v2selectiParam = SCALAR_PARAM::VEC2_1;
    else if (2 == Vec2Param)
        v2selectiParam = SCALAR_PARAM::VEC2_2;
    else if (3 == Vec2Param)
        v2selectiParam = SCALAR_PARAM::VEC2_3;

    ImGui::Text("Add Vec4     ");
    ImGui::SameLine();
    ImGui::InputFloat4("##ScalarParamVec4", a_vec4);
    ImGui::Text("VEC4_        ");
    ImGui::SameLine();
    ImGui::InputInt("##v4ParamNum", &Vec4Param);
    SCALAR_PARAM v4selectiParam;
    if (0 == Vec4Param)
        v4selectiParam = SCALAR_PARAM::VEC4_0;
    else if (1 == Vec4Param)
        v4selectiParam = SCALAR_PARAM::VEC4_1;
    else if (2 == Vec4Param)
        v4selectiParam = SCALAR_PARAM::VEC4_2;
    else if (3 == Vec4Param)
        v4selectiParam = SCALAR_PARAM::VEC4_3;
    
    //Texture 선택
    ImGui::Text("Texture      ");
    ImGui::SameLine();

    ImGui::InputText("##selectTexture", t_name, IM_ARRAYSIZE(t_name));
    string tname = t_name;
    wstring wtname;
    wtname.assign(tname.begin(), tname.end());
    //Tex Param에 선택 필요
    

     //Material 저장
    if (ImGui::Button("Save Material"))
    {
        Ptr<CMaterial> pNewMtrl = new CMaterial;
        pNewMtrl->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(wsname));
        pNewMtrl->SetScalarParam(selectiParam, &m_int);
        pNewMtrl->SetScalarParam(fselectiParam, &m_float);
        pNewMtrl->SetScalarParam(v2selectiParam, &a_vec2);
        pNewMtrl->SetScalarParam(v4selectiParam, &a_vec4);
        pNewMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wtname));
        CResMgr::GetInst()->AddRes<CMaterial>(wmname, pNewMtrl);
        wstring path = L"material\\";
        wstring mtrl = L".mtrl";
        path += wmname;
        path += mtrl;
        pNewMtrl->Save(path);
    }
   
    //Material 불러오기

	return 0;
}

