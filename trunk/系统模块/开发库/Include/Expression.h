#ifndef EXPRESSION_HEAD_FILE
#define EXPRESSION_HEAD_FILE

#pragma once

#include "ClientShare.h"

//�궨��
#define MAX_EXPRESS_ITEM			121										//������Ŀ
#define WM_HIT_EXPMESSTION			WM_USER+100								//��Ϣ����

//////////////////////////////////////////////////////////////////////////

//��������
class CLIENT_SHARE_CLASS CExpressionItem
{
	//��������
public:
	WORD							m_wTranceLength;						//���ͳ���
	TCHAR							m_szTrancelate[12];						//�����ַ�
	TCHAR							m_szChineseName[12];					//���Ľ���

	//��������
public:
	//���캯��
	CExpressionItem();
	//��������
	~CExpressionItem();

	//���ܺ���
public:
	//��������
	void InitExpressItem(LPCTSTR pszChineseName, LPCTSTR pszTrancelate);
};

//////////////////////////////////////////////////////////////////////////

//���鶯��������
class CLIENT_SHARE_CLASS CExpression : public CDialog
{
	//��������
protected:
	WORD							m_wSelectIndex;							//ѡ������
	BYTE							m_cbPageIndex;							//��ҳ����
	TCHAR							m_szPath[MAX_PATH];						//��ǰ·��

	//ȫ�ֱ���
protected:
	static WORD						m_wMaxRow;								//�������Ŀ
	static WORD						m_wMaxLine;								//�������Ŀ

	//�ؼ�����
protected:
	CSkinButton						m_btLastPage;							//��ҳ��ť
	CSkinButton						m_btNextPage;							//��ҳ��ť

public:
	static CExpressionItem			m_ExpreesionItem[MAX_EXPRESS_ITEM];		//��������

	//��������
public:
	//���캯��
	CExpression();
	//��������
	virtual ~CExpression();

	//���ܺ���
public:
	//��������
	bool CreateExpression(CWnd * pParentWnd);
	//��ȡ��С
	bool GetFixSize(CSize & Size);
	//��ȡ����
	static CExpressionItem * GetExpressItem(WORD wIndex);

	//��������
protected:
	//��ʼ����Դ
	static bool InitExpressItem();
	//�滭ͷ��
	void DrawItem(CDC * pDC, int iXPos, int iYPos, bool bSelected);
	//��������
	WORD HitItemTest(CPoint MousePoint);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);

	//��Ϣ����
public:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//ʧȥ������Ϣ
	afx_msg void OnKillFocus(CWnd * pNewWnd);
	//����ƶ���Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//��갴����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
};

//////////////////////////////////////////////////////////////////////////

#endif