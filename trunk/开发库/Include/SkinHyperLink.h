#ifndef SKIN_HYPER_LINK_HEAD_FILE
#define SKIN_HYPER_LINK_HEAD_FILE

#pragma once

#include "SkinControls.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////

//��������
//#define SKIN_LINK_CRHOVERTEXT			TEXT("����������ɫ")			// COLORREF ���� ����д��
//#define SKIN_LINK_CRVISITEDTEXT			TEXT("����������ɫ")			// COLORREF ���� ����д��
//#define SKIN_LINK_CRNORMALTEXT			TEXT("����������ɫ")			// COLORREF ���� ����д��
//#define SKIN_LINK_CRBACKGROUND			TEXT("���屳����ɫ")			// COLORREF ���� ����д��

//�б�������
class CSkinHyperLinkAttribute : public CSkinResourceManager
{
	friend class CSkinHyperLink;

	//���ò���
public:
	COLORREF							m_crHoverText;					//������ɫ
	COLORREF							m_crVisitedText;				//������ɫ
	COLORREF							m_crNormalText;					//������ɫ
	COLORREF							m_crBackColor;					//������ɫ

	//��������
protected:
	//���캯��
	CSkinHyperLinkAttribute();
	//��������
	virtual ~CSkinHyperLinkAttribute();

	//�ӿں���
public:
	//��������
	bool UpdateSkinResource(ISkinResource * pISkinResource);
};

//////////////////////////////////////////////////////////////////////////

//��������
class SKIN_CONTROL_CLASS CSkinHyperLink : public CStatic
{
	//��������
protected:
   	bool								m_bHovering;					//������־
	bool								m_bVisited;                     //�Ƿ����
    bool								m_bUnderline;					//�Ƿ���
    bool								m_bAutoAdjust;					//�Զ�����
    CString								m_strURL;						//���ӵ�ַ
	COLORREF							m_crBackColor;					//������ɫ

	//�ӿ�ָ��
protected:
	IMouseEvent							* m_pIMouseEvent;				//����¼�

	//�ؼ�����
protected:
    CToolTipCtrl						m_ToolTip;						//��ʾ�ؼ�

	//���Ա���
public:
	static CSkinHyperLinkAttribute		m_SkinAttribute;				//����������

	//��������
public:
	//���캯��
	CSkinHyperLink();
	//��������
	virtual ~CSkinHyperLink();

	//���ú���
public:
	//�������ص��ӿ�
	void SetMouseEventSink(IUnknownEx * pIUnknownEx);
	//��ȡ���ص��ӿ�
	void * GetMouseEventSink(const IID & Guid, DWORD dwQueryVer);

	//��ѯ����
public:
	//��ȡ����
	bool GetAutoAdjust() { return m_bAutoAdjust; }
	//��ȡ����
	bool GetVisited() { return m_bVisited; }
	//��ȡ�»���
	bool GetUnderline() { return m_bUnderline; }
	//��ȡ URL
	LPCTSTR GetHyperLinkUrl() { return m_strURL; }

	//���ú���
public:
	//���÷���
    void SetVisited(bool bVisited);
	//�����»���
    void SetUnderline(bool bUnderline);
	//���õ���
    void SetAutoAdjust(bool bAutoAdjust);
	//���� URL
	void SetHyperLinkUrl(LPCTSTR pszUrl);
	//������ɫ
	void SetBackGroundColor(COLORREF crColor);

	//���غ���
protected:
	//�ؼ����໯
	virtual void PreSubclassWindow();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//Ĭ�ϻص�����
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//��ѯ����
public:
	//��ȡ���Խӿ�
	static CSkinHyperLinkAttribute * GetSkinAttribute() { return &m_SkinAttribute; }

	//�ڲ�����
protected:
	//��ʼ��
	void InitHyperLink();
	//������ʾ
	void UpdateToolTip();
	//����λ��
	void AdjustHyperLink();

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�滭��Ϣ
	afx_msg void OnPaint();
	//�����ؼ�
	afx_msg void OnStnClicked();
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//�����Ϣ
    afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	afx_msg LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinHyperLink)
};

//////////////////////////////////////////////////////////////////////////

#endif