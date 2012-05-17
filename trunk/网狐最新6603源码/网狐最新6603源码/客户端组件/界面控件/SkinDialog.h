#ifndef SKIN_DIALOG_HEAD_FILE
#define SKIN_DIALOG_HEAD_FILE

#pragma once

#include "SkinEncircle.h"
#include "SkinControlHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//������Դ
class SKIN_CONTROL_CLASS CSkinDialogAttribute
{
	//��ɫ����
public:
	COLORREF						m_crTitleText;						//������ɫ
	COLORREF						m_crTitleFrame;						//������ɫ
	COLORREF						m_crBackGround;						//������ɫ
	COLORREF						m_crControlText;					//������ɫ

	//��Դ����
public:
	CSize							m_SizeButton;						//��ť��С
	CBrush							m_brBackGround;						//������ˢ
	CEncirclePNG					m_PNGEncircleFrame;					//�����Դ

	//λ�ñ���
public:
	tagEncircleInfo					m_EncircleInfoView;					//��ͼ����
	tagEncircleInfo					m_EncircleInfoFrame;				//��ܻ���

	//��������
public:
	//���캯��
	CSkinDialogAttribute();
	//��������
	virtual ~CSkinDialogAttribute();

	//������
public:
	//������Դ
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//�ֲ㴰��
class SKIN_CONTROL_CLASS CSkinLayered : public CWnd
{
	//��������
protected:
	CWnd *							m_pWndControl;						//�ؼ�����

	//��������
public:
	//���캯��
	CSkinLayered();
	//��������
	virtual ~CSkinLayered();

	//���ܺ���
public:
	//��������
	VOID CreateLayered(CWnd * pWndControl, CRect & rcWindow);
	//��������
	VOID InitLayeredArea(CDC * pDCImage, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild);
	//��������
	VOID InitLayeredArea(CPngImage & Image, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild);

	//��Ϣӳ��
protected:
	//�ر���Ϣ
	VOID OnClose();
	//������Ϣ
	VOID OnSetFocus(CWnd * pOldWnd);

	//��̬����
protected:
	//ö�ٺ���
	static BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�Ի�����
class SKIN_CONTROL_CLASS CSkinDialog : public CDialog
{
	//��ť״̬
private:
	BYTE							m_cbNowHotIndex;					//���ڽ���
	BYTE							m_cbNowHitIndex;					//���°�ť
	BYTE							m_cbButtonState[3];					//��ť״̬

	//����״̬
private:
	bool							m_bTransition;						//����״̬
	WORD							m_wImageIndex[3];					//��������

	//���ڱ���
private:
	CRect							m_rcButton[3];						//��ťλ��
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��

	//��Դ����
public:
	static CSkinDialogAttribute		m_SkinAttribute;					//��Դ����

	//��������
public:
	//���캯��
	CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd=NULL);
	//��������
	virtual ~CSkinDialog();

	//���غ���
public:
	//������Ϣ
	virtual BOOL OnInitDialog();
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//��������
private:
	//��������
	UINT ControlHitTest(CPoint Point);
	//������Դ
	VOID RectifyResource(INT nWidth, INT nHeight);
	//������ť
	VOID CaleTitleButton(INT nWidth, INT nHeight);

	//�滭����
private:
	//�滭�ؼ�
	VOID DrawControlView(CDC * pDC, INT nWdith, INT nHeight);
	//�滭����
	VOID DrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//�滭��ť
	VOID DrawSystemButton(CDC * pDC, INT nWdith, INT nHeight);

	//��Ϣӳ��
protected:
	//�����Ϣ
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

	//��Ϣӳ��
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//�ؼ���ɫ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	//�Զ���Ϣ
protected:
	//������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif