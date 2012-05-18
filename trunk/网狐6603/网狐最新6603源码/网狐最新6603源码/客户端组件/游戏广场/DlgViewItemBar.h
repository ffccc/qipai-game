#ifndef WND_VIEW_ITEM_CTRL_HEAD_FILE
#define WND_VIEW_ITEM_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagViewItemInfo
{
	CWnd *							pItemView;							//������ͼ
	TCHAR							szItemTitle[32];					//�����ַ�
};

//�����¼�
interface IViewItemEvent
{
	//�����¼�
	virtual bool OnViewItemActive(WORD wItemIndex, CWnd * pItemView)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�������
class CWndViewItemCtrl : public CWnd
{
	//�����Ϣ
protected:
	bool							m_bHovering;						//������־
	WORD							m_wItemDown;						//��������
	WORD							m_wItemHover;						//��������
	WORD							m_wItemActive;						//��������

	//������Ϣ
protected:
	CSize							m_SizeButton;						//��ť��С
	CRect							m_RectViewItem;						//����λ��
	IViewItemEvent *				m_pIViewItemEvent;					//�¼��ӿ�

	//������Ϣ
protected:
	CWHArray<tagViewItemInfo *>		m_ViewItemArray;					//��������

	//��������
public:
	//���캯��
	CWndViewItemCtrl();
	//��������
	virtual ~CWndViewItemCtrl();

	//���ܺ���
public:
	//��ȡ��С
	VOID GetViewItemSize(CSize & ResultSize);
	//���ýӿ�
	VOID SetViewItemEvent(IViewItemEvent * pIViewItemEvent);
	//����λ��
	VOID SetViewItemPostion(INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//���ƺ���
public:
	//��������
	VOID ActiveViewItem(CWnd * pItemView);
	//ɾ������
	VOID RemoveViewItem(CWnd * pItemView);
	//��������
	VOID InsertViewItem(CWnd * pItemView, LPCTSTR pszTitleImage);

	//�ڲ�����
private:
	//��������
	WORD GetViewItemIndex(CWnd * pItemView);
	//��������
	WORD GetViewItemIndex(CPoint MousePoint);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif