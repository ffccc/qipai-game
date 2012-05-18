#ifndef GAME_PROPERTY_CTRL_HEAD_FILE
#define GAME_PROPERTY_CTRL_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class GAME_PROPERTY_CLASS CGamePropertyCtrl : public CWnd
{
	//״̬����
protected:
	bool							m_bHovering;						//������־
	WORD							m_wDownItem;						//�������
	WORD							m_wHoverItem;						//��������

	//���ñ���
protected:
	CSize							m_SizeImageItem;					//����ͼƬ
	tagPropertyResource				m_PropertyResource;					//��Դ����

	//��������
protected:
	BYTE							m_cbPropertyCount;					//������Ŀ
	CGamePropertyItem *				m_pGamePropertyItem[MAX_PROPERTY];	//��������

	//�ӿڱ���
protected:
	IGamePropertySink *				m_pIGamePropertySink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CGamePropertyCtrl();
	//��������
	virtual ~CGamePropertyCtrl();

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//���ú���
public:
	//���ýӿ�
	bool SetGamePropertySink(IUnknownEx * pIUnknownEx);
	//������Դ
	bool SetPropertyResource(tagPropertyResource & PropertyResource);
	//���õ���
	bool SetGamePropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount, WORD wIssueArea);

	//�ڲ�����
protected:
	//����ж�
	WORD SwitchItemFromPoint(CPoint MousePoint);

	//��Ϣӳ��
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif