#ifndef SKIN_LISTCTRL_HEAD_FILE
#define SKIN_LISTCTRL_HEAD_FILE

#pragma once

#include "SkinScrollBar.h"
#include "SkinControlHead.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//����λ��
#define SORT_AFTER					1									//����λ��
#define SORT_FRONT					-1									//ǰ��λ��

//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//��������
enum enSortType
{
	SortType_NoSort,				//û������
	SortType_Ascend,				//��������
	SortType_Descale,				//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CSkinListData;
class CSkinListCtrl;
class CSkinHeaderCtrl;

//////////////////////////////////////////////////////////////////////////////////

//�б�����
class SKIN_CONTROL_CLASS CSkinListCtrlAttribute
{
	//��Ԫ����
	friend class CSkinListCtrl;
	friend class CSkinHeaderCtrl;

	//��ɫ����
public:
	COLORREF						m_crTitle;							//������ɫ
	COLORREF						m_crListTX;							//�б�����
	COLORREF						m_crListBK;							//�б��ɫ

	//��Դ����
public:
	CBitImage						m_SkinImageL;						//����λͼ
	CBitImage						m_SkinImageM;						//����λͼ
	CBitImage						m_SkinImageR;						//����λͼ

	//��������
protected:
	//���캯��
	CSkinListCtrlAttribute();
	//��������
	virtual ~CSkinListCtrlAttribute();

	//���ܺ���
public:
	//������Դ
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//��ͷ�ؼ�
class SKIN_CONTROL_CLASS CSkinHeaderCtrl : public CHeaderCtrl
{
	//�����б�
protected:
	UINT							m_uLockCount;						//������Ŀ
	UINT							m_uItemHeight;						//����߶�

	//��������
public:
	//���캯��
	CSkinHeaderCtrl();
	//��������
	virtual ~CSkinHeaderCtrl();

	//���غ���
public:
	//�ؼ���
	virtual VOID PreSubclassWindow();
	//�ؼ���Ϣ
	virtual BOOL OnChildNotify(UINT uMessage, WPARAM wParam, LPARAM lParam, LRESULT * pLResult);

	//���ܺ���
public:
	//��������
	VOID SetLockCount(UINT uLockCount);
	//�����и�
	VOID SetItemHeight(UINT uItemHeight);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�б�ؼ�
class SKIN_CONTROL_CLASS CSkinListCtrl : public CListCtrl
{
	//���ݱ���
protected:
	bool							m_bAscendSort;						//�����־

	//��������
public:
	CSkinScrollBar					m_SkinScrollBar;					//��������
	CSkinHeaderCtrl					m_SkinHeaderCtrl;					//��ͷ�ؼ�

	//���Ա���
public:
	static CSkinListCtrlAttribute	m_SkinAttribute;					//�б�����

	//��������
public:
	//���캯��
	CSkinListCtrl();
	//��������
	virtual ~CSkinListCtrl();

	//���غ���
public:
	//�ؼ���
	virtual VOID PreSubclassWindow();
	//�滭����
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//���뺯��
public:
	//��������
	virtual VOID InsertDataItem(VOID * pItemData);
	//��������
	virtual VOID UpdateDataItem(VOID * pItemData);
	//ɾ������
	virtual VOID DeleteDataItem(VOID * pItemData);

	//���ݿ���
protected:
	//��ȡλ��
	virtual INT GetInsertIndex(VOID * pItemData);
	//��������
	virtual INT SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort);
	//�����ַ�
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);

	//�滭����
protected:
	//��ȡ��ɫ
	virtual VOID GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF & crColorText, COLORREF & crColorBack);
	//�滭����
	virtual VOID DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect & rcSubItem, INT nColumnIndex);

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	//��̬����
protected:
	//���к���
	static INT CALLBACK SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);

	//��Ϣӳ��
protected:
	//����б�
	VOID OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif