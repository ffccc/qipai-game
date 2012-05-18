

#pragma once

#include "Stdafx.h"

//��Ϸ��¼
class CGameLZ : public CGameWnd
{
	//��ʷ����
public:
	WORD							m_wPageNO;							//��ǰ��ʾҳ
	CArrayTemplate<tagHistory>		m_HistoryArray;						//���������ʷ��¼
	WORD							m_wHistoryWinCount[3];				//��ʷ��¼ׯ�к͵þ���

protected:

	SURFACE*					    m_pImageHistoryWin;					//Ӯ������λͼ

	SURFACE*					    m_pImageRed;							//��Ȧ

	SURFACE*					    m_pImageBlue;							//��Ȧ

	SURFACE*					    	m_pImageGreen;

	SURFACE*					    m_pImageZ;							//ׯ

	SURFACE*					    m_pImageX;							//��

	SURFACE*					    m_pImageP;							//ƽ

	SURFACE*					    m_pImageD;							//��

	SURFACE*					    m_pImageBG;							//����ͼ

	CGameButton						m_pImageClose;						//�رհ���

public:
//	���밴ť����¼�
	LRESULT OnButtonClick(CGameWnd* pGameWnd, WPARAM wParam, LPARAM lParam);

	//��������
public:
	//���캯��
	CGameLZ();
	//��������
	virtual ~CGameLZ();

	//���ܺ���
public:
	virtual void OnDraw(SURFACE* psurfTarget);
	virtual void OnSize();
	virtual bool OnCreate();
	LRESULT OnWndMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	//�滭����
	void DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource, LONG lNumber, INT nXPos, INT nYPos, BYTE cbNumCount);
	//��ʷ�������
	void SetHistoryResult(tagHistory[], WORD Count);
	void ReHistoryResult();
	//��ʷ�������
	void AddHistoryResult(BYTE cbPlayerValue, BYTE cbBankerValue, int bRefresh,bool Banker,bool Player, bool big);

};

//////////////////////////////////////////////////////////////////////////

