#include "pch.h"
#include "ParticleSystemUI.h"
#include <Engine/ptr.h>
#include <Engine/CRes.h>
#include <Engine/CResMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CDevice.h>
#include <Engine/CStructuredBuffer.h>

#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>

#include "commdlg.h"

ParticleSystemUI::ParticleSystemUI()    :
    ComponentUI("##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM),
	m_Module{}
	/*SpawnModuleOn(false),
	ColorChangeModuleOn(false),
	ScaleChangeModuleOn(false),
	AddVelocityModuleOn(false),
	DragModuleOn(false),
	NoiseForceModuleOn(false),
	RenderModuleOn(false)*/
{
    SetName("ParticleSystem");

	/*MPC = 1000;*/
	
}

ParticleSystemUI::~ParticleSystemUI()
{
	
}

void ParticleSystemUI::SaveParticle()
{
	//Save Particle
	if (ImGui::Button("Save##Particle", ImVec2(100, 18)))
	{
		OPENFILENAME ofn = {};

		wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
		strFolderpath += L"texture\\particle\\";

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"Particle\0*.particle\0ALL\0*.*";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = strFolderpath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetSaveFileName(&ofn))
			return;

		// 파일 입출력
		FILE* pFile = nullptr;
		errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"wb");

		if (nullptr == pFile)
		{
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Particle Save 실패, Error Number : %d", iErrNum);
			MessageBox(nullptr, szStr, L"파티클 저장 실패", MB_OK);
			return;
		}

		wchar_t szNum[50] = {};
		size_t iFrmCount = m_Module.AddVelocityType;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[AddVelocityType]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[StartDrag]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.StartDrag);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[EndDrag]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.EndDrag);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[StartScale]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.StartScale);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[EndScale]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.EndScale);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[fNoiseForce]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.fNoiseForce);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[fNoiseTerm]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.fNoiseTerm);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[fSphereShapeRadius]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.fSphereShapeRadius);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.iMaxParticleCount;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[iMaxParticleCount]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[MaxLifeTime]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.MaxLifeTime);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[MinLifeTime]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.MinLifeTime);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=ADDVELOCITY]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=COLOR_CHANGE]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=DRAG]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=NOISE_FORCE]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=PARTICLE_SPAWN]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=RENDER]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=SCALE_CHANGE]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[OffsetAngle]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.OffsetAngle);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.Space;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[Space]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.SpawnRate;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[SpawnRate]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.SpawnShapeType;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[SpawnShapeType]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[Speed]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.Speed);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[vBoxShapeScale]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f\n", m_Module.vBoxShapeScale.x, m_Module.vBoxShapeScale.y, m_Module.vBoxShapeScale.z);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.VelocityAlignment;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[VelocityAlignment]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_Module.VelocityScale;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[VelocityScale]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[vStartColor]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_Module.vStartColor.x, m_Module.vStartColor.y, m_Module.vStartColor.z, m_Module.vStartColor.w);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vEndColor]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_Module.vEndColor.x, m_Module.vEndColor.y, m_Module.vEndColor.z, m_Module.vEndColor.w);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[vMaxSpeed]\n");
		fwprintf_s(pFile, L"%.2f\n", m_Module.vMaxSpeed);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vMaxVelocityScale]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_Module.vMaxVelocityScale.x, m_Module.vMaxVelocityScale.y, m_Module.vMaxVelocityScale.z, m_Module.vMaxVelocityScale.w);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vSpawnColor]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_Module.vSpawnColor.x, m_Module.vSpawnColor.y, m_Module.vSpawnColor.z, m_Module.vSpawnColor.w);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vSpawnScaleMax]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_Module.vSpawnScaleMax.x, m_Module.vSpawnScaleMax.y, m_Module.vSpawnScaleMax.z, m_Module.vSpawnScaleMax.w);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vSpawnScaleMin]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_Module.vSpawnScaleMin.x, m_Module.vSpawnScaleMin.y, m_Module.vSpawnScaleMin.z, m_Module.vSpawnScaleMin.w);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vVelocityDir]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_Module.vVelocityDir.x, m_Module.vVelocityDir.y, m_Module.vVelocityDir.z, m_Module.vVelocityDir.w);
		fwprintf_s(pFile, L"\n\n");

		fclose(pFile);
	}
}

void ParticleSystemUI::LoadParticle()
{
	//Load
	ImGui::SameLine();
	if (ImGui::Button("Load##Particle", ImVec2(100, 18)))
	{
		OPENFILENAME ofn = {};

		wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
		strFolderpath += L"texture\\particle\\";

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"Particle\0*.particle\0ALL\0*.*";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = strFolderpath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetOpenFileName(&ofn))
			return;
		// 파일 입출력
		FILE* pFile = nullptr;
		errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"rb");

		if (nullptr == pFile)
		{
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Particle Load 실패, Error Number : %d", iErrNum);
			MessageBox(nullptr, szStr, L"파티클 불러오기 실패", MB_OK);
			return;
		}

		while (true)
		{
			wchar_t szBuffer[256] = {};
			fwscanf_s(pFile, L"%s", szBuffer, 256);//문자열 변수를 만들어 문자열을 읽게 할 건데

			if (!wcscmp(szBuffer, L"[AddVelocityType]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.AddVelocityType);
			}

			else if (!wcscmp(szBuffer, L"[StartDrag]"))
			{
				fwscanf_s(pFile, L"%f.2", &m_Module.StartDrag);
			}

			else if (!wcscmp(szBuffer, L"[EndDrag]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.EndDrag);
			}

			else if (!wcscmp(szBuffer, L"[StartScale]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.StartScale);
			}

			else if (!wcscmp(szBuffer, L"[EndScale]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.EndScale);
			}

			else if (!wcscmp(szBuffer, L"[fNoiseForce]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.fNoiseForce);
			}

			else if (!wcscmp(szBuffer, L"[fNoiseTerm]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.fNoiseTerm);
			}

			else if (!wcscmp(szBuffer, L"[fSphereShapeRadius]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.fSphereShapeRadius);
			}

			else if (!wcscmp(szBuffer, L"[iMaxParticleCount]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.iMaxParticleCount);
			}

			else if (!wcscmp(szBuffer, L"[MaxLifeTime]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.MaxLifeTime);
			}

			else if (!wcscmp(szBuffer, L"[MinLifeTime]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.MinLifeTime);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=PARTICLE_SPAWN]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=COLOR_CHANGE]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=SCALE_CHANGE]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=ADDVELOCITY]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=DRAG]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=NOISE_FORCE]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=RENDER]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER]);
			}

			else if (!wcscmp(szBuffer, L"[OffsetAngle]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.OffsetAngle);
			}

			else if (!wcscmp(szBuffer, L"[Space]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.Space);
			}

			else if (!wcscmp(szBuffer, L"[Space]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.Space);
			}

			else if (!wcscmp(szBuffer, L"[SpawnRate]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.SpawnRate);
			}

			else if (!wcscmp(szBuffer, L"[SpawnShapeType]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.SpawnShapeType);
			}

			else if (!wcscmp(szBuffer, L"[Speed]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.Speed);
			}

			else if (!wcscmp(szBuffer, L"[vBoxShapeScale]"))
			{
				fwscanf_s(pFile, L"%f%f%f", &m_Module.vBoxShapeScale.x, &m_Module.vBoxShapeScale.y, &m_Module.vBoxShapeScale.z);
			}

			else if (!wcscmp(szBuffer, L"[VelocityAlignment]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.VelocityAlignment);
			}

			else if (!wcscmp(szBuffer, L"[VelocityScale]"))
			{
				fwscanf_s(pFile, L"%d", &m_Module.VelocityScale);
			}

			else if (!wcscmp(szBuffer, L"[vStartColor]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_Module.vStartColor.x, &m_Module.vStartColor.y, &m_Module.vStartColor.z, &m_Module.vStartColor.w);
			}
			
			else if (!wcscmp(szBuffer, L"[vEndColor]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_Module.vEndColor.x, &m_Module.vEndColor.y, &m_Module.vEndColor.z, &m_Module.vEndColor.w);
			}

			else if (!wcscmp(szBuffer, L"[vMaxSpeed]"))
			{
				fwscanf_s(pFile, L"%f", &m_Module.vMaxSpeed);
			}

			else if (!wcscmp(szBuffer, L"[vMaxVelocityScale]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_Module.vMaxVelocityScale.x, &m_Module.vMaxVelocityScale.y, &m_Module.vMaxVelocityScale.z, &m_Module.vMaxVelocityScale.w);
			}

			else if (!wcscmp(szBuffer, L"[vSpawnColor]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_Module.vSpawnColor.x, &m_Module.vSpawnColor.y, &m_Module.vSpawnColor.z, &m_Module.vSpawnColor.w);
			}

			else if (!wcscmp(szBuffer, L"[vSpawnScaleMax]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_Module.vSpawnScaleMax.x, &m_Module.vSpawnScaleMax.y, &m_Module.vSpawnScaleMax.z, &m_Module.vSpawnScaleMax.w);
			}

			else if (!wcscmp(szBuffer, L"[vSpawnScaleMin]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_Module.vSpawnScaleMin.x, &m_Module.vSpawnScaleMin.y, &m_Module.vSpawnScaleMin.z, &m_Module.vSpawnScaleMin.w);
			}

			else if (!wcscmp(szBuffer, L"[vVelocityDir]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_Module.vVelocityDir.x, &m_Module.vVelocityDir.y, &m_Module.vVelocityDir.z, &m_Module.vVelocityDir.w);
				break;
			}
			
		}
		GetTarget()->ParticleSystem()->SetModuleData(m_Module);

		fclose(pFile);
	}
}

void ParticleSystemUI::ParticleInputs()
{
	//=======================INPUT
	
	//Max Particle 개수
	MPC = GetTarget()->ParticleSystem()->GetModuleData().iMaxParticleCount;
	ImGui::Text("MaxParticleCount ");
	ImGui::SameLine();
	ImGui::InputInt("##MPC", &MPC, 1, 1);//주소값을 반환한 인자
	m_Module.iMaxParticleCount = MPC;

	SpawnModuleOn = GetTarget()->ParticleSystem()->GetModuleData().ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN];
	ImGui::Checkbox("SpawnModule", &SpawnModuleOn);
	if (SpawnModuleOn)
	{
		
		
		//SpawnRate
		SpawnRate = GetTarget()->ParticleSystem()->GetModuleData().SpawnRate;
		ImGui::Text("SpawnRate        ");
		ImGui::SameLine();
		ImGui::InputInt("##SpawnRate", &SpawnRate, 1, 1, ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.SpawnRate = SpawnRate;

		//SpawnColor
		
		Vec4 vSpawnCol = GetTarget()->ParticleSystem()->GetModuleData().vSpawnColor;
		float fSpawnColor[3] = { vSpawnCol.x, vSpawnCol.y, vSpawnCol.z };
		ImGui::Text("SpawnColor       ");
		ImGui::SameLine();
		ImGui::InputFloat3("##SpawnColor", fSpawnColor, " % .1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		ImGui::SetNextItemWidth(130);
		ImGui::ColorPicker3("##SpawnColorPicker1", fSpawnColor, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		ImGui::ColorPicker3("##SpawnColorPicker2", fSpawnColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
		m_Module.vSpawnColor = Vec3(fSpawnColor[0], fSpawnColor[1], fSpawnColor[2]);

		//SpawnScaleMin
		Vec4 ssmin = GetTarget()->ParticleSystem()->GetModuleData().vSpawnScaleMin;
		float fSpawnScaleMin[3] = { ssmin.x, ssmin.y,ssmin.z };
		ImGui::Text("SpawnScaleMin    ");
		ImGui::SameLine();
		ImGui::InputFloat3("##SpawnScaleMin", fSpawnScaleMin, " % .1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.vSpawnScaleMin = Vec3(fSpawnScaleMin[0], fSpawnScaleMin[1], fSpawnScaleMin[2]);

		//SpawnScaleMax
		Vec4 ssmax = GetTarget()->ParticleSystem()->GetModuleData().vSpawnScaleMax;
		float fSpawnScaleMax[3] = { ssmax.x, ssmax.y,ssmax.z };
		ImGui::Text("SpawnScaleMax    ");
		ImGui::SameLine();
		ImGui::InputFloat3("##SpawnScaleMax", fSpawnScaleMax, " % .1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.vSpawnScaleMax = Vec3(fSpawnScaleMax[0], fSpawnScaleMax[1], fSpawnScaleMax[2]);

		// SpawnShapeType
		SpawnShapeType = GetTarget()->ParticleSystem()->GetModuleData().SpawnShapeType;
		ImGui::Text("SpawnShapeType   ");
		ImGui::SameLine();
		ImGui::InputInt("##SpawnShapeType", &SpawnShapeType, 1, 1, ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		ImGui::TextColored(ImVec4(1.f, 0.4f, 0.4f, 1.f), "SpawnShapeType = 0: BOX, 1: Sphere");
		m_Module.SpawnShapeType = SpawnShapeType;
		ImGui::NewLine();

		//BoxShapeScale
		Vec3 BSS = GetTarget()->ParticleSystem()->GetModuleData().vBoxShapeScale;
		float fBoxShapeScale[3] = {BSS.x,BSS.y,BSS.z};
		ImGui::Text("BoxShapeScale    ");
		ImGui::SameLine();
		ImGui::InputFloat3("##BoxShapeScale", fBoxShapeScale, " % .1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.vBoxShapeScale = Vec3(fBoxShapeScale[0], fBoxShapeScale[1], fBoxShapeScale[2]);
		
		//SphereShapeRadius
		SphereShapeRadius = GetTarget()->ParticleSystem()->GetModuleData().fSphereShapeRadius;
		ImGui::Text("SphereShapeRadius");
		ImGui::SameLine();
		ImGui::InputFloat("##SphereShapeRadius", &SphereShapeRadius, 0, 0, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.fSphereShapeRadius = SphereShapeRadius;
		
		//Space
		Space = GetTarget()->ParticleSystem()->GetModuleData().Space;
		ImGui::Text("Space            ");
		ImGui::SameLine();
		ImGui::InputInt("##Space", &Space, 1, 1, ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		ImGui::TextColored(ImVec4(1.f, 0.4f, 0.4f, 1.f), "Space = 0: World, 1: Local");
		m_Module.Space = Space;
		ImGui::NewLine();
		
		//MinLifeTime float
		MinLifeTime = GetTarget()->ParticleSystem()->GetModuleData().MinLifeTime;
		ImGui::Text("MinLifeTime      ");
		ImGui::SameLine();
		ImGui::InputFloat("##MinLifeTime", &MinLifeTime, 0, 0, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.MinLifeTime = MinLifeTime;

		//MaxLifeTime float
		MaxLifeTime = GetTarget()->ParticleSystem()->GetModuleData().MaxLifeTime;
		ImGui::Text("MaxLifeTime      ");
		ImGui::SameLine();
		ImGui::InputFloat("##MaxLifeTime", &MaxLifeTime, 0, 0, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.MaxLifeTime = MaxLifeTime;
	}
	m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = SpawnModuleOn;
	
	ColorChangeModuleOn = GetTarget()->ParticleSystem()->GetModuleData().ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE];
	ImGui::Checkbox("ColorChangeModule", &ColorChangeModuleOn);
	if (ColorChangeModuleOn)
	{
		

		//StartColor Vec4
		Vec4 vStartCol = GetTarget()->ParticleSystem()->GetModuleData().vStartColor;
		float StartColor[4] = { vStartCol.x,vStartCol.y,vStartCol.z, vStartCol.w };
		ImGui::Text("StartColor       ");
		ImGui::SameLine();
		ImGui::InputFloat4("##StartColor", StartColor, " % .1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		ImGui::SetNextItemWidth(130);
		ImGui::ColorPicker4("##StartColorPicker", StartColor, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		ImGui::ColorPicker4("##SpawnColorPicker2", StartColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
		m_Module.vStartColor = Vec4(StartColor[0], StartColor[1], StartColor[2], StartColor[3]);

		//EndColor Vec4
		Vec4 vEndCol = GetTarget()->ParticleSystem()->GetModuleData().vEndColor;
		float EndColor[4] = { vEndCol.x,vEndCol.y,vEndCol.z, vEndCol.w };
		ImGui::Text("EndColor         ");
		ImGui::SameLine();
		ImGui::InputFloat4("##EndColor", EndColor, " % .1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		ImGui::SetNextItemWidth(130);
		ImGui::ColorPicker4("##EndColorPicker", EndColor, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		ImGui::ColorPicker4("##SpawnColorPicker2", EndColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
		m_Module.vEndColor = Vec4(EndColor[0], EndColor[1], EndColor[2], EndColor[3]);
	}
	m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = ColorChangeModuleOn;

	//==============여기부터 다시
	GetTarget()->ParticleSystem()->GetModuleData().ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE];
	ImGui::Checkbox("ScaleChangeModule", &ScaleChangeModuleOn);
	if (ScaleChangeModuleOn)
	{
		//	StartScale
		StartScale = GetTarget()->ParticleSystem()->GetModuleData().StartScale;
		ImGui::Text("StartScale       ");
		ImGui::SameLine();
		ImGui::InputFloat("##StartScale", &StartScale, 0, 0, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.StartScale = StartScale;

		//EndScale
		EndScale = GetTarget()->ParticleSystem()->GetModuleData().EndScale;
		ImGui::Text("EndScale         ");
		ImGui::SameLine();
		ImGui::InputFloat("##EndScale", &EndScale, 0, 0, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.EndScale = EndScale;
	}
	m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = ScaleChangeModuleOn;

	AddVelocityModuleOn = GetTarget()->ParticleSystem()->GetModuleData().ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY];
	ImGui::Checkbox("AddVelocityModule", &AddVelocityModuleOn);
	if (AddVelocityModuleOn)
	{
		Vec4 vVelDir = GetTarget()->ParticleSystem()->GetModuleData().vVelocityDir;
		float fVelocityDir[4] = { vVelDir.x, vVelDir.y, vVelDir.z, vVelDir.w };
		ImGui::Text("VelocityDir      ");
		ImGui::SameLine();
		ImGui::InputFloat4("##VelocityDir", fVelocityDir, " % .1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.vVelocityDir = Vec4(fVelocityDir[0], fVelocityDir[1], fVelocityDir[2], fVelocityDir[3]);

		AddVelocityType = GetTarget()->ParticleSystem()->GetModuleData().AddVelocityType;
		ImGui::Text("AddVelocityType  ");
		ImGui::SameLine();
		ImGui::InputInt("##AddVelocityType", &AddVelocityType, 0, 0);
		m_Module.AddVelocityType = AddVelocityType;
		ImGui::TextColored(ImVec4(1.f, 0.4f, 0.4f, 1.f), "AddVelocityType = 0: From Center, 1: To Center, 2: Fixed Direction");
		ImGui::NewLine();

		OffsetAngle = GetTarget()->ParticleSystem()->GetModuleData().OffsetAngle;
		ImGui::Text("OffSetAngle      ");
		ImGui::SameLine();
		ImGui::InputFloat("##OffSetAngle", &OffsetAngle, 0, 0, "%.2f");
		m_Module.OffsetAngle = OffsetAngle;

		Speed = GetTarget()->ParticleSystem()->GetModuleData().Speed;
		ImGui::Text("Speed            ");
		ImGui::SameLine();
		ImGui::InputFloat("##Speed", &Speed, 0, 0, "%.2f");
		m_Module.Speed = Speed;
	}
	m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = AddVelocityModuleOn;

	DragModuleOn = GetTarget()->ParticleSystem()->GetModuleData().ModuleCheck[(UINT)PARTICLE_MODULE::DRAG];
	ImGui::Checkbox("DragModule", &DragModuleOn);
	if (DragModuleOn)
	{
		StartDrag = GetTarget()->ParticleSystem()->GetModuleData().StartDrag;
		ImGui::Text("StartDrag        ");
		ImGui::SameLine();
		ImGui::InputFloat("##StartDrag", &StartDrag, 0, 0, "%.2f");
		m_Module.StartDrag = StartDrag;

		EndDrag = GetTarget()->ParticleSystem()->GetModuleData().EndDrag;
		ImGui::Text("EndDrag          ");
		ImGui::SameLine();
		ImGui::InputFloat("##EndDrag", &EndDrag, 0, 0, "%.2f");
		m_Module.EndDrag = EndDrag;
	}
	m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = DragModuleOn;

	NoiseForceModuleOn = GetTarget()->ParticleSystem()->GetModuleData().ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE];
	ImGui::Checkbox("NoiseForceModule", &NoiseForceModuleOn);
	if (NoiseForceModuleOn)
	{
		//// NoiseForce 모듈 - 랜덤 힘 적용	
		NoiseTerm = GetTarget()->ParticleSystem()->GetModuleData().fNoiseTerm;
		ImGui::Text("NoiseTerm        ");// 랜덤 힘 변경 간격
		ImGui::SameLine();
		ImGui::InputFloat("##NoiseTerm", &NoiseTerm, 0, 0, "%.2f");
		m_Module.fNoiseTerm = NoiseTerm;

		NoiseForce = GetTarget()->ParticleSystem()->GetModuleData().fNoiseForce;
		ImGui::Text("NoiseForce       ");// 랜덤 힘 크기
		ImGui::SameLine();
		ImGui::InputFloat("##NoiseForce", &NoiseForce, 0, 0, "%.2f");
		m_Module.fNoiseForce = NoiseForce;
	}
	m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = NoiseForceModuleOn;

	RenderModuleOn = GetTarget()->ParticleSystem()->GetModuleData().ModuleCheck[(UINT)PARTICLE_MODULE::RENDER];
	ImGui::Checkbox("RenderModule", &RenderModuleOn);
	if (RenderModuleOn)
	{
		VelocityAlignment = GetTarget()->ParticleSystem()->GetModuleData().VelocityAlignment;
		ImGui::Text("VelocityAlignment");// 속도 정렬 - 이동속도에 맞춰서  == 빌보드를 사용하면 안됨  //1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
		ImGui::SameLine();
		ImGui::InputInt("##VelocityAlignment", &VelocityAlignment, 0, 0);
		m_Module.VelocityAlignment = VelocityAlignment;
		ImGui::TextColored(ImVec4(1.f, 0.4f, 0.4f, 1.f), "VelocityAlignment =\n 0: Not Using, 1: Using Alignment (rotate to moving direction");
		ImGui::NewLine();

		VelocityScale = GetTarget()->ParticleSystem()->GetModuleData().VelocityScale;
		ImGui::Text("VelocityScale    "); //1: 속도에 따른 크기 변화 사용, 0 : 사용 안함
		ImGui::SameLine();
		ImGui::InputInt("##VelocityScale", &VelocityScale, 0, 0);
		m_Module.VelocityScale = VelocityScale;
		ImGui::TextColored(ImVec4(1.f, 0.4f, 0.4f, 1.f), "VelocityScale =\n 0: Not Using, 1: Using Scale (Scale change by Speed");
		ImGui::NewLine();

		MaxSpeed = GetTarget()->ParticleSystem()->GetModuleData().vMaxSpeed;
		ImGui::Text("MaxSpeed         ");// 최대 크기에 도달하는 속도
		ImGui::SameLine();
		ImGui::InputFloat("##MaxSpeed", &MaxSpeed, 0, 0, "%.2f");
		m_Module.vMaxSpeed = MaxSpeed;

		// 속력에 따른 크기 변화량 최대치 
		Vec4 MaxVS = GetTarget()->ParticleSystem()->GetModuleData().vMaxVelocityScale;
		float MaxVelocityScale[4] = { MaxVS.x, MaxVS.y, MaxVS.z, MaxVS.w };
		ImGui::Text("MaxVelocityScale ");
		ImGui::SameLine();
		ImGui::InputFloat4("##MaxVelocityScale", MaxVelocityScale, " % .1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
		m_Module.vMaxVelocityScale = Vec4(MaxVelocityScale[0], MaxVelocityScale[1], MaxVelocityScale[2], MaxVelocityScale[3]);

	}
	m_Module.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = RenderModuleOn;

	GetTarget()->ParticleSystem()->SetModuleData(m_Module);
}
int ParticleSystemUI::render_update()
{
	
    if (FALSE == ComponentUI::render_update())
        return FALSE;

	m_Module = GetTarget()->ParticleSystem()->GetModuleData();
	
	SaveParticle();//파티클 저장

	LoadParticle();//파티클 불러오기
	
	ParticleInputs();//파티클 정보 입력
	m_Module;
	

	//GetTarget()->ParticleSystem()->SetModuleData(m_Module);
	//ParticleSystem에있는 ModuleData에 재 설정한다
	
	return TRUE;
}