#include "Stdafx.h"
#include "Resource.h"

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagGameScoreInfo
{
	BYTE							cbChiHuCard;						//�Ժ��˿�
	WORD							wBankerUser;						//ׯ���û�
	WORD							wProvideUser;						//�����û�
	LONG							lGameTax;							//��Ϸ˰��
	LONG							lScore[GAME_PLAYER];				//�û�����
	WORD							wCardType[GAME_PLAYER];				//��������
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];	//�û��ʺ�
	WORD							wwChiHuKind;						//�Ժ�����
	WORD							wChiHuRight;						//����Ȩλ

};

//////////////////////////////////////////////////////////////////////////

//���ֱ���
class CGameScoreWnd : public CWnd
{
	//��������
protected:
	CSkinImage						m_ImageBack;						//������Դ
	tagGameScoreInfo				m_GameScoreInfo;					//������Ϣ

	//��������
public:
	//���캯��
	CGameScoreWnd();
	//��������
	virtual ~CGameScoreWnd();

	//���ܺ���
public:
	//���û���
	void SetScoreInfo(tagGameScoreInfo & GameScoreInfo);

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
};

//////////////////////////////////////////////////////////////////////////
