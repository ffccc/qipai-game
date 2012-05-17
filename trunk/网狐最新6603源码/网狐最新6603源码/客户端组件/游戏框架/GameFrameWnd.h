#ifndef GAME_FRAME_WND_HEAD_FILE
#define GAME_FRAME_WND_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameControl.h"

//////////////////////////////////////////////////////////////////////////////////

//�����Դ
class CFrameEncircle : public CEncircleBMP
{
	//��������
public:
	//���캯��
	CFrameEncircle();
	//��������
	virtual ~CFrameEncircle();

	//���غ���
protected:
	//�滭����
	virtual bool PreDrawEncircleImage(tagEncircleBMP & EncircleImage);
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class GAME_FRAME_CLASS CGameFrameWnd : public CFrameWnd, public IGameFrameWnd
{
	//״̬����
protected:
	bool							m_bMaxShow;							//����־
	bool							m_bShowControl;						//��ʾ��־
	CRect							m_rcNormalSize;						//����λ��

	//��������
protected:
	INT								m_nScrollXPos;						//����λ��
	INT								m_nScrollYPos;						//����λ��
	INT								m_nScrollXMax;						//���λ��
	INT								m_nScrollYMax;						//���λ��

	//����ӿ�
protected:
	IClientKernel *					m_pIClientKernel;					//�ں˽ӿ�
	IGameFrameView *				m_pIGameFrameView;					//��ͼ�ӿ�

	//�ؼ�����
public:
	CSkinButton						m_btMin;							//��С��ť
	CSkinButton						m_btMax;							//���ť
	CSkinButton						m_btClose;							//�رհ�ť
	CSkinScrollBar					m_SkinScrollBar;					//��������

	//���ܿؼ�
public:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CSkinSplitter					m_SkinSplitter;						//��ֿؼ�
	CGameFrameControl				m_GameFrameControl;					//���ƿ��

	//��Դ����
protected:
	CFrameEncircle					m_FrameEncircle;					//�����Դ

	//��������
public:
	//���캯��
	CGameFrameWnd();
	//��������
	virtual ~CGameFrameWnd();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���溯��
protected:
	//��ԭ����
	bool RestoreWindow();
	//��󴰿�
	bool MaxSizeWindow();

	//�������
protected:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��������
	UINT OnNcHitTest(CPoint Point);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//λ����Ϣ
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�ı���Ϣ
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	//�Զ���Ϣ
protected:
	//������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif