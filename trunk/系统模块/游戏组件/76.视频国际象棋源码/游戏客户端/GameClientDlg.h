#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	WORD							m_wBlackUser;						//�������
	BYTE							m_cbChessColor;						//������ɫ
	WORD							m_wCurrentUser;						//��ǰ�û�
	WORD							m_wLeftClock[2];					//ʣ��ʱ��

	//��������
protected:
	bool							m_ShamView;							//Ԥ����ʾ
	BYTE							m_cbXSourcePos;						//ѡ����Ϣ
	BYTE							m_cbYSourcePos;						//ѡ����Ϣ
	BYTE							m_cbXTargetPos;						//ѡ����Ϣ
	BYTE							m_cbYTargetPos;						//ѡ����Ϣ

	//���ñ���
protected:
	bool							m_bShowChessName;					//��ʾ����
	bool							m_bAutoSaveManual;					//�Զ�����

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
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

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�ƶ�����
	bool OnSubMoveChess(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubRegretReq(const void * pBuffer, WORD wDataSize);
	//����ʧ��
	bool OnSubRegretFaile(const void * pBuffer, WORD wDataSize);
	//������
	bool OnSubRegretResult(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubPeaceReq(const void * pBuffer, WORD wDataSize);
	//����Ӧ��
	bool OnSubPeaceAnser(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubChessManual(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//ȡ��ѡ��
	void CancelChessSelect();
	//ִ������
	void PerformMoveChess(BYTE cbSwitchChess);
	//��������
	bool SaveChessManual(LPCTSTR pszFileName);

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��ť
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//���尴ť
	LRESULT OnRegret(WPARAM wParam, LPARAM lParam);
	//��Ͱ�ť
	LRESULT OnPeace(WPARAM wParam, LPARAM lParam);
	//���䰴ť
	LRESULT OnGiveUp(WPARAM wParam, LPARAM lParam);
	//���水ť
	LRESULT OnPreserve(WPARAM wParam, LPARAM lParam);
	//�о���ť
	LRESULT OnStudy(WPARAM wParam, LPARAM lParam);
	//����Ӧ��
	LRESULT OnPeaceAnswer(WPARAM wParam, LPARAM lParam);
	//����Ӧ��
	LRESULT OnRegretAnswer(WPARAM wParam, LPARAM lParam);
	//ѡ������
	LRESULT OnSelectChess(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnHitChessBorad(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
