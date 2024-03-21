#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"

#include "CMeshRender.h"
#include "CMaterial.h"

#include "CResMgr.h"

CAnimator2D::CAnimator2D()	: 
	CComponent(COMPONENT_TYPE::ANIMATOR2D),
	m_pCurAnim(nullptr),
	m_bRepeat(false)
{
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
	
}

void CAnimator2D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		if (m_bRepeat && m_pCurAnim->IsFinish()) // anim = 반복O, 끝이면
		{
			m_pCurAnim->Reset();// reset
		}

		m_pCurAnim->finaltick(); // 다음 프레임으로 넘겨주기
	}
}

void CAnimator2D::UpdateData()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial(); // 재질을 가져오고

	const tAnim2DFrm& frm = m_pCurAnim->GetCurFrame(); // 현재 프레임 ref
	Vec2 vBackSize = m_pCurAnim->GetBackSize(); // 현재 애니메이션의 back size

	int iAnimUse = 1;
	pMtrl->SetScalarParam(INT_0, &iAnimUse); // register b1의 변수들에 재질정보를 넣고
	pMtrl->SetScalarParam(VEC2_0, &frm.LeftTopUV);
	pMtrl->SetScalarParam(VEC2_1, &frm.SliceUV);
	pMtrl->SetScalarParam(VEC2_2, &frm.Offset);
	pMtrl->SetScalarParam(VEC2_3, &vBackSize);

	pMtrl->SetTexParam(TEX_0, m_pCurAnim->GetAtlasTex()); // register t0에 Texture 객체를 가져온다
}

void CAnimator2D::Clear()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	int iAnimUse = 0;
	pMtrl->SetScalarParam(INT_0, &iAnimUse);

	Ptr<CTexture> pTex = nullptr;
	pMtrl->SetTexParam(TEX_0, pTex);
}

void CAnimator2D::Play(const wstring& _strName, bool _bRepeat)
{
	CAnim2D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_pCurAnim = pAnim;
	m_bRepeat = _bRepeat;
}

CAnim2D* CAnimator2D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator2D::DelAnim(const wstring& _strName)
{
	CAnim2D* anim = FindAnim(_strName);
	
	m_mapAnim.erase(_strName);
	delete anim;
}

CAnim2D* CAnimator2D::LoadAnimationfromfile()
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Load(); //texture setting
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));

	return pAnim;
}

CAnim2D* CAnimator2D::LoadAnimation(const wstring& _strRelativePath)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Load(_strRelativePath);
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));

	return pAnim;
}

void CAnimator2D::CreateAnimation(const wstring& _strAnimName,
	Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice ,Vec2 _vBackSize,
	int _FrameCount, int _FPS, Vec2 _Offset)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Create(_strAnimName, _AtlasTex, _vLeftTop, _vSlice, _vBackSize, _FrameCount, _FPS,  _Offset);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}

void CAnimator2D::SaveToLevelFile(FILE* _File)
{
	SaveWString(m_pCurAnim->GetName(), _File);
	SaveWString(m_pCurAnim->GetAtlasTex()->GetKey(), _File);
	SaveWString(m_pCurAnim->GetAtlasTex()->GetRelativePath(), _File);
	Vec2 m_backsize = m_pCurAnim->GetBackSize();
	fwrite(&m_backsize, sizeof(Vec2), 1, _File);
	size_t iFrmCount = m_pCurAnim->GetFramSize();
	fwrite(&iFrmCount, sizeof(size_t), 1, _File);
	
	for (size_t i = 0; i < iFrmCount; ++i)
	{
		fwrite(&m_pCurAnim->GetFrame()[i].LeftTopUV, sizeof(Vec2), 1, _File);
		fwrite(&m_pCurAnim->GetFrame()[i].SliceUV, sizeof(Vec2), 1, _File);
		fwrite(&m_pCurAnim->GetFrame()[i].Offset, sizeof(Vec2), 1, _File);
		fwrite(&m_pCurAnim->GetFrame()[i].fDuration, sizeof(float), 1, _File);
	}

}

void CAnimator2D::LoadFromLevelFile(FILE* _File)
{
	CAnim2D* pAnim = new CAnim2D;

	wstring m_name;
	LoadWString(m_name, _File);//
	wstring m_key;
	LoadWString(m_key, _File);//
	wstring m_relativepath;
	LoadWString(m_relativepath, _File);//
	Vec2 m_backSize;
	fread(&m_backSize, sizeof(Vec2), 1, _File);//
	size_t m_framesize;
	fread(&m_framesize, sizeof(size_t), 1, _File);//
	
	for (size_t i = 0; i < m_framesize; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm.LeftTopUV, sizeof(Vec2), 1, _File);
		fread(&frm.SliceUV, sizeof(Vec2), 1, _File);
		fread(&frm.Offset, sizeof(Vec2), 1, _File);
		fread(&frm.fDuration, sizeof(float), 1, _File);
		pAnim->GetFrame().push_back(frm);
	}
	
	Ptr<CTexture>       m_AtlasTex = CResMgr::GetInst()->Load<CTexture>(m_key, m_relativepath);
	pAnim->SetName(m_name);
	pAnim->SetAtlasTex(m_AtlasTex);
	pAnim->SetBackSize(m_backSize);
	m_mapAnim.insert(make_pair(m_name, pAnim));
	Play(pAnim->GetName(), true);
}

