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
	if (nullptr != GetTarget()->Animator2D()) // animator comp�� �ִٸ�
		GetTarget()->Animator2D()->DelAnim(wanimName);
	
}

int Animator2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;
	
	

	//���� ������Ʈ�� ���� animator�� �߰������� (animation�� ���� ����) 
	if (nullptr == GetTarget()->Animator2D()->GetCurAnim())
	{

		size_t FrameSize = 0;
		m_Atlas = nullptr;//���� �ؽ���55
		tAnim2DFrm frame = {};
		vector<tAnim2DFrm> vecframe;
		//Animator ���� â================================================================
		ImGui::Begin("Animator");
		{
			//custom anim texture preview==============
			if (nullptr != LoadTexture)
				ImGui::Text("Atlas Size: %.0fx%.0f", LoadTexture->Width(), LoadTexture->Height());//�ؽ��� ������ ǥ��
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

			//�ִϸ��̼� ���� �ڽ�-----------------------
			ImGui::SameLine();
			if (ImGui::BeginChild("##frameinfo", ImVec2(300, 250), true))
			{
				//1.�ִϸ��̼� �̸� = Ű��
				ImGui::Text("AnimKey    ");
				ImGui::SameLine();
				ImGui::InputText("##AnimName", filename, IM_ARRAYSIZE(filename));
				animName = filename;//string ���� ������ �־��ش�
				wanimName.assign(animName.begin(), animName.end());


				//2.������ ���� ǥ��
				ImGui::Text("FrameCount ");
				ImGui::SameLine();
				ImGui::SliderInt("##FrameCount", &FrameCount, 0, 10);

				//3.Left Top ��ġ
				ImGui::Text("LeftTop    ");
				ImGui::SameLine();
				ImGui::InputFloat2("##LeftTopUVx", fLT);
				LT = Vec2(fLT[0], fLT[1]);

				//4.�ؽ��� �� ����
				ImGui::Text("Slice      ");
				ImGui::SameLine();
				ImGui::InputFloat2("##SliceUV", fRB);
				RB = Vec2(fRB[0], fRB[1]);

				//5.�������
				ImGui::Text("BackSize   ");
				ImGui::SameLine();
				ImGui::InputFloat2("##BackSizexy", fBackSize);
				BackSize = Vec2(fBackSize[0], fBackSize[1]);


				//6.offset �߽����� ���� ����
				ImGui::Text("Offset     ");
				ImGui::SameLine();
				ImGui::InputFloat2("##Offset", fOffset);
				Offset = Vec2(fOffset[0], fOffset[1]);

				//7.���� �ð�
				ImGui::Text("Duration   ");
				ImGui::SameLine();
				ImGui::InputFloat("##Duration", &Duration);

				ImGui::EndChild();
			}

			if (nullptr != LoadTexture)
			{
				// �ִϸ��̼� ������ �� ���============================
				for (size_t i = 0; i < FrameSize; i++)
				{
					//���
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


			//Load Atlas ��ư
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

				if (false == GetOpenFileName(&ofn))//Ž���� ����
					return true;

				FILE* pFile = nullptr; // ���� ���Ͽ� ���� pointer�� ������ ���� �����Ϳ� ���� �������Դϴ�.
				_wfopen_s(&pFile, sFilePath, L"rb");// �ش� ����� ������ ����

				if (nullptr == pFile)
				{
					wchar_t wStr[256] = {};
					MessageBox(nullptr, wStr, L"���� �ҷ����� ����", MB_OK);
				}
				
				size_t fullLen = wcslen(sFilePath);//��ü���
				wstring FilePath = sFilePath;
				size_t absLen = strFolderPath.length();//�������
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

			

			//�ִϸ��̼� ���� ��ư
			ImGui::SameLine();
			if (ImGui::Button("Save Anim", ImVec2(200, 30)))
			{
				GetTarget()->Animator2D()->FindAnim(wanimName)->Save();
			}

			//�ִϸ��̼� �ҷ����� ��ư
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

























	//Animation�� �ִ� ����========================================================


	else
	{
		//char cfilename[128] = {};
		CAnim2D* curAnim = GetTarget()->Animator2D()->GetCurAnim();
		vecFrame = curAnim->GetFrame();//frame vector
		size_t FrameSize = curAnim->GetFramSize();
		LoadTexture = curAnim->GetAtlasTex();//���� �ؽ���
		tAnim2DFrm frame = curAnim->GetCurFrame();

		//Inspector�� ��� �� ����=============================
		if (nullptr != LoadTexture)
		{
			//���� Inspector Texture
			ImGui::Text("Atlas Size: %.0fx%.0f", LoadTexture->Width(), LoadTexture->Height());//�ؽ��� ������ ǥ��
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
				// ���� Inspector �ִϸ��̼� Preview
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

			//���� �ִϸ��̼� ����
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


















		//Animator ���� â================================================================
		ImGui::Begin("Animator");
		{
			//custom anim texture preview==============
			if (nullptr != LoadTexture)
				ImGui::Text("Atlas Size: %.0fx%.0f", LoadTexture->Width(), LoadTexture->Height());//�ؽ��� ������ ǥ��
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

			//�ִϸ��̼� ���� �ڽ�-----------------------
			ImGui::SameLine();
			if (ImGui::BeginChild("##frameinfo", ImVec2(300, 250), true))
			{
				//1.�ִϸ��̼� �̸� = Ű��
				ImGui::Text("AnimKey    ");
				ImGui::SameLine();

				ImGui::InputText("##AnimKey", filename, IM_ARRAYSIZE(filename));
				animName = filename;//string ���� ������ �־��ش�
				wanimName.assign(animName.begin(), animName.end());


				//2.������ ���� ǥ��
				ImGui::Text("FrameCount ");
				ImGui::SameLine();
				ImGui::SliderInt("##FrameCount", &FrameCount, 0, 10);//***

				//3.Left Top ��ġ
				ImGui::Text("LeftTop    ");
				ImGui::SameLine();
				ImGui::InputFloat2("##LeftTopUVx", fLT);
				LT = Vec2(fLT[0], fLT[1]);

				//4.�ؽ��� �� ����
				ImGui::Text("Slice      ");
				ImGui::SameLine();
				ImGui::InputFloat2("##SliceUV", fRB);
				RB = Vec2(fRB[0], fRB[1]);

				//5.�������
				ImGui::Text("BackSize   ");
				ImGui::SameLine();
				ImGui::InputFloat2("##BackSizexy", fBackSize);
				BackSize = Vec2(fBackSize[0], fBackSize[1]);


				//6.offset �߽����� ���� ����
				ImGui::Text("Offset     ");
				ImGui::SameLine();
				ImGui::InputFloat2("##Offset", fOffset);
				Offset = Vec2(fOffset[0], fOffset[1]);

				//7.���� �ð�
				ImGui::Text("Duration   ");
				ImGui::SameLine();
				ImGui::InputFloat("##Duration", &Duration);
				ImGui::EndChild();
			}

			if (nullptr != LoadTexture)
			{
				// �ִϸ��̼� ������ �� ���============================
				for (size_t i = 0; i < FrameSize; i++)
				{
					//���
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
				//�ִϸ����Ͱ� �۵��ϰ� �ִ� ������Ʈ���� �ؽ��ĸ� ���� �� �� �ְ� �Ѵ�
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

				if (false == GetOpenFileName(&ofn))//Ž���� ����
					return true;

				//�ҷ��� �ؽ��ĸ� �̹��� ǥ��

				FILE* pFile = nullptr; // ���� ���Ͽ� ���� pointer�� ������ ���� �����Ϳ� ���� �������Դϴ�.
				_wfopen_s(&pFile, sFilePath, L"rb");// �ش� ����� ������ ����

				if (nullptr == pFile)
				{
					wchar_t wStr[256] = {};
					MessageBox(nullptr, wStr, L"���� �ҷ����� ����", MB_OK);
				}
				//loadedfilepath = strFolderPath - sFilePath;
				
				size_t fullLen = wcslen(sFilePath);//��ü���
				wstring FilePath = sFilePath;
				size_t absLen = strFolderPath.length();//�������
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

			//2.�ִϸ��̼� ����
			ImGui::SameLine();
			if (ImGui::Button("Save Anim", ImVec2(200, 30)))
			{
				GetTarget()->Animator2D()->FindAnim(wanimName)->Save();
			}

			//3.�ִϸ��̼� �ҷ�����
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

