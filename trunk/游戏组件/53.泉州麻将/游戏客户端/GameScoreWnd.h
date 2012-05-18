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
	LONG							lScore[GAME_PLAYER];				//�û�����
	WORD							wCardType[GAME_PLAYER];				//��������
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];	//�û��ʺ�
	char							m_cHu[100];							//������Ϣ
	BYTE							bFlowerCount[GAME_PLAYER];			//�������
	BYTE							bGoldCount[GAME_PLAYER];			//���Ƹ���
	BYTE							bGangCount[GAME_PLAYER];			//���Ʒ��� 
	BYTE							bGoOnBankerTime;					//��ׯ����
	bool							bAllSidesCard[GAME_PLAYER];			//��������
	bool							bAllSeasonCard[GAME_PLAYER];		//�����ﶬ
	bool							bAllPlantCard[GAME_PLAYER];			//÷�����
	LONG							lCellScore;							//�׷ִ�С
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
