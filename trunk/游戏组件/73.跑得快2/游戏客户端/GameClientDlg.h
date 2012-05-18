#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "..\��Ϣ����\CMD_QuickRun.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//�ؼ�����
protected:
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:
	//��ʼ����
	virtual bool InitGameFrame();
	//���ÿ��
	virtual void ResetGameFrame();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnCmdOutPisa(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCmdUserReady(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCmdTimerOut(WPARAM wParam,LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////
