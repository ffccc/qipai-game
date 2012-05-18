#ifndef SKIN_BUTTON_HEAD_FILE
#define SKIN_BUTTON_HEAD_FILE

#pragma once

#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��ť��
class SKIN_CONTROL_CLASS CSkinButton : public CButton
{
	//״̬����
protected:
	bool							m_bHovering;						//������־
	bool							m_bTransparent;						//�Ƿ�͸��
	bool							m_bRenderImage;						//��Ⱦ����

	//���ñ���
protected:
	WORD							m_wImageIndex;						//��������
	COLORREF						m_crButtonText;						//������ɫ

	//������Դ
protected:
	LPCTSTR							m_pszSurface;						//��ť��Դ
	LPCTSTR							m_pszResource;						//��Դ��Ϣ
	HINSTANCE						m_hResInstance;						//��Դ���

	//��������
public:
	//���캯��
	CSkinButton();
	//��������
	virtual ~CSkinButton();

	//���غ���
protected:
	//�󶨺���
	virtual VOID PreSubclassWindow();
	//�滭����
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//���ú���
public:
	//������Ⱦ
	VOID SetRenderImage(bool bRenderImage);
	//������ɫ
	VOID SetButtonColor(COLORREF crButtonText);

	//��������
public:
	//����λͼ
	bool SetButtonImage(UINT uBitmapID, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent);
	//����λͼ
	bool SetButtonImage(LPCTSTR pszResource, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent);
	//����λͼ
	bool SetButtonImage(UINT uBitmapID, LPCTSTR pszSurface, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent);
	//����λͼ
	bool SetButtonImage(LPCTSTR pszResource, LPCTSTR pszSurface, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent);

	//��������
private:
	//�����ؼ�
	VOID RectifyControl();
	//�滭����
	VOID DrawButtonText(CDC * pDC, UINT uItemState);

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnNcDestroy();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif