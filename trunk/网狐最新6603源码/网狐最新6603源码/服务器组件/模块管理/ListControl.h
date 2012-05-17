#ifndef LIST_CONTROL_HEAD_FILE
#define LIST_CONTROL_HEAD_FILE

#pragma once

#include "ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��ɫ����
#define CR_NORMAL_TK				RGB(255,255,255)					//�б���
#define CR_NORMAL_BK				RGB(255,255,255)					//�б���

//����λ��
#define SORT_POSITION_AFTER			1									//����λ��
#define SORT_POSITION_FRONT			-1									//ǰ��λ��

//////////////////////////////////////////////////////////////////////////////////

//��ɫ����
struct tagListItemColor
{
	COLORREF						rcTextColor;						//������ɫ
	COLORREF						rcBackColor;						//������ɫ
};

//////////////////////////////////////////////////////////////////////////////////

//��ͷ�ؼ�
class MODULE_MANAGER_CLASS CListHeaderCtrl : public CHeaderCtrl
{
	//��������
public:
	//���캯��
	CListHeaderCtrl();
	//��������
	virtual ~CListHeaderCtrl();

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�б�ؼ�
class MODULE_MANAGER_CLASS CListControl : public CListCtrl
{
	//״̬����
protected:
	bool							m_bAscendSort;						//�����־

	//�������
protected:
	CListHeaderCtrl					m_ListHeaderCtrl;					//��ͷ�ؼ�

	//��������
public:
	//���캯��
	CListControl();
	//��������
	virtual ~CListControl();

	//���غ���
public:
	//�ؼ���
	virtual VOID PreSubclassWindow();
	//�滭����
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//���ú���
private:
	//�����б�
	virtual VOID InitListControl()=NULL;
	//��������
	virtual INT SortListItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex)=NULL;
	//��ȡ��ɫ
	virtual VOID GetListItemColor(LPARAM lParam, UINT uItemStatus, tagListItemColor & ListItemColor)=NULL;

	//��̬����
protected:
	//���к���
	static INT CALLBACK SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);

	//��Ϣӳ��
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//����б�
	VOID OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////

#endif
