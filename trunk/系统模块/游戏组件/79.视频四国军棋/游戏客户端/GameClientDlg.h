#pragma once

#include "Stdafx.h"
#include "historyrecord.h"
#include "GameLogic.h"
#include "GameClientView.h"


//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ԫ����
	friend class CGameClientView ;

	//�������
protected:

	//���ñ���
protected:

	//��Ϸ����
protected:
	BYTE							m_bChessColor;						//������ɫ
	WORD							m_wCurrentUser;						//��ǰ�û�
	tagChessItem					m_Chess[17][17];					//������Ϣ
	bool							m_bFinishPlacement;					//��ɲ���
	bool							m_bGameSart;						//��Ϸ��ʼ
	BYTE							m_bPlayerCount;						//��Ϸ����
	CHistoryRecord					m_HistoryRecord ;					//��ʷ��¼
	LONG							m_lCurrentStep ;					//Ŀǰ����
	bool							m_bDeasilOrder ;					//����˳��
	LONG							m_lCurrentStepCount;				//��ǰ����

	//��������
protected:
	BYTE							m_bXSourcePos;						//ѡ����Ϣ
	BYTE							m_bYSourcePos;						//ѡ����Ϣ
	BYTE							m_bXTargetPos;						//ѡ����Ϣ
	BYTE							m_bYTargetPos;						//ѡ����Ϣ
	bool							m_bChooseChess;						//ѡ������

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
	//��ɲ���
	bool OnSubFinishiPlacement(const void * pDataBuffer, WORD wDataSize) ;
	//��ʼ����
	bool OnSubPlacement(const void * pDataBuffer, WORD wDataSize) ;
	//�ƶ�����
	bool OnSubMoveChess(const void * pDataBuffer, WORD wDataSize) ;
	//���û��
	bool OnSubLoseOne(const void * pDataBuffer, WORD wDataSize) ;
	//��Ϸ����
	bool OnSubGameEnd(const void * pDataBuffer, WORD wDataSize) ;
	//��Ϸ����
	bool OnSubChessManual(const void * pBuffer, WORD wDataSize) ;

	//��������
protected:
	//��ʼ����
	void InitCurrentUserChess() ;
	//����ת��
	void ServerToClient(const BYTE bChessColor , const BYTE bServerX , const BYTE bServerY , BYTE &bClientX , BYTE &bClientY) ;
	//��ʾ����
	void ShowJunQi(BYTE bChessColor) ;
	//�������
	void ShowSave() ;
	//����ת��
	void ClientToServer(const BYTE bChessColor , const BYTE bClientX , const BYTE bClientY , BYTE &bServerX , BYTE &bServerY) ;

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//���ְ�ť
	LRESULT OnHitPlacement(WPARAM wparam, LPARAM lparam);
	//�������
	LRESULT OnHitChessBoard(WPARAM wParam , LPARAM lParam) ;
	//�ı����
	LRESULT OnChangeCursor(WPARAM wParam , LPARAM lParam) ;
	//������Ϸ
	LRESULT OnHitResetGame(WPARAM wParam , LPARAM lParam) ;	
	//���沼��
    LRESULT OnHitSavePlacement(WPARAM wParam , LPARAM lParam) ;	
	//װ�ز���	
	LRESULT OnHitLoadPlacement(WPARAM wParam , LPARAM lParam) ;	
	//�������
	LRESULT OnHitLoadHistory(WPARAM wParam , LPARAM lParam) ;	
	//��һ����			
	LRESULT OnHitPrevStep(WPARAM wParam , LPARAM lParam) ;	
	//��һ����				
	LRESULT OnHitNextStep(WPARAM wParam , LPARAM lParam) ;	
	//��ת����				
	LRESULT OnHitTurn(WPARAM wParam , LPARAM lParam) ;
	//�������
	LRESULT OnHitLose(WPARAM wparam, LPARAM lparam);
	//��������
	LRESULT OnHitGiveUp(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnHitPeace(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
