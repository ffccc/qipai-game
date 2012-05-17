#ifndef SKIN_SPLITTER_HEAD_FILE
#define SKIN_SPLITTER_HEAD_FILE

#pragma once

#include "SkinControlHead.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
enum enSplitterType
{
	SplitterType_Hor,				//������
	SplitterType_Vor,				//������
};

//����¼�
interface ISplitterEvent
{
	//����¼�
	virtual VOID OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�������
class SKIN_CONTROL_CLASS CSkinSplitter : public CWnd
{
	//״̬����
protected:
	bool							m_bMouseDown;						//�Ƿ���
	CRect							m_rcDrawLine;						//����λ��

	//���Ա���
protected:
	INT								m_nMaxDragPos;						//���λ��
	INT								m_nMinDragPos;						//��Сλ��
	enSplitterType					m_SplitterType;						//�������

	//��ɫ����
protected:
	DOUBLE							m_ColorS;							//������ɫ
	DOUBLE							m_ColorB;							//������ɫ
	COLORREF						m_crBackColor;						//������ɫ

	//�������
protected:
	ISplitterEvent *				m_pISplitterEvent;					//����¼�

	//��������
public:
	//���캯��
	CSkinSplitter();
	//��������
	virtual ~CSkinSplitter();

	//���غ���
protected:
	//�󶨺���
	virtual VOID PreSubclassWindow();
	//������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ú���
public:
	//��������
	VOID SetSplitterType(enSplitterType SplitterType);
	//���÷�Χ
	VOID SetSplitterRange(INT nMinDragPos, INT nMaxDragPos);
	//���ýӿ�
	VOID SetSplitterEvent(ISplitterEvent * pISplitterEvent);

	//������ɫ
public:
	//������ɫ
	VOID SetSplitterColor(DOUBLE S, DOUBLE B);
	//������ɫ
	VOID SetSplitterColor(COLORREF crBackColor);

	//��ѯ����
public:
	//��ȡ����
	enSplitterType GetSplitterType() { return m_SplitterType; }
	//��ȡ�ӿ�
	ISplitterEvent * GetSplitterEvent() { return m_pISplitterEvent; };
	//���÷�Χ
	VOID GetSplitterRange(INT & nMinDragPos, INT & nMaxDragPos) { nMinDragPos=m_nMinDragPos; nMaxDragPos=m_nMaxDragPos; }

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//ʧȥ����
	VOID OnKillFocus(CWnd * pNewWnd);
	//�����Ϣ
	VOID OnMouseMove(UINT nFlags, CPoint point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMmessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif