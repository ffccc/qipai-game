#ifndef SERVER_LIST_VIEW_HEAD_FILE
#define SERVER_LIST_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//����˵��
typedef CMap<WORD,WORD,UINT,UINT>	CGameImageMap;						//��Ϸͼ��

//////////////////////////////////////////////////////////////////////////////////

//�����б�
class CServerListView : public CTreeCtrl, public IServerListDataSink
{
	//λ�ñ���
protected:
	INT								m_nXScroll;							//����ƫ��
	INT								m_nYScroll;							//����ƫ��

	//�б���
protected:
	HTREEITEM						m_TreeListRoot;						//������
	HTREEITEM						m_TreeAssistant;					//�������

	//��Դ����
protected:
	CFont							m_FontBold;							//��������
	CPngImage						m_ImageArrow;						//��ͷͼ��
	CSkinScrollBar					m_SkinScrollBar;					//��������

	//��Դ����
protected:
	CImageList						m_ImageList;						//ͼƬ��Դ
	CGameImageMap					m_GameImageMap;						//ͼ������

	//��������
protected:
	HTREEITEM						m_hItemMouseHover;					//��������
	HTREEITEM						m_hTreeClickExpand;					//��������

	//�������
protected:

	//��������
public:
	//���캯��
	CServerListView();
	//��������
	virtual ~CServerListView();

	//״̬֪ͨ
public:
	//��ȡ֪ͨ
	virtual VOID OnGameItemFinish();
	//��ȡ֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID);

	//����֪ͨ
public:
	//����֪ͨ
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//���ں���
	virtual LRESULT DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//����ͨ��
	VOID InitAssistantItem();
	//���ú���
	VOID InitServerTreeView();

	//���ܺ���
public:
	//��ȡѡ��
	HTREEITEM GetCurrentSelectItem(bool bOnlyText);

	//չ������
public:
	//չ���ж�
	bool ExpandVerdict(HTREEITEM hTreeItem);
	//չ���б�
	bool ExpandListItem(HTREEITEM hTreeItem);
	//չ���б�
	bool ExpandListItem(CGameListItem * pGameListItem);

	//�滭����
private:
	//�滭����
	VOID DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox);
	//�滭����
	VOID DrawTreeBack(CDC * pDC, CRect & rcClient, CRect & rcClipBox);

	//�滭����
private:
	//����ͼ��
	VOID DrawListImage(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem);
	//�����ı�
	VOID DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected);

	//ͼ�꺯��
private:
	//��ȡͼ��
	UINT GetGameImageIndex(CGameKindItem * pGameKindItem);
	//��ȡͼ��
	UINT GetGameImageIndex(CGameServerItem * pGameServerItem);

	//���⺯��
private:
	//��ȡ����
	LPCTSTR GetGameItemTitle(CGameKindItem * pGameKindItem, LPTSTR pszTitle, UINT uMaxCount);
	//��ȡ����
	LPCTSTR GetGameItemTitle(CGameServerItem * pGameServerItem, LPTSTR pszTitle, UINT uMaxCount);

	//��������
private:
	//ɾ������
	VOID DeleteUpdateItem(CGameListItem * pGameListItem);
	//�޸�����
	VOID ModifyGameListItem(HTREEITEM hTreeItem, LPCTSTR pszTitle, UINT uImage);
	//��������
	HTREEITEM InsertInsideItem(LPCTSTR pszTitle, UINT uImage, DWORD dwInsideID, HTREEITEM hParentItem);
	//��������
	HTREEITEM InsertGameListItem(LPCTSTR pszTitle, UINT uImage, CGameListItem * pGameListItem, HTREEITEM hParentItem);

	//ϵͳ��Ϣ
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//�б���Ϣ
protected:
	//�Ҽ��б�
	VOID OnNMRClick(NMHDR * pNMHDR, LRESULT * pResult);
	//����б�
	VOID OnNMLClick(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�ı�
	VOID OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�չ��
	VOID OnTvnItemexpanding(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif