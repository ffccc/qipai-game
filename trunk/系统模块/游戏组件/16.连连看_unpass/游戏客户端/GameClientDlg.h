#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "midi.h"
#include "DirectSound.h"


//////////////////////////////////////////////////////////////////////////


//��˵��
class CGameFrameControl;

//��Ϸ�Ի���
class CGameClientDlg : public CSkinDialog,public IClientKernelSink
{
private:
	//��Ԫ����
	friend class CGameFrameControl;

	//��Ļ�����С
	CRect m_FullScreenRect; 

	//
	
	//���渨��
private:
	bool								m_bReset;						//�Ƿ�����
	bool								m_bRectify;						//������־
	bool								m_bInitDialog;					//��ʼ��־

	//���ñ���
private:
	bool								m_bAllowSound;					//��������


	//�ں˱���
private:
	IClientKernelSink					* m_pKernelSink;				//���ӽӿ�
	CUserFaceResHelper					m_UserFaceResHelper;			//ͷ�����
	CClientKernelHelper					m_ClientKernelHelper;			//��Ϸ�߼�

	//�ؼ�����
protected:
	CGameClientView						m_GameClientView;				//��Ϸ��ͼ

	CMIDI								m_midi;							//��������
	CDirectSound						m_Bomb;
	CDirectSound						m_Link;
	CDirectSound						m_FlyStar;
	CDirectSound						m_Start;
	CDirectSound						m_Prop;
	CDirectSound						m_Warn;
	CDirectSound						m_Select;
	CDirectSound						m_End;
	
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

public:
	//�Ƿ���Ч
	virtual bool __cdecl IsValid();
	//�ͷŶ���
	virtual bool __cdecl Release();
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����̳�
	//��ʼ����
	virtual bool InitGameFrame();
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

	//�����ӿ�
	//��ȡ����
	virtual HWND __cdecl GetFrameWnd();
	//����״̬
	virtual void __cdecl ResetGameFrame();
	//�رմ���
	virtual void __cdecl CloseGameFrame(bool bForceClose);

	//�ں��¼�
	//ʱ����Ϣ
	virtual bool __cdecl OnEventTimer(WORD wChairID, UINT nElapse, UINT nTimerID);
	//�����Ϣ
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool __cdecl OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool __cdecl OnEventProcess(const IPC_Head & Head, const void * pBuffer, WORD wDataSize, HWND hWndSend);

	//��Ϸ�¼�
	//��Ϸ״̬
	virtual bool __cdecl OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool __cdecl OnUserChatMessage(tagUserData * pSendUserData, tagUserData * pRecvUserData, LPCTSTR pszChatMessage, COLORREF crColor);
	//�û�����
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�����
	virtual void __cdecl OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//ʱ�����
	virtual void __cdecl OnEventTimerKilled(WORD wChairID, UINT nTimerID);
	//�Թ�״̬
	virtual void __cdecl OnEventLookonChanged(bool bLookonUser, void * pBuffer, WORD wDataSize);

	//��ť��Ϣ
	virtual bool __cdecl OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton);
	//�������Ϣ
	virtual bool __cdecl OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos);
	
	
	//��������
	void RectifyControl(int nWidth, int nHeight);
	
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);

public:

	afx_msg void OnTimer(UINT nIDEvent);

DECLARE_MESSAGE_MAP()
virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
protected:
	virtual void OnCancel();

	//����ѡ��ť����
	LRESULT OnPlayMusic(WPARAM wParam, LPARAM lParam);


public:
	//���ͺ���
	bool SendData(WORD wSubCmdID);
	//���ͺ���
	bool SendData(WORD wSubCmdID, void * pData, WORD wDataSize);
	//����׼��
	bool SendUserReady(void * pBuffer, WORD wDataSize);
	//���ö�ʱ��
	bool SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse);
	//ɾ����ʱ��
	bool KillGameTimer(UINT nTimerID);
	//��������
	bool PlayGameSound(LPCTSTR pszSoundName);
	//��������
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//��Ϣ����
public:
	//����ͼƬ
	bool InsertImage(CBitmap * pBitmap);
	//��������
	bool InsertUserName(LPCTSTR pszUserName);
	//ϵͳ��Ϣ
	bool InsertSystemString(LPCTSTR pszString);
	//��ɫ��Ϣ
	bool InsertShtickString(LPCTSTR pszString, COLORREF crColor, bool bNewLine);
	//��ͨ��Ϣ
	bool InsertGeneralString(LPCTSTR pszString, COLORREF crColor, bool bNewLine);

	//��������
public:
	//�������
	void ActiveGameFrame();
	//�л�����
	WORD SwitchViewChairID(WORD wChairID);

	//��������
public:
	//�Ƿ�����
	bool IsEnableSound();
	//��������
	void EnableSound(bool bEnable);

	//��Ϣ����
public:
	//�Թ�ģʽ
	bool IsLookonMode();
	//�����Թ�
	bool IsAllowLookon();
	//�Լ�λ��
	WORD GetMeChairID();
	//ʱ��λ��
	WORD GetTimeChairID();
	//��ȡ�û�
	const tagUserData * GetUserData(WORD wChairID);
	//�ں˽ӿ�
	void * GetClientKernel(const IID & Guid, DWORD dwQueryVer);

	//��Ϸ״̬
	BYTE GetGameStatus();
	//��Ϸ״̬
	void SetGameStatus(BYTE bGameStatus);

public:
	///////////////////////////////////////////////////////////////
	//��Ϣ����
	//��Ϸ��ʼ
	bool OnSubStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubEnd(const void * pBuffer, WORD wDataSize);
	//��Ϸʧ��
	bool OnSubLost(const void * pBuffer, WORD wDataSize);
	//��Ϸ���
	bool OnSubGroup(const void * pBuffer, WORD wDataSize);
	//���Ӵ���
	bool OnSubLink(const void * pBuffer, WORD wDataSize);
	//���д���
	bool OnSubRealign(const void * pBuffer, WORD wDataSize);
	//���ִ���
//	bool OnSubStopIt(const void * pBuffer, WORD wDataSize);
	//�ϰ�����
//	bool OnSubClog(const void * pBuffer, WORD wDataSize);

	///////////////////////////////////////////////////////////////
	//���ܺ���
	// ��������λ��ת��
	int ChairSwitch(int chair);

protected:
	//���Ϳ�ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//����������Ϣ
	LRESULT OnLink(WPARAM wParam, LPARAM lParam);
	//����ָ������Ϣ
	LRESULT OnGuide(WPARAM wParam, LPARAM lParam);
	//����������Ϣ
	LRESULT OnRealign(WPARAM wParam, LPARAM lParam);
	//����ը����Ϣ
	LRESULT OnBomb(WPARAM wParam, LPARAM lParam);
	//���Ͷ�ʱ����Ϣ
	LRESULT OnTimerStep(WPARAM wParam, LPARAM lParam);
	//���ͽ�����Ϣ
	//LRESULT OnStopIt(WPARAM wParam, LPARAM lParam);
	//�����ϰ���Ϣ
	//LRESULT OnClog(WPARAM wParam, LPARAM lParam);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void OnOK();
	LRESULT	OnQuit(WPARAM wParam, LPARAM lParam);
public:
//	LRESULT OnMusicNotify(WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////
