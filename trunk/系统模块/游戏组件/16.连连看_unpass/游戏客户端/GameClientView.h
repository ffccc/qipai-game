#ifndef GAME_VIEW_HEAD_FILE
#define GAME_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GlobalOption.h"
#include "MyListView.h"
#include "D3DWnd.h"
#include "GameOption.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;
	friend class CGameThreadDraw;

	//�������
protected:
	CD3DWnd			m_wndD3D;								//direct9.0����
	CDC				m_MemDC;								//����ͼƬ����


	HCURSOR m_hButtonCursor;

public:
	//user info myself
	tagUserData *    m_UserID[4];

	//�ؼ�����
public:
	CMyListView     m_MyListView;							//�ؼ���ͼ
	CLogic			m_Logic;							   //��Ϸ�߼���
	CGameOption		m_GameOption;

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);
	//��Ϣӳ��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()


public:

	// ������Ϣ����
	LRESULT OnLink(WPARAM wParam, LPARAM lParam);
	// ���Ӵ���
	void LinkPro(CPoint first, CPoint end, WORD userid);
	// ����������Ӵ���
	void OtherPlayLinkPro(CPoint first, CPoint end, WORD userid);

	//��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);						// ��ʼ
	LRESULT OnExecise(WPARAM wParam, LPARAM lParam);					// ��ϰ
	LRESULT OnQuit(WPARAM wParam, LPARAM lParam);						// ����
	//������Ϣ
	LRESULT OnGuide(WPARAM wParam, LPARAM lParam);						// ָ����
	LRESULT OnRealign(WPARAM wParam, LPARAM lParam);					// ����
	LRESULT OnBomb(WPARAM wParam, LPARAM lParam);						// ը��
	LRESULT OnStopIt(WPARAM wParam, LPARAM lParam);						// ����
	LRESULT OnClog(WPARAM wParam, LPARAM lParam);						// �ϰ�

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnTimer(UINT nIDEvent);

	//
	LRESULT OnChatColor(WPARAM wParam, LPARAM lParam);
	
	// �����û���Ϣ
	void MySetUserInfo(WORD wChairID, tagUserData * pUserData);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
#endif
//////////////////////////////////////////////////////////////////////////
