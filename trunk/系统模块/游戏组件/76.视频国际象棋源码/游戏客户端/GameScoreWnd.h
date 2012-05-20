#include "Stdafx.h"
#include "Resource.h"

#pragma once

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagGameScoreInfo
{
	LONG							lScore[GAME_PLAYER];				//�û�����
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];	//�û��ʺ�
};

//////////////////////////////////////////////////////////////////////////

//���ֱ���
class CGameScoreWnd : public CWnd
{
	//������Ϣ
protected:
	tagGameScoreInfo				m_GameScoreInfo;					//������Ϣ

	//������Ϣ
protected:
	CSize							m_ScoreSize;						//���ڴ�С
	CSkinImage						m_ImageBack;						//������Դ

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

	//��Ϣ����
public:
	//��ȡ��С
	const CSize & GetGameScoreSize() { return m_ScoreSize; }

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
