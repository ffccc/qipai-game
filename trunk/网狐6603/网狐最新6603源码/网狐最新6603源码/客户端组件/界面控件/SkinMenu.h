#ifndef SKIN_MENU_HEAD_FILE
#define SKIN_MENU_HEAD_FILE

#pragma once

#include "SkinControlHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//�˵�����
class SKIN_CONTROL_CLASS CSkinMenuAttribute
{
	//������ɫ
public:
	COLORREF						m_crNormalText;						//�˵���ɫ
	COLORREF						m_crSelectText;						//�˵���ɫ

	//�߿���ɫ
public:
	COLORREF						m_crMenuBar;						//�˵���ɫ
	COLORREF						m_crSeparator;						//�˵���ɫ
	COLORREF						m_crNormalBack;						//�˵���ɫ
	COLORREF						m_crSelectBack;						//�˵���ɫ
	COLORREF						m_crSelectBorder;					//�˵���ɫ

	//��������
public:
	//���캯��
	CSkinMenuAttribute();
	//��������
	virtual ~CSkinMenuAttribute();

	//������
public:
	//������Դ
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
enum enMenuItemType
{
	MenuItemType_Image,				//ͼ������
	MenuItemType_String,			//�ַ�����
	MenuItemType_Separator,			//�������
};

//////////////////////////////////////////////////////////////////////////////////

//�˵�����
class CSkinMenuItem
{
	//��������
public:
	const enMenuItemType			m_MenuItemType;						//��������

	//��������
public:
	//���캯��
	CSkinMenuItem(enMenuItemType MenuItemType) : m_MenuItemType(MenuItemType) {}
	//��������
	virtual ~CSkinMenuItem() {}
};

//ͼ�β˵�
class CSkinMenuImage : public CSkinMenuItem
{
	//��������
public:
	HBITMAP							m_hBitmap;							//ͼ����

	//��������
public:
	//���캯��
	CSkinMenuImage() : CSkinMenuItem(MenuItemType_Image) { m_hBitmap=NULL; }
	//��������
	virtual ~CSkinMenuImage() {}
};

//�ַ��˵�
class CSkinMenuString : public CSkinMenuItem
{
	//��������
public:
	CString							m_strString;						//�˵��ַ�

	//��������
public:
	//���캯��
	CSkinMenuString() : CSkinMenuItem(MenuItemType_String) {}
	//��������
	virtual ~CSkinMenuString() {}
};

//��ֲ˵�
class CSkinMenuSeparator : public CSkinMenuItem
{
	//��������
public:
	//���캯��
	CSkinMenuSeparator() : CSkinMenuItem(MenuItemType_Separator) {}
	//��������
	virtual ~CSkinMenuSeparator() {}
};

//////////////////////////////////////////////////////////////////////////////////

//�˵��ں�
class SKIN_CONTROL_CLASS CSkinMenuKernel : public CMenu
{
	//�˵�����
public:
	static CSkinMenuAttribute	m_SkinAttribute;						//�˵�����

	//��������
public:
	//���캯��
	CSkinMenuKernel();
	//��������
	virtual ~CSkinMenuKernel();

	//���غ���
public:
	//�滭����
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//����λ��
	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

//////////////////////////////////////////////////////////////////////////////////

//����˵��
typedef CWHArray<CSkinMenuItem *>		CMenuItemArray;					//��������
typedef CWHArray<CSkinMenuImage *>		CMenuImageArray;				//��������
typedef CWHArray<CSkinMenuString *>		CMenuStringArray;				//��������
typedef CWHArray<CSkinMenuSeparator *>	CMenuSeparatorArray;			//��������

//����˵�
class SKIN_CONTROL_CLASS CSkinMenu
{
	//�ں˱���
protected:
	CMenuItemArray					m_MenuItemActive;					//��������
	CSkinMenuKernel					m_SkinMenuKernel;					//�˵��ں�

	//��̬����
protected:
	static CMenuImageArray			m_MenuItemImage;					//ͼ������
	static CMenuStringArray			m_MenuItemString;					//�ַ�����
	static CMenuSeparatorArray		m_MenuItemSeparator;				//�������

	//��������
public:
	//���캯��
	CSkinMenu();
	//��������
	virtual ~CSkinMenu();

	//������
public:
	//�����˵�
	bool CreateMenu();
	//���ٲ˵�
	bool DestroyMenu();
	//�����˵�
	bool TrackPopupMenu(CWnd * pWnd);
	//�����˵�
	bool TrackPopupMenu(INT nXPos, INT nYPos, CWnd * pWnd);

	//���Ӻ���
public:
	//������
	bool AppendSeparator();
	//����λͼ
	bool AppendMenu(UINT nMenuID, HBITMAP hBitmap, UINT nFlags=0);
	//�����ַ�
	bool AppendMenu(UINT nMenuID, LPCTSTR pszString, UINT nFlags=0);

	//���뺯��
public:
	//������
	bool InsertSeparator(UINT nPosition);
	//����λͼ
	bool InsertMenu(UINT nMenuID, HBITMAP hBitmap, UINT nPosition, UINT nFlags=0);
	//�����ַ�
	bool InsertMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition, UINT nFlags=0);

	//�޸ĺ���
public:
	//ɾ���˵�
	bool RemoveMenu(UINT nPosition, UINT nFlags);
	//�޸Ĳ˵�
	bool ModifyMenu(UINT nMenuID, HBITMAP hBitmap, UINT nPosition, UINT nFlags=0);
	//�޸Ĳ˵�
	bool ModifyMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition, UINT nFlags=0);

	//״������
public:
	//����Ĭ��
	bool SetDefaultItem(UINT uItem, BOOL fByPos);
	//ѡ�����
	UINT CheckMenuItem(UINT nMenuID, UINT nCheck);
	//���ÿ���
	UINT EnableMenuItem(UINT nMenuID, UINT nEnable);

	//��������
public:
	//�˵����
	HMENU GetSafeHMenu() { return m_SkinMenuKernel; }
	//�˵����
	operator HMENU () { return (HMENU)m_SkinMenuKernel; }

	//�ڲ�����
private:
	//�ͷ�����
	VOID FreeMenuItem(CSkinMenuItem * pSkinMenuItem);
	//��ȡ����
	CSkinMenuItem * AcitveMenuItem(enMenuItemType MenuItemType);
};

//////////////////////////////////////////////////////////////////////////////////

#endif