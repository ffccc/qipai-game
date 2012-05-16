#ifndef SKIN_SPLITTER_HEAD_FILE
#define SKIN_SPLITTER_HEAD_FILE

#pragma once

#include "SkinImage.h"
#include "SkinButton.h"
#include "SkinControls.h"
//#include "SkinAttribute.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////

//��������
//#define SKIN_SPLITTER_IMAGEHOR_PATH		TEXT("���򱳾�ͼ")				// CString ���� ����д��
//#define SKIN_SPLITTER_IMAGEVOR_PATH		TEXT("���򱳾�ͼ")				// CString ���� ����д��

//���������
class CSkinSplitterAttribute : public CSkinResourceManager
{
	friend class CSkinSplitter;

	//GDI ����
public:
	CSkinImage							m_ImageHor;						//���λͼ
	CSkinImage							m_ImageVor;						//���λͼ

	//��������
protected:
	//���캯��
	CSkinSplitterAttribute();
	//��������
	virtual ~CSkinSplitterAttribute();

	//�ӿں���
public:
	//��������
	bool UpdateSkinResource(ISkinResource * pISkinResource);
};

//////////////////////////////////////////////////////////////////////////

//��˵��
class CSkinSplitter;

//���������
enum enSplitterType
{
	HorSpliter,
	VorSpliter,
};

//���������ӿ�
interface ISplitterSink
{
	//��ť��Ϣ
	virtual bool __cdecl OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton)=NULL;
	//�������Ϣ
	virtual bool __cdecl OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//�������
class SKIN_CONTROL_CLASS CSkinSplitter : public CWnd
{
	//��������
protected:
	int									m_nMaxPos;							//���λ��
	int									m_nLessPos;							//��Сλ��
	bool								m_bEnalbe;							//���ñ�־
	bool								m_bMouseDown;						//�Ƿ���
	CRect								m_rcSplitter;						//��־���
	enSplitterType						m_SplitterType;						//���������
	ISplitterSink						* m_pISplitterSink;					//����¼�

	//�ؼ�����
protected:
	CSkinButton							m_SplitterButton;					//��ְ�ť

	//���������
public:
	static CSkinSplitterAttribute		m_SkinAttribute;					//���������

	//��������
public:
	//���캯��
	CSkinSplitter(enSplitterType SplitterType, ISplitterSink * pISplitterSink=NULL);
	//��������
	virtual ~CSkinSplitter();

	//���غ���
protected:
	//�󶨺���
	virtual VOID PreSubclassWindow();

	//���ú���
public:
	//���ò�ַ�Χ
	void SetSplitterRange(int nLessPos, int nMaxPos);
	//���ò������
	void SetSplitterType(enSplitterType SplitterType);
	//���ûص��ӿ�
	void SetSplitterSink(ISplitterSink * pISplitterSink);

	//���ܺ���
public:
	//��������
	void EnableSplitter(bool bEnable);
	//���ذ�ť
	bool HideSplitterButton();
	//��ʾ��ť
	bool ShowSplitterButton(UINT uButtonImage, HINSTANCE hInstance);
	//��ť��С
	CSize GetButtonSize();

	//��ѯ����
public:
	//��ȡ�������
	enSplitterType GetSplitterType() { return m_SplitterType; }
	//��ȡ�ص��ӿ�
	ISplitterSink * GetSplitterSink() { return m_pISplitterSink; };
	//���ò�ַ�Χ
	void GetSplitterRange(int & nLessPos, int & nMaxPos) { nLessPos=m_nLessPos; nMaxPos=m_nMaxPos; }
	//��ȡ���Խӿ�
	static CSkinSplitterAttribute * GetSkinAttribute() { return &m_SkinAttribute; }

	//��Ϣ����
protected:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg void OnSplitterButton();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//ʧȥ����
	afx_msg void OnKillFocus(CWnd * pNewWnd);
	//�����Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinSplitter)
};

//////////////////////////////////////////////////////////////////////////

#endif