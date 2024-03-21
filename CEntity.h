#pragma once
//최상위 부모
class CEntity 
{
private:
	static UINT g_iNextID; // 객체의 ID가 겹치지 않게
	wstring		m_strName; // 객체의 이름 설정
	const UINT	m_iID;	   // 객체의 ID 설정

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_iID; }

public:
	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _other);	// 복사 생성자를 만든이유는 복사했을 때 ID가 중첩되지 않게 하기 위해서
	virtual ~CEntity();				// 상속받기에
	
};

