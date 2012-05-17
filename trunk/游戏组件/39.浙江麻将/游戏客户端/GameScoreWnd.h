#include "Stdafx.h"
#include "Resource.h"

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "IDataCtrl.h"

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagGameScoreInfo
{
	WORD							wBankerUser;						//ׯ���û�
	tagHuCourt						stHuCourt;							//��������
	LONG							lScore[GAME_PLAYER];				//�û�����
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];	//�û��ʺ�
};

//////////////////////////////////////////////////////////////////////////

//���ֱ���
class CGameScoreWnd : public CWnd
{
	//��������
protected:
	IDataCtrl*						m_pDataCtrl;						//���ݿ���
	CSkinImage						m_ImageBack;						//������Դ
	tagGameScoreInfo				m_GameScoreInfo;					//������Ϣ

	//��������
public:
	//���캯��
	CGameScoreWnd(IDataCtrl* pDataCtrl);
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
