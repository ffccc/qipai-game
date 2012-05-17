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
	BYTE							cbChiHuCard;						//�Ժ��齫
	WORD							wBankerUser;						//ׯ���û�
	WORD							wProvideUser;						//�����û�
	LONG							lScore[GAME_PLAYER];				//�û�����
	WORD							wCardType[GAME_PLAYER];				//��������
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];	//�û��ʺ�
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
};

//////////////////////////////////////////////////////////////////////////
