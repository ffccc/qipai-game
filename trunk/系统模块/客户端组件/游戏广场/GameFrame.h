#ifndef PLAZA_GAME_FRAME_HEAD_FILE
#define PLAZA_GAME_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "RoomViewItem.h"
#include "PlazaViewItem.h"
#include "UserInfoView.h"
#include "ServerItemView.h"
#include "UserCompanionList.h"
#include "DlgCustomFace.h"

/////////////////////////////////////////////////////////////////////////////

//�б�״̬
enum enServerListControl
{
	ServerListControl_Hide,			//�����б�
	ServerListControl_Show,			//��ʾ�б�
	ServerListControl_Turn,			//��ת�б�
};

//�궨��
#define MAX_SERVER						3								//��󷿼�

//�ȼ�����
#define IDI_HOT_KEY_BOSS			0x0100								//�ϰ��ȼ�

/////////////////////////////////////////////////////////////////////////////

//�������
class CGameFrame : public CFrameWnd, public ISplitterSink, public ITreeCtrlSink
{
	//״̬����
public:
	bool								m_bRectify;						//������־
	bool								m_bHideGameList;				//���ر�־
	WORD								m_wShowListType;				//�б��־
	CRect								m_rcViewItem;					//����λ��
	CPlazaViewItem						m_DlgGamePlaza;					//��Ϸ�㳡
	IViewItem							* m_pActiveViewItem;			//��ǰ��ͼ
	CRoomViewItem						* m_pRoomViewItemCreate;		//��������
	CRoomViewItem						* m_pRoomViewItem[MAX_SERVER];	//����ӿ�
	CDlgCustomFace						m_DlgCustomFace;				//�Զ�ͷ��

	//�ؼ�����
public:
	CWebBrowser							m_BrowerAD;						//�������
	CToolTipCtrl						m_ToolTipCtrl;					//��ʾ�ؼ�
	CUserInfoView						m_UserInfoView;					//��Ϸ��ͼ
	CSkinSplitter						m_Splitter;						//��ִ���
	CServerItemView						m_ServerItemView;				//��������ͼ
	CUserCompanionList					m_UserCompanionList;			//�����б�

	//����״̬
protected:
	bool								m_bMaxShow;						//����־
	CRect								m_rcNormalSize;					//����λ��

	//������ť
public:
	CSkinButton							m_btButton1;					//���ܰ�ť
	CSkinButton							m_btButton2;					//���ܰ�ť
	CSkinButton							m_btButton3;					//���ܰ�ť
	CSkinButton							m_btButton4;					//���ܰ�ť
	CSkinButton							m_btButton5;					//���ܰ�ť

	//������ť
public:
	CSkinButton							m_btMin;						//��С��ť
	CSkinButton							m_btMax;						//���ť
	CSkinButton							m_btClose;						//�رհ�ť
	CSkinButton							m_btExChangeSkin;				//������ť
	CSkinButton							m_btGamePlaza;					//��Ϸ�㳡
	CSkinButton							m_btViewItem[MAX_SERVER];		//���䰴ť
	//CSkinButton						m_btQuitGame;					//�˳���Ϸ
	CSkinButton							m_btSelfOption;					//��������
	CSkinButton							m_btSwitchUser;					//�л��ʺ�
	CSkinButton							m_btGlobalOption;				//ϵͳ����

	//�б�ť
public:
	CSkinButton							m_btListControl1;				//���ư�ť
	CSkinButton							m_btListControl2;				//���ư�ť

	//������Ϣ
protected:
	tagEncircleInfo						m_ImageInfoFrame;				//�����Ϣ
	tagEncircleInfo						m_ImageInfoServer;				//�б���Ϣ

	//��Դ����
protected:
	tagEncircleImage					m_EncircleFrame;				//�����Դ
	tagEncircleImage					m_EncircleServer;				//�б���Դ

	//��������
public:
	//���캯��
	CGameFrame();
	//��������
	virtual ~CGameFrame();

	//������ӿ�
public:
	//��ť��Ϣ
	virtual bool __cdecl OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton);
	//�������Ϣ
	virtual bool __cdecl OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos);

	//����ӿ�
public:
	//�������
	virtual void __cdecl OnTreeLeftClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//�Ҽ�����
	virtual void __cdecl OnTreeRightClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//���˫��
	virtual void __cdecl OnTreeLeftDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//�Ҽ�˫��
	virtual void __cdecl OnTreeRightDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//ѡ��ı�
	virtual void __cdecl OnTreeSelectChanged(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//����չ��
	virtual void __cdecl OnTreeItemexpanded(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��ȡ�㳡
	CPlazaViewItem * GetPlazaViewItem() { return &m_DlgGamePlaza; }
	//��ʾ��ҳ
	void WebBrowse(LPCTSTR lpszUrl, bool bForceBrowse);
	//ϵͳ����
	void ShowSystemOption();
	//��ʾ��Ϣ
    int ShowMessageBox(LPCTSTR pszMessage, UINT nType);
	//�б����
	bool ControlServerList(enServerListControl ServerListControl);
	//��Դ���
	HINSTANCE GetResInstanceHandle();
	//��ȡ��Դ
	tagPlatformFrameImage GetPlatformRes();
	//������Դ
	bool UpdateSkinResource();

	//���亯��
public:
	//����㳡
	void ActivePlazaViewItem();
	//�����
	void ActiveRoomViewItem(CRoomViewItem * pRoomViewItem);
	//���ҷ���
	CRoomViewItem * SearchRoomViewItem(WORD wKindID, WORD wServerID);
	//���뷿��
	CRoomViewItem * CreateRoomViewItem(CListServer * pListServer);
	//�رշ���
	void CloseRoomViewItem(IRoomViewItem * pIRoomViewItem);
	//�رշ���
	void CloseAllRoomViewItem();
	//�رյ�ǰ
	void CloseCurrentViewItem();

	//�ڲ�����
private:
	//���¿���
	void UpdateControlButton();
	//�����ؼ�
	void RectifyControl(int nWidth, int nHeight);

	//���溯��
protected:
	//��󴰿�
	bool MaxSizeWindow();
	//��ԭ����
	bool RestoreWindow();

	//��Ϣ����
protected:
	//�滭��Ϣ
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��С��Ϣ
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR * lpMMI);
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��������
	afx_msg LRESULT OnNcHitTest(CPoint Point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//�����¼�
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//�ı���Ϣ
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//�ر���Ϣ
	afx_msg void OnClose();

	//�Զ�����Ϣ
protected:
	//�ȼ���Ϣ
	LRESULT OnHotKeyMessage(WPARAM wParam, LPARAM lParam);
	//��װ���
	LRESULT OnMessageSetupFinish(WPARAM wParam, LPARAM lParam);
	//�رշ���
	LRESULT OnCloseRoomViewItem(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#endif
