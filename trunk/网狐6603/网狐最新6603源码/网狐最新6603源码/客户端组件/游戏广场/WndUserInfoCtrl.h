#ifndef WND_USER_INFO_CTRL_HEAD_FILE
#define WND_USER_INFO_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//�༭�ؼ�
class CEditUnderWrite : public CSkinEditEx, public CMissionItem
{
	//��Ԫ����
	friend class CWndUserInfoCtrl;

	//��������
protected:
	bool							m_bNeedSend;						//�����־
	TCHAR							m_szUnderWrite[LEN_UNDER_WRITE];	//����ǩ��

	//��������
protected:
	CWnd *							m_pParentWnd;						//���ڶ���
	CMissionManager					m_MissionManager;					//�������

	//��������
public:
	//���캯��
	CEditUnderWrite();
	//��������
	virtual ~CEditUnderWrite();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
public:
	//�޸��ж�
	bool IsModifyStatus();
	//���ô���
	VOID SetParentWindow(CWnd * pParentWnd);

	//�ڲ�����
protected:
	//�رտؼ�
	VOID CloseUnderWrite();
	//����ǩ��
	VOID UpdateUnderWrite();

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnDestroy();
	//ʧȥ����
	VOID OnKillFocus(CWnd * pNewWnd);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class CWndUserInfoCtrl : public CWnd
{
	//״̬����
protected:
	bool							m_bClickFace;						//�����־
	bool							m_bMouseEvent;						//ע���־

	//������־
protected:
	bool							m_bHoverFace;						//������־
	bool							m_bHoverUnderWrite;					//������־

	//����λ��
protected:
	CRect							m_rcFaceArea;						//ͷ������
	CRect							m_rcUnderWrite;						//ǩ������

	//�ؼ�����
public:
	CSkinButton						m_btUserInfo1;						//�û���ť
	CSkinButton						m_btUserInfo2;						//�û���ť
	CSkinButton						m_btUserInfo3;						//�û���ť
	CSkinButton						m_btUserInfoSet;					//�û���ť
	CEditUnderWrite					m_EditUnderWrite;					//�༭�ؼ�

	//�������
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�

	//��������
public:
	//���캯��
	CWndUserInfoCtrl();
	//��������
	virtual ~CWndUserInfoCtrl();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//�Զ���Ϣ
protected:
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif