#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "GameLogic.h"
#include "..\��Ϣ����\CMD_Mj.h"
#include "TranscendLog.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��־
	TranscendLog log;
	//��Ϸ����
protected:
	BYTE							m_byOutMj;							//�������
	BYTE							m_byHandMjCount;					//�����齫��
	BYTE							m_byHandMjs[14];					//�����齫
	BYTE							m_byMjCount[4];						//�齫��

	int								m_nChiMode;							//����ģʽ
	int								m_nGangMode;						//����ģʽ
//	MJ_HU							m_hu[4];
	
	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
public:
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	
	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��
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
protected:
	void PlayPaiSound(BYTE nPai,BYTE sex);

	//��Ϣ����
protected:
	//�����齫
	bool OnSubSendMj( const void * pBuffer, WORD wDataSize );
	//��Ϸ��ʼ
	bool OnSubGameStart( const void * pBuffer, WORD wDataSize );
	//��ʾ��Ť
	bool OnSubGameButton( const void * pBuffer, WORD wDataSize );
	//�û�����
	bool OnSubOutMj( const void * pBuffer, WORD wDataSize );
	//�Զ�����
	bool OnSubTouchMj( const void * pBuffer, WORD wDataSize );
	//�û�����
	bool OnSubPengMj( const void *pBuffer, WORD wDataSize );
	//�û�����
	bool OnSubChiMj( const void *pBuffer, WORD wDataSize );
	//�û�����
	bool OnSubGangMj( const void *pBuffer, WORD wDataSize );
	//��Ϸ����
	bool OnSubGameEnd( const void *pBuffer, WORD wDataSize );
	//����ѡ��ģʽ
	bool OnSubSelectGang( const void *pBuffer, WORD wDataSize );
	//����
	bool OnSubEndEmpty( const void *pBuffer, WORD wDataSize);

	//��������
protected:
	//�û�λ����
	WORD TransViewID( WORD wChairID );
	//����ׯλ
	void SetBanker( WORD wBanker );

	//��Ϣӳ��
public:
	//��ʼ��Ϣ
	LRESULT OnStart( WPARAM wParam, LPARAM lParam );
	//������Ϣ
	LRESULT OnOutMj( WPARAM wParam, LPARAM lParam );
	//������Ϣ
	LRESULT OnHuMj( WPARAM wParam, LPARAM lParam );
	//������Ϣ
	LRESULT OnTingMj( WPARAM wParam, LPARAM lParam );
	//������Ϣ
	LRESULT OnGangMj( WPARAM wParam, LPARAM lParam );
	//������Ϣ
	LRESULT OnPengMj( WPARAM wParam, LPARAM lParam );
	//������Ϣ
	LRESULT OnChiMj( WPARAM wParam, LPARAM lParam );
	//������Ϣ
	LRESULT OnGiveUp( WPARAM wParam, LPARAM lParam );

	//�ر���Ϣ
	LRESULT OnGameSet( WPARAM wParam, LPARAM lParam );
	//��С��
	LRESULT OnShowMini( WPARAM wParam, LPARAM lParam );

	//ѡ�����
	LRESULT OnSelectChi( WPARAM wParam, LPARAM lParam );
	//ѡ�����
	LRESULT OnSelectGang( WPARAM wParam, LPARAM lParam );
	//�Ҽ���Ϣ
	LRESULT OnRightHitMj( WPARAM wParam, LPARAM lParam );
	//�����Ϣ
	LRESULT OnLeftHitMj( WPARAM wParam, LPARAM lParam );

	//��ʱ��
	LRESULT OnCmdTimerOut( WPARAM wParam, LPARAM lParam );

	//��ʱ����Ϣ
	//afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
