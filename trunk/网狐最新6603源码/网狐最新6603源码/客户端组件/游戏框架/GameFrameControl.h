#ifndef GAME_FRAME_CONTROL_HEAD_FILE
#define GAME_FRAME_CONTROL_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameUserView.h"
#include "GameFrameMedalView.h"

//////////////////////////////////////////////////////////////////////////////////

//�û�ѡ��
#define MAX_HISTOY_USER				8									//�����Ŀ

//�û�����
typedef CWHArray<IClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////////////

//���ƿ��
class GAME_FRAME_CLASS CGameFrameControl : public CDialog, public IGameFrameControl, public IUserEventSink,
	public IChatControlSink, public IExpressionSink, public IGamePropertySink
{
	//�������
protected:
	bool							m_bCreateFlag;						//������־

	//�������
protected:
	DWORD							m_dwChatTime;						//����ʱ��
	CString							m_strChatString;					//������Ϣ

	//�������
protected:
	CClientUserItemArray			m_MenuUserItemArray;				//�û�����

	//�ؼ�����
protected:
	CSkinEditEx						m_ChatEdit;							//��������
	CSkinButton						m_btSendChat;						//��������
	CSkinButton						m_btChatShort;						//�������
	CRichEditMessage				m_ChatMessage;						//������Ϣ

	//���찴ť
protected:
	CSkinButton						m_btChatButton1;					//���찴ť
	CSkinButton						m_btChatButton2;					//���찴ť
	CSkinButton						m_btChatButton3;					//���찴ť
	CSkinButton						m_btChatButton4;					//���찴ť

	//�ؼ�����
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CWndChatControl					m_ChatControl;						//�������
	CUserListControl				m_UserListControl;					//�û��б�
	CGamePropertyCtrl				m_GamePropertyCtrl;					//��Ϸ����
	CGameFrameUserView				m_GameFrameUserView;				//�û���Ϣ
	CGameFrameMedalView				m_GameFrameMedalView;				//������Ϣ

	//�ؼ�����
protected:
	CExpressionControl *			m_pExpressionControl;				//���鴰��

	//�ӿڱ���
protected:
	IClientKernel *					m_pIClientKernel;					//�ں˽ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��

	//��Դ����
protected:
	CEncircleBMP					m_ChatEncircle;						//������Դ
	CEncircleBMP					m_UserListEncircle;					//�û���Դ

	//��������
public:
	//���캯��
	CGameFrameControl();
	//��������
	virtual ~CGameFrameControl();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//���ú���
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�ӿں���
protected:
	//���ÿؼ�
	virtual VOID ResetControl();
	//���ù��
	virtual VOID SetGameBillInfo(WORD wKindID, WORD wServerID);
	//�����û�
	virtual VOID SetUserViewInfo(IClientUserItem * pIClientUserItem);
	//�б�����
	virtual VOID SetColumnDescribe(tagColumnItem ColumnItem[], BYTE cbColumnCount);
	//��������
	virtual VOID SetPropertyDescribe(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount);

	//���߽ӿ�
protected:
	//�������
	virtual bool OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);

	//�û��˵�
protected:
	//��ʾ�˵�
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem);
	//��ʾ�˵�
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wItemCount);

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�����
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�״̬
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�ͷ��
	virtual VOID OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//�ؼ��¼�
protected:
	//�û�ѡ��
	virtual VOID OnChangeChatTarget(IClientUserItem * pIClientUserItem);
	//�����¼�
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//��������
	VOID RectifyControl(CSkinRenderManager * pSkinRenderManager);

	//�ڲ�����
protected:
	//����Ч��
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);

	//��������
protected:
	//������ɫ
	VOID OnSelectMoreColor();
	//ѡ����ɫ
	VOID OnSelectChatColor(UINT nCommandID);
	//ѡ�����
	VOID OnSelectChatShort(UINT nCommandID);

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//�Ҽ��б�
	VOID OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//ѡ��ı�
	VOID OnLvnItemchangedUserList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif