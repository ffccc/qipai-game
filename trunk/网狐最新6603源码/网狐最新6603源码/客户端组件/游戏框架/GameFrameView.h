#ifndef GAME_FRAME_VIEW_HEAD_FILE
#define GAME_FRAME_VIEW_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class GAME_FRAME_CLASS CGameFrameView : public CWnd, public IGameFrameView
{
	//λ�ñ���
protected:
	CPoint							m_ptClock[MAX_CHAIR];				//ʱ��λ��
	CPoint							m_ptReady[MAX_CHAIR];				//׼��λ��
	CPoint							m_ptAvatar[MAX_CHAIR];				//ͷ��λ��
	CPoint							m_ptNickName[MAX_CHAIR];			//�ǳ�λ��

	//�û�����
protected:
	WORD							m_wUserClock[MAX_CHAIR];			//�û�ʱ��
	IClientUserItem *				m_pIClientUserItem[MAX_CHAIR];		//�û��ӿ�

	//����ӿ�
protected:
	IStringMessage *				m_pIStringMessage;					//��Ϣ�ӿ�
	IGameLevelParser *				m_pIGameLevelParser;				//�ȼ�����

	//��̬����
protected:
	static CGameFrameView *			m_pGameFrameView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CGameFrameView();
	//��������
	virtual ~CGameFrameView();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ڽӿ�
public:
	//���ھ��
	virtual HWND GetGameViewHwnd();
	//��������
	virtual bool CreateGameViewWnd(CWnd * pParentWnd, UINT nID);

	//���ƽӿ�
public:
	//�л�״̬
	virtual VOID SwitchToReadyStatus();

	//ʱ�ӹ���
public:
	//��ȡʱ��
	virtual WORD GetUserClock(WORD wChairID);
	//����ʱ��
	virtual VOID SetUserClock(WORD wChairID, WORD wUserClock);

	//�û�����
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wChairID);
	//�����û�
	virtual VOID SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem);

	//���غ���
public:
	//���ý���
	virtual VOID ResetGameView()=NULL;
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight)=NULL;
	//�������
	virtual VOID InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight)=NULL;

	//��������
public:
	//��������
	bool PlayGameSound(LPCTSTR pszSoundName);
	//��������
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//��Ϣ����
public:
	//��ȡ�ȼ�
	LPCTSTR GetLevelDescribe(IClientUserItem * pIClientUserItem);

	//������Ϣ
public:
	//������Ϣ
	LRESULT SendEngineMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);
	//Ͷ����Ϣ
	LRESULT PostEngineMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);

	//��Ϣӳ��
protected:
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnRButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

	//���ܺ���
public:
	//��ȡʵ��
	static CGameFrameView * GetInstance() { return m_pGameFrameView; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif