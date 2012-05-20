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
class CGameScoreWnd : public CDialog
{
	//������Ϣ
protected:
	tagGameScoreInfo				m_GameScoreInfo;					//������Ϣ

	//������Ϣ
protected:
	CSkinImage						m_ImageBack;						//������Դ
	CSkinButton						m_btClose;							//�رհ�ť

	//��������
public:
	//���캯��
	CGameScoreWnd();
	//��������
	virtual ~CGameScoreWnd();

	//���غ���
public:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

	//���ܺ���
public:
	//���û���
	void SetScoreInfo(tagGameScoreInfo & GameScoreInfo);

	//��Ϣӳ��
protected:
	//�رհ�ť
	afx_msg void OnClose();
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
