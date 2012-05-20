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
	BYTE				cbChiHuCard;			            //�Ժ��˿�
	BYTE				cbGoldCard;				            //������

	WORD				wBankerUser;						//ׯ���û�
	WORD				wProvideUser;						//�����û�
	BYTE				wWinner;	  						//Ӯ��

	LONG				lScore[GAME_PLAYER];				//�û�����
	TCHAR				szUserName[GAME_PLAYER][NAME_LEN];	//�û��ʺ�
	tagHuCardWeave		HuCardWeave;						//�������



	LONG				lHuPoint; 						//������
};

//////////////////////////////////////////////////////////////////////////

//���ֱ���
class CGameScoreWnd : public CWnd
{
	//��������
protected:
	CSkinImage				m_ImageBack;				//������Դ
	tagGameScoreInfo		m_GameScoreInfo;			//������Ϣ
	LONG					m_lTax;						//��Ϸ˰��

	//��������
public:
	//���캯��
	CGameScoreWnd();
	//��������
	virtual ~CGameScoreWnd();

	//���ܺ���
public:
	//���û���
	void SetScoreInfo(tagGameScoreInfo &GameScoreInfo);
	//����˰��
	void SetTax(LONG lTax);

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
