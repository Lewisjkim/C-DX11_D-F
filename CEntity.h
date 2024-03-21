#pragma once
//�ֻ��� �θ�
class CEntity 
{
private:
	static UINT g_iNextID; // ��ü�� ID�� ��ġ�� �ʰ�
	wstring		m_strName; // ��ü�� �̸� ����
	const UINT	m_iID;	   // ��ü�� ID ����

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_iID; }

public:
	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _other);	// ���� �����ڸ� ���������� �������� �� ID�� ��ø���� �ʰ� �ϱ� ���ؼ�
	virtual ~CEntity();				// ��ӹޱ⿡
	
};

