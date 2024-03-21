#include "pch.h"
#include "CAnim2D.h"
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "commdlg.h"
#include "CResMgr.h"
CAnim2D::CAnim2D()	:
	m_pOwner(nullptr),
	m_iCurFrm(0),
	m_bFinish(false),
	m_fTime(0.f)
{
}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::finaltick()
{
	if (m_bFinish)
		return;

	m_fTime += DT;
	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration; // 초과한 만큼만 프레임 추가
		++m_iCurFrm;

		if (m_vecFrm.size() <= m_iCurFrm)//현재프레임이 프레임 사이즈와 같거나 넘어가면
		{
			m_iCurFrm = m_vecFrm.size() - 1;//마지막 프레임으로 현재 프레임 고정하고
			m_bFinish = true;
		}
	}
}

void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _BackSize, int _FrameCount, int _FPS, Vec2 _Offset)
{
	assert(_AtlasTex.Get());

	SetName(_strAnimName); // 이름 설정

	m_AtlasTex = _AtlasTex; // 텍스쳐 설정

	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height()); // 텍스쳐의 가로 세로 길이

	m_vBackSize = _BackSize / vResolution;

	for (size_t i = 0; i < _FrameCount; ++i) // 프레임 개수를 순회하며
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / vResolution; //UV단위로 환산
		frm.SliceUV = _vSlice / vResolution;

		frm.Offset = _Offset;

		m_vecFrm.push_back(frm);
	}
}

//void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex
//	, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _BackSize, int _FrameCount, int _FPS, Vec2 _Offset)
//{
//	
//}


void CAnim2D::Save()
{
	OPENFILENAME ofn = {};

	wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
	strFolderpath += L"anim\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Anim\0*.anim\0ALL\0*.*";
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
		wsprintf(szStr, L"Animation Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return;
	}
	// Animation 이름 저장
	fwprintf_s(pFile, L"\n");

	fwprintf_s(pFile, L"[ANIMATION_NAME]\n");
	fwprintf_s(pFile, GetName().c_str());
	fwprintf_s(pFile, L"\n\n");

	// 아틀라스 텍스쳐 키값 저장	****
	fwprintf_s(pFile, L"[ATLAS_KEY]\n");
	fwprintf_s(pFile, m_AtlasTex->GetKey().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[ATLAS_PATH]\n");
	fwprintf_s(pFile, m_AtlasTex->GetRelativePath().c_str());
	fwprintf_s(pFile, L"\n\n");

	// Backsize
	
	m_vBackSize = m_vBackSize;
	fwprintf_s(pFile, L"[BACK_SIZE]\n");
	fwprintf_s(pFile, L"%f %f\n", m_vBackSize.x , m_vBackSize.y );
	fwprintf_s(pFile, L"\n\n");


	// 프레임 정보 저장
	wchar_t szNum[50] = {};

	fwprintf_s(pFile, L"[FRAME_COUNT]\n");

	size_t iFrmCount = m_vecFrm.size();
	_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환

	fwprintf_s(pFile, szNum);
	fwprintf_s(pFile, L"\n\n");

	

	for (size_t i = 0; i < iFrmCount; ++i)
	{
		fwprintf_s(pFile, L"[%zd_FRAME]\n", i);

		// LEFT_TOP
		fwprintf_s(pFile, L"[LEFT_TOP]\n");
		fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].LeftTopUV.x , m_vecFrm[i].LeftTopUV.y );

		// SIZE
		m_vecFrm[i].SliceUV = m_vecFrm[i].SliceUV;
		fwprintf_s(pFile, L"[SIZE]\n");
		fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].SliceUV.x , m_vecFrm[i].SliceUV.y );

		

		// OFFSET
		fwprintf_s(pFile, L"[OFFSET]\n");
		fwprintf_s(pFile, L"%f %f\n", m_vecFrm[i].Offset.x , m_vecFrm[i].Offset.y);

		// DURATION
		fwprintf_s(pFile, L"[DURATION]\n");
		fwprintf_s(pFile, L"%f\n", m_vecFrm[i].fDuration);

		

		fwprintf_s(pFile, L"\n");
	}



	fclose(pFile);
}

void CAnim2D::Load(const wstring& _strRelativePath)
{
	wstring strFilepath = CPathMgr::GetInst()->GetContentPath();
	strFilepath += _strRelativePath;

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"rb");

	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 불러오기 실패", MB_OK);
		return;
	}


	size_t iFrameCount = 0;
	wstring strAtlasKey;
	wstring strAltasRelativePath;

	while (true)
	{
		wchar_t szBuffer[256] = {};
		fwscanf_s(pFile, L"%s", szBuffer, 256);//문자열을 읽어라, 문자를 받을 곳은 버퍼

		if (!wcscmp(szBuffer, L"[ANIMATION_NAME]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			SetName(szBuffer);
		}

		else if (!wcscmp(szBuffer, L"[ATLAS_KEY]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAtlasKey = szBuffer;
		}

		else if (!wcscmp(szBuffer, L"[ATLAS_PATH]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAltasRelativePath = szBuffer;
		}

		else if (!wcscmp(szBuffer, L"[BACK_SIZE]"))
		{
			fwscanf_s(pFile, L"%f %f", &m_vBackSize.x, &m_vBackSize.y);
		}

		else if (!wcscmp(szBuffer, L"[FRAME_COUNT]"))
		{
			fwscanf_s(pFile, L"%zd", &iFrameCount);

			for (size_t i = 0; i < iFrameCount; ++i)
			{
				tAnim2DFrm frm = {};

				while (true)
				{
					wchar_t szBuffer[256] = {};
					fwscanf_s(pFile, L"%s", szBuffer, 256);

					if (!wcscmp(szBuffer, L"[LEFT_TOP]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.LeftTopUV.x, &frm.LeftTopUV.y);
					}

					else if (!wcscmp(szBuffer, L"[SIZE]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.SliceUV.x, &frm.SliceUV.y);
					}

					else if (!wcscmp(szBuffer, L"[OFFSET]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.Offset.x, &frm.Offset.y);
					}

					else if (!wcscmp(szBuffer, L"[DURATION]"))
					{
						fwscanf_s(pFile, L"%f", &frm.fDuration);
						break;
					}
				}

				m_vecFrm.push_back(frm);//프레임에 
			}

			break;
		}
	}

	m_AtlasTex = CResMgr::GetInst()->Load<CTexture>(strAtlasKey, strAltasRelativePath);

	fclose(pFile);
}


void CAnim2D::Load()
{
	OPENFILENAME ofn = {};

	wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
	strFolderpath += L"anim\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Anim\0*.anim\0ALL\0*.*";
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
	loadedfilepath = szFilePath;
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 불러오기 실패", MB_OK);
		return;
	}


	size_t iFrameCount = 0;
	wstring strAtlasKey;
	wstring strAltasRelativePath;
	
	while (true)
	{
		wchar_t szBuffer[256] = {};
		fwscanf_s(pFile, L"%s", szBuffer, 256);//문자열을 읽어라, 문자를 받을 곳은 버퍼

		if (!wcscmp(szBuffer, L"[ANIMATION_NAME]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			SetName(szBuffer);
		}

		else if (!wcscmp(szBuffer, L"[ATLAS_KEY]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAtlasKey = szBuffer;
		}

		else if (!wcscmp(szBuffer, L"[ATLAS_PATH]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAltasRelativePath = szBuffer;
		}
		if (L"" != strAtlasKey && L"" != strAltasRelativePath)
		{
			
		}
		/*if(nullptr !=m_AtlasTex)
			VeclResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());*/

		 if (!wcscmp(szBuffer, L"[BACK_SIZE]"))
		{
			/*m_vBackSize = m_vBackSize / vResolution;*/
			fwscanf_s(pFile, L"%f %f", &m_vBackSize.x, &m_vBackSize.y);
		}

		else if (!wcscmp(szBuffer, L"[FRAME_COUNT]"))
		{
			fwscanf_s(pFile, L"%zd", &iFrameCount);
			

			for (size_t i = 0; i < iFrameCount; ++i)
			{
				tAnim2DFrm frm = {};

				while (true)
				{
					wchar_t szBuffer[256] = {};
					fwscanf_s(pFile, L"%s", szBuffer, 256);

					if (!wcscmp(szBuffer, L"[LEFT_TOP]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.LeftTopUV.x, &frm.LeftTopUV.y);
					}

					else if (!wcscmp(szBuffer, L"[SIZE]"))
					{
						//frm.SliceUV = frm.SliceUV / vResolution;
						fwscanf_s(pFile, L"%f %f", &frm.SliceUV.x, &frm.SliceUV.y);
					}



					else if (!wcscmp(szBuffer, L"[OFFSET]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.Offset.x, &frm.Offset.y);
					}

					else if (!wcscmp(szBuffer, L"[DURATION]"))
					{
						frm.fDuration =frm.fDuration;
						fwscanf_s(pFile, L"%f", &frm.fDuration);
						break;
					}
					
				}
				m_vecFrm.push_back(frm);
				
			}

			break;
		}
	}
	
	m_AtlasTex = CResMgr::GetInst()->Load<CTexture>(strAtlasKey, strAltasRelativePath);
	
	//loadedfilepath = strAltasRelativePath;

	fclose(pFile);
}


void CAnim2D::SaveToLevelFile(FILE* _File)
{
	/*SaveWString(GetName(), _File);

	size_t FrameCount = m_vecFrm.size();
	fwrite(&FrameCount, sizeof(size_t), 1, _File);
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), FrameCount, _File);
	fwrite(&m_vBackSize, sizeof(Vec2), 1, _File);

	SaveResRef(m_AtlasTex.Get(), _File);*/
}

void CAnim2D::LoadFromLevelFile(FILE* _File)
{
	/*wstring name;
	LoadWString(name, _File);
	SetName(name);

	size_t FrameCount = 0;
	fread(&FrameCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < FrameCount; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm, sizeof(tAnim2DFrm), 1, _File);
		m_vecFrm.push_back(frm);
	}

	fread(&m_vBackSize, sizeof(Vec2), 1, _File);

	LoadResRef(m_AtlasTex, _File);*/
}