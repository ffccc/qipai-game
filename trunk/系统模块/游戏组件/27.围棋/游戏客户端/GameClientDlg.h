#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//�������
protected:
	tagGameRuleInfo					m_GameRuleInfo;						//��Ϸ����

	//���ñ���
protected:
	bool							m_bDoubleMouse;						//˫������
	bool							m_bShowChessID;						//��ʾ����
	bool							m_bAutoSaveManual;					//�Զ�����

	//��Ϸ����
protected:
	bool							m_ShamView;							//Ԥ����ʾ
	bool							m_bAgreeRule;						//ͬ���־
	bool							m_bCustomize;						//���Ʊ�־
	bool							m_bGameStart;						//��Ϸ��־
	bool							m_bLimitTime;						//�޶�ʱ��
	bool							m_bReqChessCount;					//��Ŀ����
	bool							m_bResponsionChessCount;			//��Ӧ��Ŀ

	//��Ϸ����
protected:
	WORD							m_wBlackUser;						//�������
	WORD							m_wCurrentUser;						//��ǰ�û�
	tagStatusInfo					m_StatusInfo[2];					//��Ϸ��Ϣ

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
	//�û�����
	bool OnSubPlaceChess(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubPassChess(const void * pBuffer, WORD wDataSize);
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
	bool OnSubSetRule(const void * pBuffer, WORD wDataSize);
	//����ͬ��
	bool OnSubAgreeRule(const void * pBuffer, WORD wDataSize);
	//�����Ŀ
	bool OnSubReqChessCount(const void * pBuffer, WORD wDataSize);
	//��ĿӦ��
	bool OnSubChessCountResponsion(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//��������
	bool SaveChessManual(LPCTSTR pszFileName);

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��ť
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//��Ŀ��ť
	LRESULT OnCount(WPARAM wParam, LPARAM lParam);
	//ͨ����ť
	LRESULT OnPass(WPARAM wParam, LPARAM lParam);
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
	//�������
	LRESULT OnHitChessBorad(WPARAM wParam, LPARAM lParam);
	//ͬ�����
	LRESULT OnAgreeGameRule(WPARAM wParam, LPARAM lParam);
	//��ĿӦ��
	LRESULT OnChessCountResponsion(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
