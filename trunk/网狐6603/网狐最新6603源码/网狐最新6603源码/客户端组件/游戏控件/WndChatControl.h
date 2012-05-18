#ifndef WMD_CHAT_CONTROL_HEAD_FILE
#define WMD_CHAT_CONTROL_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////

//�û�����
typedef CWHArray<IClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////

//�¼��ӿ�
interface IChatControlSink
{
	//�û�ѡ��
	virtual VOID OnChangeChatTarget(IClientUserItem * pIClientUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//�������
class SHARE_CONTROL_CLASS CWndChatControl : public CWnd
{
	//�ؼ�����
protected:
	CSkinButton						m_btClose;							//�رհ�ť
	CSkinButton						m_btControl;						//���ư�ť

	//��������
protected:
	IClientUserItem *				m_pIClientUserItem;					//Ŀ���û�
	IChatControlSink *				m_pIChatControlSink;				//�¼��ӿ�
	CClientUserItemArray			m_ChatUserItemArray;				//�û�����

	//��������
public:
	//���캯��
	CWndChatControl();
	//��������
	virtual ~CWndChatControl();

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//Ŀ���û�
public:
	//��ȡ�û�
	DWORD GetChatTargetUserID();
	//��ȡ�û�
	IClientUserItem * GetChatTargetUserItem();

	//���ܺ���
public:
	//ɾ���û�
	VOID DeleteUserItem(IClientUserItem * pIClientUserItem);
	//�����û�
	VOID SetChatTargetUser(IClientUserItem * pIClientUserItem);
	//���ýӿ�
	VOID SetChatControlSink(IChatControlSink * pIChatControlSink);

	//��ť��Ϣ
protected:
	//�رհ�ť
	VOID OnBnClickedClose();
	//���ư�ť
	VOID OnBnClickedControl();

	//��Ϣӳ��
protected:
	//�滭����
	VOID OnPaint();
	//������Ϣ
	VOID OnDestroy();
	//λ�ñ仯
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif