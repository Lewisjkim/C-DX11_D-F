#include "pch.h"
#include "Animator2DUI.h"
#include <Engine\CAnim2D.h>
#include <Engine/CAnimator2D.h>
#include <Engine\CMeshRender.h>
#include <Engine\CTexture.h>

#include "commdlg.h"

#include <Engine\CPathMgr.h>
#include <Engine\CResMgr.h>
#include <Engine\CGameObject.h>
#include<Engine/CRes.h>
#include "ListUI.h"

Animator2DUI::Animator2DUI() :
	ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
	SetName("Animator2D");
	
}

Animator2DUI::~Animator2DUI()
{
	if (nullptr == GetTarget())
		return;
	if (nullptr != GetTarget()->Animator2D()) // animator comp가 있다면
		GetTarget()->Animator2D()->DelAnim(wanimName);
	
}

int Animator2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;
	
	

	//새로 오브젝트를 만들어서 animator를 추가했으면 (animation이 없는 상태) 
	if (nullptr == GetTarget()->Animator2D()->GetCurAnim())
	{

		size_t FrameSize = 0;
		m_Atlas = nullptr;//현재 텍스쳐55
		tAnim2DFrm frame = {};
		vector<tAnim2DFrm> vecframe;
		//Animator 별도 창================================================================
		ImGui::Begin("Animator");
		{
			//custom anim texture preview==============
			if (nullptr != LoadTexture)
				ImGui::Text("Atlas Size: %.0fx%.0f", LoadTexture->Width(), LoadTexture->Height());//텍스쳐 사이즈 표시
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.f, 1.f);					// Lower-right
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
			if (nullptr == LoadTexture)
			{
				ImGui::Image((ImTextureID)0, ImVec2(250.f, 250.f), uv_min, uv_max, tint_col, border_col);
			}
			else
			{
				ImGui::Image(LoadTexture->GetSRV().Get(), ImVec2(250.f, 250.f), uv_min, uv_max, tint_col, border_col);
			}

			//custom animation preview==================
			ImGui::SameLine();
			uv_min = ImVec2(frame.LeftTopUV.x, frame.LeftTopUV.y);                 // Top-left
			uv_max = ImVec2(frame.LeftTopUV.x + frame.SliceUV.x, frame.LeftTopUV.y + frame.SliceUV.y);					// Lower-right
			tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);   // No tint
			border_col = ImVec4(1.0f, 1.f, 1.0f, 0.5f); // 50% opaque white
			if (nullptr == LoadTexture)
			{
				ImGui::Image((ImTextureID)0, ImVec2(250.f, 250.f), uv_min, uv_max, tint_col, border_col);
			}
			else
			{
				ImGui::Image(LoadTexture->GetSRV().Get(), ImVec2(250.f, 250.f), uv_min, uv_max, tint_col, border_col);
			}

			//애니메이션 정보 박스-----------------------
			ImGui::SameLine();
			if (ImGui::BeginChild("##frameinfo", ImVec2(300, 250), true))
			{
				//1.애니메이션 이름 = 키값
				ImGui::Text("AnimKey    ");
				ImGui::SameLine();
				ImGui::InputText("##AnimName", filename, IM_ARRAYSIZE(filename));
				animName = filename;//string 형식 변수에 넣어준다
				wanimName.assign(animName.begin(), animName.end());


				//2.프레임 개수 표시
				ImGui::Text("FrameCount ");
				ImGui::SameLine();
				ImGui::SliderInt("##FrameCount", &FrameCount, 0, 10);

				//3.Left Top 위치
				ImGui::Text("LeftTop    ");
				ImGui::SameLine();
				ImGui::InputFloat2("##LeftTopUVx", fLT);
				LT = Vec2(fLT[0], fLT[1]);

				//4.텍스쳐 끝 지점
				ImGui::Text("Slice      ");
				ImGui::SameLine();
				ImGui::InputFloat2("##SliceUV", fRB);
				RB = Vec2(fRB[0], fRB[1]);

				//5.백사이즈
				ImGui::Text("BackSize   ");
				ImGui::SameLine();
				ImGui::InputFloat2("##BackSizexy", fBackSize);
				BackSize = Vec2(fBackSize[0], fBackSize[1]);


				//6.offset 중심으로 부터 간격
				ImGui::Text("Offset     ");
				ImGui::SameLine();
				ImGui::InputFloat2("##Offset", fOffset);
				Offset = Vec2(fOffset[0], fOffset[1]);

				//7.지속 시간
				ImGui::Text("Duration   ");
				ImGui::SameLine();
				ImGui::InputFloat("##Duration", &Duration);

				ImGui::EndChild();
			}

			if (nullptr != LoadTexture)
			{
				// 애니메이션 프레임 별 출력============================
				for (size_t i = 0; i < FrameSize; i++)
				{
					//출력
					uv_min = ImVec2(vecFrame[i].LeftTopUV.x, vecFrame[i].LeftTopUV.y);  // Top-left
					uv_max = ImVec2(vecFrame[i].LeftTopUV.x + vecFrame[i].SliceUV.x, vecFrame[i].LeftTopUV.y + vecFrame[i].SliceUV.y);					// Lower-right
					tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);   // No tint
					border_col = ImVec4(1.0f, 1.f, 1.0f, 0.5f); // 50% opaque white

					if (nullptr == LoadTexture)
					{
						ImGui::Image((ImTextureID)0, ImVec2(80.f, 80.f), uv_min, uv_max, tint_col, border_col);
					}
					else
					{
						ImGui::Image(LoadTexture->GetSRV().Get(), ImVec2(80.f, 80.f), uv_min, uv_max, tint_col, border_col);
					}
					if (i != FrameSize - 1)
						ImGui::SameLine();
				}
			}


			//Load Atlas 버튼
			if (ImGui::Button("Load Atlas", ImVec2(200, 30)))
			{
				OPENFILENAME ofn = {};

				wstring strFolderPath = CPathMgr::GetInst()->GetContentPath();
				strFolderPath/* += L"texture\\"*/;

				wchar_t sFilePath[256] = {};

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFile = sFilePath;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = 256;
				ofn.lpstrFilter = L"All Files\0*.*\0\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = strFolderPath.c_str();
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if (false == GetOpenFileName(&ofn))//탐색기 종료
					return true;

				FILE* pFile = nullptr; // 열린 파일에 대한 pointer를 수신할 파일 포인터에 대한 포인터입니다.
				_wfopen_s(&pFile, sFilePath, L"rb");// 해당 경로의 파일을 연다

				if (nullptr == pFile)
				{
					wchar_t wStr[256] = {};
					MessageBox(nullptr, wStr, L"파일 불러오기 실패", MB_OK);
				}
				
				size_t fullLen = wcslen(sFilePath);//전체경로
				wstring FilePath = sFilePath;
				size_t absLen = strFolderPath.length();//폴더경로
				wstring relativepath = FilePath.substr(absLen, fullLen - absLen);
				LoadTexture = CResMgr::GetInst()->Load<CTexture>(wanimName, relativepath);

				fclose(pFile);
			}

			ImGui::SameLine();
			if (ImGui::Button("Play Anim", ImVec2(200, 30)))
			{
				GetTarget()->Animator2D()->CreateAnimation(wanimName, LoadTexture, LT, RB, BackSize, FrameCount, Duration, Offset);
				GetTarget()->Animator2D()->Play(wanimName, true);
			}

			

			//애니메이션 저장 버튼
			ImGui::SameLine();
			if (ImGui::Button("Save Anim", ImVec2(200, 30)))
			{
				GetTarget()->Animator2D()->FindAnim(wanimName)->Save();
			}

			//애니메이션 불러오기 버튼
			ImGui::SameLine();
			if (ImGui::Button("Load Anim", ImVec2(200, 30)))
			{
				pAnim = GetTarget()->Animator2D()->LoadAnimationfromfile();
				LoadTexture = pAnim->GetAtlasTex();
				wanimName = pAnim->GetName();
				GetTarget()->Animator2D()->Play(wanimName, true);
			}
		}
		ImGui::End();
		return TRUE;
	}

























	//Animation이 있는 상태========================================================


	else
	{
		//char cfilename[128] = {};
		CAnim2D* curAnim = GetTarget()->Animator2D()->GetCurAnim();
		vecFrame = curAnim->GetFrame();//frame vector
		size_t FrameSize = curAnim->GetFramSize();
		LoadTexture = curAnim->GetAtlasTex();//현재 텍스쳐
		tAnim2DFrm frame = curAnim->GetCurFrame();

		//Inspector에 출력 될 내용=============================
		if (nullptr != LoadTexture)
		{
			//현재 Inspector Texture
			ImGui::Text("Atlas Size: %.0fx%.0f", LoadTexture->Width(), LoadTexture->Height());//텍스쳐 사이즈 표시
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.f, 1.f);					// Lower-right
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
			if (nullptr == LoadTexture)
			{
				ImGui::Image((ImTextureID)0, ImVec2(150.f, 150.f), uv_min, uv_max, tint_col, border_col);
			}
			else
			{
				ImGui::Image(LoadTexture->GetSRV().Get(), ImVec2(150.f, 150.f), uv_min, uv_max, tint_col, border_col);
			}

			ImGui::SameLine();
			{
				// 현재 Inspector 애니메이션 Preview
				uv_min = ImVec2(frame.LeftTopUV.x, frame.LeftTopUV.y);                 // Top-left
				uv_max = ImVec2(frame.LeftTopUV.x + frame.SliceUV.x, frame.LeftTopUV.y + frame.SliceUV.y);					// Lower-right
				tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);   // No tint
				border_col = ImVec4(1.0f, 1.f, 1.0f, 0.5f); // 50% opaque white
				if (nullptr == LoadTexture)
				{
					ImGui::Image((ImTextureID)0, ImVec2(150.f, 150.f), uv_min, uv_max, tint_col, border_col);
				}
				else
				{
					ImGui::Image(LoadTexture->GetSRV().Get(), ImVec2(150.f, 150.f), uv_min, uv_max, tint_col, border_col);
				}
			}

			//현재 애니메이션 정보
			//Animation Key
			ImGui::Text("AnimKey");
			string strKey = string(LoadTexture->GetKey().begin(), LoadTexture->GetKey().end());
			ImGui::InputText("##CurAnimKey", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

			//FrameCount
			ImGui::Text("FrameCount");
			int fsize = (int)vecFrame.size();
			ImGui::InputInt("##CurAnimFrameCount", &fsize, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			//Left Top
			ImGui::Text("LeftTop");
			int aLT[2] = { curAnim->GetCurFrame().LeftTopUV.x * LoadTexture->Width(), frame.LeftTopUV.y * LoadTexture->Height() };
			ImGui::InputInt2("##CurAnimLeftTop", aLT, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			//Slice
			ImGui::Text("Slice");
			int aSlice[2] = { frame.SliceUV.x * LoadTexture->Width(), frame.SliceUV.y * LoadTexture->Height() };
			ImGui::InputInt2("##CurAnimSlice", aSlice, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			//Back Size
			ImGui::Text("BackSize");
			int aBack[2] = { curAnim->GetBackSize().x * LoadTexture->Width(), curAnim->GetBackSize().y * LoadTexture->Height() };
			ImGui::InputInt2("##CurAnimBackSize", aBack, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			//Offset
			ImGui::Text("Offset");
			int aOffset[2] = { frame.Offset.x * LoadTexture->Width(), frame.Offset.y * LoadTexture->Height() };
			ImGui::InputInt2("##CurAnimOffset", aOffset, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			//Duration
			ImGui::Text("Duration");
			float aDuration = 1.f / frame.fDuration;
			ImGui::InputFloat("##CurAnimDuration", &aDuration, 0, 0, "%.1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
		}


















		//Animator 별도 창================================================================
		ImGui::Begin("Animator");
		{
			//custom anim texture preview==============
			if (nullptr != LoadTexture)
				ImGui::Text("Atlas Size: %.0fx%.0f", LoadTexture->Width(), LoadTexture->Height());//텍스쳐 사이즈 표시
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.f, 1.f);					// Lower-right
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
			if (nullptr == LoadTexture)
			{
				ImGui::Image((ImTextureID)0, ImVec2(250.f, 250.f), uv_min, uv_max, tint_col, border_col);
			}
			else
			{
				ImGui::Image(LoadTexture->GetSRV().Get(), ImVec2(250.f, 250.f), uv_min, uv_max, tint_col, border_col);
			}

			//custom animation preview==================
			ImGui::SameLine();
			uv_min = ImVec2(frame.LeftTopUV.x, frame.LeftTopUV.y);                 // Top-left
			uv_max = ImVec2(frame.LeftTopUV.x + frame.SliceUV.x, frame.LeftTopUV.y + frame.SliceUV.y);					// Lower-right
			tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);   // No tint
			border_col = ImVec4(1.0f, 1.f, 1.0f, 0.5f); // 50% opaque white
			if (nullptr == LoadTexture)
			{
				ImGui::Image((ImTextureID)0, ImVec2(250.f, 250.f), uv_min, uv_max, tint_col, border_col);
			}
			else
			{
				ImGui::Image(LoadTexture->GetSRV().Get(), ImVec2(250.f, 250.f), uv_min, uv_max, tint_col, border_col);
			}

			//애니메이션 정보 박스-----------------------
			ImGui::SameLine();
			if (ImGui::BeginChild("##frameinfo", ImVec2(300, 250), true))
			{
				//1.애니메이션 이름 = 키값
				ImGui::Text("AnimKey    ");
				ImGui::SameLine();

				ImGui::InputText("##AnimKey", filename, IM_ARRAYSIZE(filename));
				animName = filename;//string 형식 변수에 넣어준다
				wanimName.assign(animName.begin(), animName.end());


				//2.프레임 개수 표시
				ImGui::Text("FrameCount ");
				ImGui::SameLine();
				ImGui::SliderInt("##FrameCount", &FrameCount, 0, 10);//***

				//3.Left Top 위치
				ImGui::Text("LeftTop    ");
				ImGui::SameLine();
				ImGui::InputFloat2("##LeftTopUVx", fLT);
				LT = Vec2(fLT[0], fLT[1]);

				//4.텍스쳐 끝 지점
				ImGui::Text("Slice      ");
				ImGui::SameLine();
				ImGui::InputFloat2("##SliceUV", fRB);
				RB = Vec2(fRB[0], fRB[1]);

				//5.백사이즈
				ImGui::Text("BackSize   ");
				ImGui::SameLine();
				ImGui::InputFloat2("##BackSizexy", fBackSize);
				BackSize = Vec2(fBackSize[0], fBackSize[1]);


				//6.offset 중심으로 부터 간격
				ImGui::Text("Offset     ");
				ImGui::SameLine();
				ImGui::InputFloat2("##Offset", fOffset);
				Offset = Vec2(fOffset[0], fOffset[1]);

				//7.지속 시간
				ImGui::Text("Duration   ");
				ImGui::SameLine();
				ImGui::InputFloat("##Duration", &Duration);
				ImGui::EndChild();
			}

			if (nullptr != LoadTexture)
			{
				// 애니메이션 프레임 별 출력============================
				for (size_t i = 0; i < FrameSize; i++)
				{
					//출력
					uv_min = ImVec2(vecFrame[i].LeftTopUV.x, vecFrame[i].LeftTopUV.y);  // Top-left
					uv_max = ImVec2(vecFrame[i].LeftTopUV.x + vecFrame[i].SliceUV.x, vecFrame[i].LeftTopUV.y + vecFrame[i].SliceUV.y);					// Lower-right
					tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);   // No tint
					border_col = ImVec4(1.0f, 1.f, 1.0f, 0.5f); // 50% opaque white

					if (nullptr == LoadTexture)
					{
						ImGui::Image((ImTextureID)0, ImVec2(80.f, 80.f), uv_min, uv_max, tint_col, border_col);
					}
					else
					{
						ImGui::Image(LoadTexture->GetSRV().Get(), ImVec2(80.f, 80.f), uv_min, uv_max, tint_col, border_col);
					}
					if (i != FrameSize - 1)
						ImGui::SameLine();
				}
			}


			//1.Load Atlas Texture*********************
			if (ImGui::Button("Load Atlas", ImVec2(200, 30)))
			{
				//애니메이터가 작동하고 있는 오브젝트에서 텍스쳐를 변경 할 수 있게 한다
				LoadTexture = nullptr;
				GetTarget()->Animator2D()->DelAnim(curAnim->GetName());

				OPENFILENAME ofn = {};

				wstring strFolderPath = CPathMgr::GetInst()->GetContentPath();
				strFolderPath/* += L"texture\\"*/;

				wchar_t sFilePath[256] = {};

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFile = sFilePath;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = 256;
				ofn.lpstrFilter = L"All Files\0*.*\0\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = strFolderPath.c_str();
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if (false == GetOpenFileName(&ofn))//탐색기 종료
					return true;

				//불러온 텍스쳐를 이미지 표시

				FILE* pFile = nullptr; // 열린 파일에 대한 pointer를 수신할 파일 포인터에 대한 포인터입니다.
				_wfopen_s(&pFile, sFilePath, L"rb");// 해당 경로의 파일을 연다

				if (nullptr == pFile)
				{
					wchar_t wStr[256] = {};
					MessageBox(nullptr, wStr, L"파일 불러오기 실패", MB_OK);
				}
				//loadedfilepath = strFolderPath - sFilePath;
				
				size_t fullLen = wcslen(sFilePath);//전체경로
				wstring FilePath = sFilePath;
				size_t absLen = strFolderPath.length();//폴더경로
				wstring relativepath = FilePath.substr(absLen, fullLen-absLen);
				 
				LoadTexture = CResMgr::GetInst()->Load<CTexture>(wanimName, relativepath);
				GetTarget()->Animator2D()->DelAnim(wanimName);
				GetTarget()->Animator2D()->CreateAnimation(wanimName, LoadTexture, LT, RB, BackSize, FrameCount, Duration, Offset);
				GetTarget()->Animator2D()->Play(wanimName, true);
				fclose(pFile);
			}

			ImGui::SameLine();
			if (ImGui::Button("Play Anim", ImVec2(200, 30)))
			{
				GetTarget()->Animator2D()->DelAnim(wanimName);
				GetTarget()->Animator2D()->CreateAnimation(wanimName, LoadTexture, LT, RB, BackSize, FrameCount, Duration, Offset);
				GetTarget()->Animator2D()->Play(wanimName, true);
			}

			//2.애니메이션 저장
			ImGui::SameLine();
			if (ImGui::Button("Save Anim", ImVec2(200, 30)))
			{
				GetTarget()->Animator2D()->FindAnim(wanimName)->Save();
			}

			//3.애니메이션 불러오기
			ImGui::SameLine();
			if (ImGui::Button("Load Anim", ImVec2(200, 30)))
			{
				GetTarget()->Animator2D()->DelAnim(wanimName);
				pAnim = GetTarget()->Animator2D()->LoadAnimationfromfile();
				LoadTexture = pAnim->GetAtlasTex();
				curAnim = pAnim;

				GetTarget()->Animator2D()->Play(pAnim->GetName(), true);
			}
		}
		ImGui::End();
	}
	
	return TRUE;
}

