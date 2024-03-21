#include "pch.h"
#include "PrefabUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include "CLevelSaveLoad.h"
#include <Engine/CResMgr.h>

PrefabUI::PrefabUI()
    :ResUI(RES_TYPE::PREFAB)
{
    SetName("Prefab");
}

PrefabUI::~PrefabUI()
{

}

int PrefabUI::render_update()
{
    ResUI::render_update();

    if (ImGui::Button("Add", ImVec2(100, 18)))
    {
        wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
        wstring relativepath = GetTargetRes()->GetRelativePath();
        strFolderpath += relativepath;
        FILE* pFile = nullptr;
        errno_t iErrNum = _wfopen_s(&pFile, strFolderpath.c_str(), L"rb");
        int ind = 0;
        fread(&ind, sizeof(int), 1, pFile);
        CGameObject* newPrefab = CLevelSaveLoad::LoadGameObject(pFile);
      

        int num = 0;
        wstring prefname = newPrefab->GetName();
        wstring orgprefname = prefname;
        CLevel* curlevel = CLevelMgr::GetInst()->GetCurLevel();
        CLayer* curlayer = curlevel->GetLayer(ind);
        Vec3 prefpos = newPrefab->Transform()->GetWorldPos();
        vector<CGameObject*> vecObj = curlayer->GetParentObject();
        for (size_t i = 0; i < vecObj.size(); i++)
        {
            //if (vecObj[i]->GetName() == prefname)//같은 이름이 있다면 숫자 카운트
            //{
            //    num++;//1
            //    wstring idx = std::to_wstring(num);
            //    prefname = orgprefname;
            //    prefname += idx;
            //    if (vecObj[i]->GetName() != prefname)
            //    {
            //        
            //        newPrefab->SetName(prefname);
            //    }
            //}
        }
        SpawnGameObject(newPrefab, prefpos, ind);
        fclose(pFile);
    }
    return 0;
}