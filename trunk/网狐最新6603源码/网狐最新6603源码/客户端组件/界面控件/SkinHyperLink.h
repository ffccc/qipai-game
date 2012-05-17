#ifndef SKIN_HYPER_LINK_HEAD_FILE
#define SKIN_HYPER_LINK_HEAD_FILE

#pragma once

#include "SkinControlHead.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class SKIN_CONTROL_CLASS CSkinHyperLinkAttribute
{
	//���ò���
public:
	COLORREF						m_crBackColor;						//������ɫ
	COLORREF						m_crHoverText;						//������ɫ
	COLORREF						m_crNormalText;						//������ɫ
	COLORREF						m_crVisitedText;					//������ɫ

	//��������
public:
	//���캯��
	CSkinHyperLinkAttribute();
	//��������
	virtual ~CSkinHyperLinkAttribute();

	//���ܺ���
public:
	//������Դ
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class SKIN_CONTROL_CLASS CSkinHyperLink : public CStatic
{
	//״̬����
protected:
	bool							m_bVisited;                     	//�Ƿ����
   	bool							m_bHovering;						//������־

	//���ñ���
protected:
    bool							m_bUnderLine;						//�Ƿ���
    bool							m_AutoRectify;						//�Զ�����

	//��ɫ����
protected:
	COLORREF						m_crBackColor;						//������ɫ
	COLORREF						m_crHoverText;						//������ɫ
	COLORREF						m_crNormalText;						//������ɫ
	COLORREF						m_crVisitedText;					//������ɫ

	//���Ա���
public:
	static CSkinHyperLinkAttribute	m_SkinAttribute;					//��������

	//��������
public:
	//���캯��
	CSkinHyperLink();
	//��������
	virtual ~CSkinHyperLink();

	//���غ���
protected:
	//�ؼ���
	virtual VOID PreSubclassWindow();

	//��ѯ����
public:
	//��ȡ����
	bool IsVisiteded() { return m_bVisited; }
	//��ȡ�»���
	bool IsUnderline() { return m_bUnderLine; }
	//��ȡ����
	bool IsAutoRectify() { return m_AutoRectify; }

	//���ú���
public:
	//���÷���
    VOID SetVisited(bool bVisited);
	//���»���
    VOID SetUnderLine(bool bUnderLine);
	//���õ���
    VOID SetAutoRectify(bool bAutoRectify);

	//��ɫ����
public:
	//������ɫ
	VOID SetBackGroundColor(COLORREF crBackColor);
	//������ɫ
	VOID SetHyperLinkTextColor(COLORREF crHoverText, COLORREF crNormalText, COLORREF crVisitedText);

	//��������
protected:
	//����λ��
	VOID RectifyControl();
	//���ÿؼ�
	VOID Initialization();

	//��Ϣ����
protected:
	//�滭��Ϣ
	VOID OnPaint();
	//�����ؼ�
	VOID OnStnClicked();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
    BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//�Զ���Ϣ
protected:
	//������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);
	//����뿪
	LRESULT OnMouseLeaveMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif