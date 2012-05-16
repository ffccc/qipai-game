#ifndef GAME_FRAME_DLG_HEAD_FILE
#define GAME_FRAME_DLG_HEAD_FILE

#pragma once

#include "GameFrame.h"
#include "GameFrameView.h"

//��˵��
class CGameFrameControl;
class CClientKernelSink;

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class GAME_FRAME_CLASS CGameFrameDlg : public CSkinDialog, public ISplitterSink
{
	//��Ԫ����
	friend class CClientKernelSink;
	friend class CGameFrameControl;

	//����״̬
private:
	bool							m_bRectify;							//������־
	bool							m_bHideList;						//���ر�־
	CRect							m_rcNormalSize;						//����λ��

	//���渨��
private:
	bool							m_bReset;							//�Ƿ�����
	bool							m_bGameSceen;						//��Ϸ����
	bool							m_bInitDialog;						//��ʼ��־

	//���ñ���
private:
	bool							m_bAllowSound;						//��������

	//�ؼ�����
protected:
	CSkinButton						m_btControl;						//���ư�ť
	CSkinSplitter					m_VorSplitter;						//��ֿؼ�
	CGameFrameView					* m_pGameFrameView;					//��ͼָ��
	CGameFrameControl				* m_pGameFrameControl;				//������ͼ

	//�ں˱���
private:
	CClientKernelSink				* m_pKernelSink;					//���ӽӿ�
	CUserFaceResHelper				m_UserFaceResHelper;				//ͷ�����
	CClientKernelHelper				m_ClientKernelHelper;				//��Ϸ�߼�
	CMessageProxyHelper				m_MessageProxyHelper;				//�������

	//��������
public:
	//���캯��
	CGameFrameDlg(CGameFrameView * pGameFrameView);
	//��������
	virtual ~CGameFrameDlg();

	//������ӿ�
public:
	//��ť��Ϣ
	virtual bool __cdecl OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton);
	//�������Ϣ
	virtual bool __cdecl OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos);

	//����̳�
private:
	//��ʼ����
	virtual bool InitGameFrame()=NULL;
	//���ÿ��
	virtual void ResetGameFrame()=NULL;
	//��Ϸ����
	virtual void OnGameOptionSet()=NULL;

	//ϵͳ�¼�
private:
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)=NULL;
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)=NULL;
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)=NULL;
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize)=NULL;

	//����̳�
private:
	//��������
	virtual void RectifyControl(int nWidth, int nHeight);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

	//�û��¼�
private:
	//�û�����
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�����
	virtual void __cdecl OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser);

	//�����¼�
public:
	//��ʼ����
	virtual void __cdecl OnEventInitProperty(tagPropertyInfo *pPropertyInfo, int nInfoCount);
	//��ʼ�ʻ�
	virtual void __cdecl OnEventInitFlower(tagFlowerInfo *pFlowerInfo, int nInfoCount);
	//�ʻ���Ϣ
	virtual void __cdecl OnEventFlower(const tagUserData * pSendUserData, const tagUserData * pRecvUserData, UINT uFlowerID, UINT uFlowerEffectID);

	//���ڿ���
public:
	//�����б�
	bool ShowViewControl(bool bShowControl);

	//���ܺ���
public:
	//������Դ
	bool UpdateSkinResource();
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

	//�Թۿ���
public:
	//�����Թ�
	bool IsAllowUserLookon();
	//�Թۿ���
	bool AllowUserLookon(DWORD dwUserID, bool bAllowLookon);

	//��Ϣ����
public:
	//�Թ�ģʽ
	bool IsLookonMode();
	//�����Թ�
	bool IsAllowLookon();

	//��Ϣ����
public:
	//�Լ�λ��
	WORD GetMeChairID();
	//ʱ��λ��
	WORD GetTimeChairID();
	//������Ϸ
	VOID JoinInGameTable(WORD wChairID);
	//��ȡ�û�
	const tagUserData * GetUserData(WORD wChairID);
	//�ں˽ӿ�
	void * GetClientKernel(const IID & Guid, DWORD dwQueryVer);

	//��Ϸ״̬
public:
	//��Ϸ״̬
	BYTE GetGameStatus();
	//��Ϸ״̬
	void SetGameStatus(BYTE bGameStatus);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//��Ϣӳ��
private:
	//���ư�ť
	VOID OnBnClickedControl();
	//λ�øı�
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//λ����Ϣ
	afx_msg void OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//λ�øı�
	afx_msg void OnWindowPosChanging(WINDOWPOS * lpwndpos);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif