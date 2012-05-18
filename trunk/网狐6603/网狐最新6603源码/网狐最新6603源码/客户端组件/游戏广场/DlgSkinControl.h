#ifndef DLG_SKIN_CONTROL_HEAD_FILE
#define DLG_SKIN_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
class CDlgSkinControl : public CDialog
{
	//��������
protected:
	WORD							m_wHoverItem;						//��������
	WORD							m_wHoverColor;						//������ɫ
	WORD							m_wMouseDownColor;					//�����ɫ

	//��С����
protected:
	CSize							m_SizeSkinItem;						//ѡ������
	CSize							m_SizeColorItem;					//ѡ����ɫ

	//ѡ�����
protected:
	tagSkinRenderInfo				m_RenderInfoItem[8];				//��ɫ����
	tagSkinRenderInfo				m_CurrentRenderInfo;				//��ɫѡ��

	//��������
public:
	//���캯��
	CDlgSkinControl();
	//��������
	virtual ~CDlgSkinControl();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK() { return; }
	//ȡ����Ϣ
	virtual VOID OnCancel() { return; }

	//��������
protected:
	//��ȡѡ��
	WORD GetCurrentItem(CPoint MousePoint);
	//��ȡѡ��
	WORD GetCurrentColor(CPoint MousePoint);
	//������ɫ
	VOID SetCurrentColor(WORD wColorIndex, INT nXExcursion);

	//��Ϣ����
public:
	//�ػ���Ϣ
	VOID OnPaint();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//������Ϣ
	VOID OnKillFocus(CWnd * pNewWnd);
	//�����Ϣ
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif