#ifndef SKIN_LISTCTRL_HEAD_FILE
#define SKIN_LISTCTRL_HEAD_FILE

#pragma once

#include "SkinImage.h"
#include "SkinControls.h"
//#include "SkinAttribute.h"
#include "SkinResourceManager.h"

//��˵��
class CSkinListCtrl;
class CSkinHeaderCtrl;

//////////////////////////////////////////////////////////////////////////

//��������
//#define SKIN_LIST_CRHEADTEXT			TEXT("��ͷ������ɫ")			// COLORREF ���� ����д��
//#define SKIN_LIST_STRIMAGEHEADPATH		TEXT("��ͷ����ͼ")				// CString ���� ����д��

//////////////////////////////////////////////////////////////////////////

//�б�ͷ�ص��ӿ�
interface IHeaderCtrlSink
{
	//����Ҽ�����
	virtual void __cdecl OnHeaderRButtonDown(CSkinHeaderCtrl * pHeaderCtrl, UINT nFlags, CPoint Point)=NULL;
	//����Ҽ�����
	virtual void __cdecl OnHeaderRButtonUp(CSkinHeaderCtrl * pHeaderCtrl, UINT nFlags, CPoint Point)=NULL;
	//����������
	virtual void __cdecl OnHeaderLButtonDown(CSkinHeaderCtrl * pHeaderCtrl, UINT nFlags, CPoint Point)=NULL;
	//����������
	virtual void __cdecl OnHeaderLButtonUp(CSkinHeaderCtrl * pHeaderCtrl, UINT nFlags, CPoint Point)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//�б�������
class CSkinListAttribute : public CSkinResourceManager
{
	friend class CSkinListCtrl;
	friend class CSkinHeaderCtrl;

	//���ò���
public:
	COLORREF							m_crHeadText;					//������ɫ

	//GDI ����
public:
	CSkinImage							m_ImageHead;					//��ͷλͼ

	//��������
protected:
	//���캯��
	CSkinListAttribute();
	//��������
	virtual ~CSkinListAttribute();

	//�ӿں���
public:
	//��������
	bool UpdateSkinResource(ISkinResource * pISkinResource);
};

//////////////////////////////////////////////////////////////////////////

//����ͷ��־
enum enHeaderSortType
{
	Type_NoSort,						//û������
	Type_Ascend,						//��������
	Type_Descale,						//��������
};

//////////////////////////////////////////////////////////////////////////

//��Ƥ���б�ͷ
class SKIN_CONTROL_CLASS CSkinHeaderCtrl : public CHeaderCtrl
{
	//��̬����
protected:
	int									m_iSortCol;						//��ǰ����
	enHeaderSortType					m_SortType;						//�����־
	IHeaderCtrlSink						* m_pIHeaderCtrlSink;			//�ص��ӿ�

	//�ڲ�����
protected:
	CDC									m_BufferDC;						//���� DC

	//���Ա���
public:
	static CSkinListAttribute			m_SkinAttribute;				//�б�����

	//��������
public:
	//���캯��
	CSkinHeaderCtrl();
	//��������
	virtual ~CSkinHeaderCtrl();

	//���غ���
protected:
	//�滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//���ܺ���
public:
	//����ת��
	int PointToColIndex(CPoint Point);
	//��ȡ����
	LPCTSTR GetItemText(int iCol, TCHAR szBuffer[], int nSize);
	//���ñ�־
	void SetSortArrow(int iSortCol, enHeaderSortType SortType);
	//��ȡ��־
	void GetSortArrow(int & iSortCol, enHeaderSortType & SortType);
	//��ȡ�ص��ӿ�
	IHeaderCtrlSink * GetHeaderCtrlSink() { return m_pIHeaderCtrlSink; }
	//���ûص��ӿ�
	void SetHeaderCtrlSink(IHeaderCtrlSink * pIHeaderCtrlSink) { m_pIHeaderCtrlSink=pIHeaderCtrlSink; }

	//��ѯ����
public:
	//��ȡ���Խӿ�
	static CSkinListAttribute * GetSkinAttribute() { return &m_SkinAttribute; }

	//��Ϣ����
protected:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//�б���
class SKIN_CONTROL_CLASS CSkinListCtrl : public CListCtrl
{
	//��������
protected:
	CSkinHeaderCtrl						m_SkinHeadCtrl;					//��ͷ�ؼ�
	
	//��������
public:
	//���캯��
	CSkinListCtrl();
	//��������
	virtual ~CSkinListCtrl();

	//���غ���
public:
	//�ؼ���
	virtual void PreSubclassWindow();

	//���ܺ���
public:
	//��ȡ�б�ͷ
	CSkinHeaderCtrl & GetHeadCtrl() { return m_SkinHeadCtrl; }
	//���ñ�־
	void SetSortArrow(int iSortCol, enHeaderSortType SortType) { return m_SkinHeadCtrl.SetSortArrow(iSortCol,SortType); }
	//��ȡ��־
	void GetSortArrow(int & iSortCol, enHeaderSortType & SortType) { m_SkinHeadCtrl.GetSortArrow(iSortCol,SortType); };
	//��ȡ�ص��ӿ�
	IHeaderCtrlSink * GetSinkSink() { return m_SkinHeadCtrl.GetHeaderCtrlSink(); }
	//���ûص��ӿ�
	void SetHeaderCtrlSink(IHeaderCtrlSink * pIHeaderCtrlSink) { m_SkinHeadCtrl.SetHeaderCtrlSink(pIHeaderCtrlSink); }

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinListCtrl)
};

//////////////////////////////////////////////////////////////////////////

#endif