#ifndef GAME_FRAME_CONTROL_HEAD_FILE
#define GAME_FRAME_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameFrame.h"
#include "UserInfoView.h"
#include "UserListView.h"
#include "GameFrameDlg.h"

//��˵��
class CClientKernelSink;

//////////////////////////////////////////////////////////////////////////

//��ܿ�����ͼ
class GAME_FRAME_CLASS CGameFrameControl : public CDialog, public IPurchaseInfo
{
	//��������
protected:
	bool								m_bRectify;						//������־
	bool								m_bInitDialog;					//������־
	bool								m_bShowAd;						//����־
	DWORD								m_dwLimitChatTimes;				//������ʱ
	tagUserData *						m_pUserData;					//�û�ָ��

	//�������
protected:
	CGameFrameDlg						* m_pGameFrameDlg;				//�Ի���
	IClientKernel						* m_pIClientKernel;				//�ں˽ӿ�

	//������Ϣ
protected:
	tagEncircleInfo						m_ImageInfoList;				//�б���Ϣ
	tagEncircleInfo						m_ImageInfoChat;				//�Ŀ���Ϣ

	//��Դ����
protected:
	tagEncircleImage					m_EncircleList;					//�б���Դ
	tagEncircleImage					m_EncircleChat;					//�Ŀ���Դ

	//����ؼ�
public:
	CComboBox							m_ChatInput;					//��������
	CComboBox							m_ChatObject;					//�������
	CWebBrowser							m_BrowerAD;						//�������
	//CSkinButton							m_btPhrase;						//���ﰴť
	CSkinButton							m_btColorSet;					//��ɫ����
	CSkinButton							m_btCleanScreen;				//������ť
	CSkinButton							m_btBugle;						//���˰�ť
	CSkinButton							m_btSendChat;					//���Ͱ�ť
	CSkinButton							m_btExpression;					//���鰴ť
	CExpression							m_Expression;					//���鴰��
	CSkinRichEdit						m_ChatMessage;					//�������

	//��������
public:
	CSkinButton							m_btSound;						//��Ϸ����
	CSkinButton							m_btLookOn;						//�Թ�����
	CSkinButton							m_btGameRule;					//��Ϸ����
	CSkinButton							m_btGameOption;					//��Ϸ����
	CSkinButton							m_btGameQuit;					//��Ϸ����

	//�ؼ�����
public:
	CToolTipCtrl						m_ToolTipCtrl;					//��ʾ�ؼ�
	CUserListView						m_UserListView;					//�û��б�
	CUserInfoView						m_UserInfoView;					//�����Ϣ

	//���߿ؼ�
protected:
	CPropertyBar						m_PropertyBar;					//���߿ؼ�

	//��������
public:
	//���캯��
	CGameFrameControl(CGameFrameDlg * pGameFrameDlg); 
	//��������
	virtual ~CGameFrameControl();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���߽ӿ� 
public:
	//������
	virtual BYTE __cdecl GetLocation(){return LOCATION_GAME_ROOM;}
	//��������
	virtual WORD __cdecl GetGameGenre();
	//ö�����
	virtual tagUserData * __cdecl EnumLookonUser(WORD wIndex);
	//�����û�
	virtual tagUserData * __cdecl SearchUserItem(DWORD dwUserID);
	//�����û�
	virtual tagUserData * __cdecl SearchUserItem(LPCTSTR pszUserName){return NULL;}
	//��ȡ���
	virtual const tagUserData * __cdecl GetUserInfo(WORD wChairID);
	//��ȡ�Լ�
	virtual const tagUserData * __cdecl GetMeUserInfo();
	//������Ϣ
	virtual void __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK() { return; };
	//ȡ������
	virtual void OnCancel() { return; };
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�˵�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ú���
public:
	//�����ں�
	void SetClientKernel(IClientKernel * pIClientKernel);
	//���ýӿ�
	bool SetUserFaceRes(IUnknownEx * pIUnknownEx);

	//���ܺ���
public:
	//������Դ
	bool UpdateSkinResource();
	//���ÿ���
	void ResetGameFrameControl();
	//�����û�
	bool InsertUserItem(tagUserData * pUserData);
	//�����û�
	bool UpdateUserItem(tagUserData * pUserData);
	//ɾ���û�
	bool DeleteUserItem(tagUserData * pUserData);

	//�ڲ�����
protected:
	//����ͼ��
	void DrawLeftViewFrame(CDC * pDC);
	//�����ؼ�
	void RectifyControl(int nWidth, int nHeight);

	//��Ϣ����
protected:
	//�滭��Ϣ
	afx_msg void OnPaint();
	//��Ϸ����
	afx_msg void OnBnClickedRule();
	//��Ϸ�˳�
	afx_msg void OnBnClickedQuit();
	//��Ϸ����
	afx_msg void OnBnClickedSound();
	//��Ϸ�Թ�
	afx_msg void OnBnClickedLookon();
	//��Ϸ����
	afx_msg void OnBnClickedOption();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//���Ͱ�ť
	afx_msg void OnBnClickedSendChat();
	//���鰴ť
	afx_msg void OnBnClickedExpression();
	//��ɫ��ť
	afx_msg void OnBnClickedColorSet();
	//������ť
	afx_msg void OnBnClickedCleanScreen();
	//���Ȱ�ť
	afx_msg void OnBnClickedBugle();
	//������Ϣ
	LRESULT OnHitExpression(WPARAM wParam, LPARAM lParam);
	//�Ҽ��б�
	afx_msg void OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//�����б�
	afx_msg void OnNMclickUserList(NMHDR *pNMHDR, LRESULT *pResult);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif