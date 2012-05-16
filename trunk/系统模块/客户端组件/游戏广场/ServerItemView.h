#ifndef SERVER_ITEM_VIEW_HEAD_FILE
#define SERVER_ITEM_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�б���������
enum enItemGenre
{
	ItemGenre_Type,						//��Ϸ����
	ItemGenre_Kind,						//��Ϸ����
	ItemGenre_Station,					//��Ϸվ��
	ItemGenre_Server,					//��Ϸ����
	ItemGenre_Inside,					//�ڲ���Ϣ
};

//����״̬
enum enServerState
{
	ServerState_Normal,					//��ͨ״̬
	ServerState_InUse,					//����ʹ��
	ServerState_EverUse,				//����ʹ��
};

//�ڲ��ṹ
struct tagGameInside
{
	DWORD								dwSortID;						//�������
	int									iImageIndex;					//��Դ����
	char								szDisplayName[64];				//��ʾ����
};

//ͼ������
#define IND_ROOT						0								//��������
#define IND_TYPE						1								//��������
#define IND_STATION						2								//վ������
#define IND_KIND_NODOWN					3								//û��������
#define IND_KIND_UNKNOW					4								//δ֪��������
#define IND_SERVER_NORNAL				5								//��ͨ��������
#define IND_SERVER_CURRENT				6								//��ǰ��¼����
#define IND_SERVER_BEENIN				7								//������¼����
#define IND_SERVER_LIKE					8								//ϲ����������
#define IND_SERVER_DISABLE				9								//��ֹ��������
#define IND_INSINDE						10								//�ڲ�����

//////////////////////////////////////////////////////////////////////////

//��˵��
class CListItem;
class CListType;
class CListKind;
class CListServer;
class CListStation;
class CListInside;
typedef CArrayTemplate<CListItem *> CListItemArray;
typedef CArrayTemplate<CListType *> CListTypeArray;
typedef CArrayTemplate<CListKind *> CListKindArray;
typedef CArrayTemplate<CListStation *> CListStationArray;
typedef CArrayTemplate<CListServer *> CListServerArray;
typedef CArrayTemplate<CListInside *> CListInsideArray;

//////////////////////////////////////////////////////////////////////////
//�ӿڶ���

//�б�ص��ӿ�
interface IServerListSink
{
	//չ���б�
	virtual bool __cdecl ExpandListItem(CListItem * pListItem)=NULL;
	//����֪ͨ
	virtual void __cdecl OnListItemUpdate(CListItem * pListItem)=NULL;
	//����֪ͨ
	virtual void __cdecl OnListItemInserted(CListItem * pListItem)=NULL;
};

//���б�ӿ�
interface ITreeCtrlSink
{
	//�������
	virtual void __cdecl OnTreeLeftClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//�Ҽ�����
	virtual void __cdecl OnTreeRightClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//���˫��
	virtual void __cdecl OnTreeLeftDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//�Ҽ�˫��
	virtual void __cdecl OnTreeRightDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//ѡ��ı�
	virtual void __cdecl OnTreeSelectChanged(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
	//����չ��
	virtual void __cdecl OnTreeItemexpanded(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//��������
class CListItem
{
	friend class CServerListManager;

	//��������
protected:
	DWORD_PTR							m_dwData;					//������
    CListItem							* m_pParentItem;			//����ָ��
	CListItemArray						m_ListItemArray;			//��������//?���õ���
	const enItemGenre					m_ItemGenre;				//��������

	//��̬����
protected:
	static IServerListSink				* m_pIServerListSink;		//�ص��ӿ�	

	//��������
protected:
	//���캯��
	CListItem(CListItem * pParentItem, enItemGenre ItemGenre) : m_ItemGenre(ItemGenre)
	{
		m_dwData=0L;
		m_pParentItem=pParentItem;
		if (m_pParentItem!=NULL) m_pParentItem->m_ListItemArray.Add(this);
	}
	//��������
	virtual ~CListItem();

	//���ܺ���
public:
	//��ȡ����
	enItemGenre GetItemGenre() { return m_ItemGenre; }
	//��ȡ����
	CListItem * GetParentItem() { return m_pParentItem; }
	//��ȡ����
	DWORD_PTR GetItemData() { return m_dwData; }
	//���ð���
	void SetItemData(DWORD_PTR dwData) { m_dwData=dwData; } 
	//��ȡ������
	INT_PTR GetItemCount() { return m_ListItemArray.GetCount(); }
	//ö������
	CListItem * EnumChildItem(INT_PTR nIndex);

	//���Һ���
public:
	//��������
	CListType * SearchTypeItem(WORD wTypeID);
	//��������
	CListKind * SearchKindItem(WORD wKindID);
	//��������
	CListStation * SearchStationItem(WORD wKindID, WORD wStationID, bool bDeepSearch);
	//��������
	CListServer * SearchServerItem(WORD wKindID, WORD wServerID, bool bDeepSearch);
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CListType : public CListItem
{
	//��������
public:
	tagGameType						m_GameType;					//��Ϣ�ṹ

	//��������
public:
	//���캯��
	CListType(CListItem * pParentItem) : CListItem(pParentItem,ItemGenre_Type)
	{
		memset(&m_GameType,0,sizeof(m_GameType));
	}
	//��ȡ����
	tagGameType * GetItemInfo() 
	{ 
		return & m_GameType; 
	}
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CListKind : public CListItem
{
	//��������
public:
	bool							m_bInstall;					//��װ��־
	tagGameKind						m_GameKind;					//��Ϣ�ṹ

	//��������
public:
	//���캯��
	CListKind(CListItem * pParentItem) : CListItem(pParentItem,ItemGenre_Kind)
	{
		m_bInstall=false;
		memset(&m_GameKind,0,sizeof(m_GameKind));
	}
	//��ȡ����
	tagGameKind * GetItemInfo() 
	{ 
		return & m_GameKind; 
	}
};

//////////////////////////////////////////////////////////////////////////

//��Ϸվ��
class CListStation : public CListItem
{
	//��������
public:
	CListKind						* m_pListKind;				//������Ϣ
	tagGameStation					m_GameStation;				//��Ϣ�ṹ

	//��������
public:
	//���캯��
	CListStation(CListItem * pParentItem, CListKind * pListKind) : CListItem(pParentItem,ItemGenre_Station)
	{
		ASSERT(pListKind!=NULL);
		m_pListKind=pListKind;
		memset(&m_GameStation,0,sizeof(m_GameStation));
	}
	//��ȡ����
	tagGameStation * GetItemInfo() 
	{ 
		return & m_GameStation; 
	}
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CListServer : public CListItem
{
	//��������
public:
	CListKind						* m_pListKind;				//������Ϣ
	tagGameServer					m_GameServer;				//��Ϣ�ṹ

	//��������
public:
	//���캯��
	CListServer(CListItem * pParentItem, CListKind * pListKind) : CListItem(pParentItem,ItemGenre_Server)
	{
		ASSERT(pListKind!=NULL);
		m_pListKind=pListKind;
		memset(&m_GameServer,0,sizeof(m_GameServer));
	}
	//��ȡ����
	tagGameServer * GetItemInfo()
	{ 
		return & m_GameServer; 
	}
	//��ȡ����
	CListKind * GetListKind() { return m_pListKind; }
};

//////////////////////////////////////////////////////////////////////////

//�ڲ�����
class CListInside : public CListItem
{
	//��������
public:
	tagGameInside					m_GameInside;				//��Ϣ�ṹ

	//��������
public:
	//���캯��
	CListInside(CListItem * pParentItem) : CListItem(pParentItem,ItemGenre_Inside)
	{
		memset(&m_GameInside,0,sizeof(m_GameInside));
	}
	//��ȡ����
	tagGameInside * GetItemInfo() 
	{ 
		return & m_GameInside; 
	}
};

//////////////////////////////////////////////////////////////////////////

//�����б������
class CServerListManager
{
	//��������
protected:
	CListTypeArray					m_PtrArrayType;					//����ָ��
	CListKindArray					m_PtrArrayKind;					//��Ϸָ��
	CListStationArray				m_PtrArrayStation;				//վ��ָ��
	CListServerArray				m_PtrArrayServer;				//����ָ��
	CListInsideArray				m_PtrArrayInside;				//����ָ��

	//�ӿ�ָ��
protected:
	IServerListSink					* m_pIServerListSink;			//�ص��ӿ�	

	//��������
public:
	//���캯��
	CServerListManager();
	//��������
	virtual ~CServerListManager();

	//���ܺ���
public:
	//չ���б�
	bool ExpandListItem(CListItem * pListItem);

	//���ܽӿ�
public:
	//ö��������
	CListType * EnumTypeItem(INT_PTR nIndex);
	//ö����Ϸ��
	CListKind * EnumKindItem(INT_PTR nIndex);
	//ö��վ����
	CListStation * EnumStationItem(INT_PTR nIndex);
	//ö�ٷ�����
	CListServer * EnumServerItem(INT_PTR nIndex);
	//ö���ڲ���
	CListInside * EnumInsideItem(INT_PTR nIndex);
	//��ʼ������
	bool InitServerListManager(IServerListSink * pIServerListSink);

	//����ӿ�
public:
	//��������
	bool InsertTypeItem(tagGameType GameType[], WORD wCount);
	//��������
	bool InsertKindItem(tagGameKind GameKind[], WORD wCount);
	//��������
	bool InsertStationItem(tagGameStation GameStation[], WORD wCount);
	//��������
	bool InsertServerItem(tagGameServer GameServer[], WORD wCount);
	//��������
	bool InsertInsideItem(tagGameInside GameInside[], WORD wCount);

	//���ҽӿ�
public:
	//��������
	CListType * SearchTypeItem(WORD wTypeID);
	//��������
	CListKind * SearchKindItem(WORD wKindID);
	//��������
	CListStation * SearchStationItem(WORD wKindID, WORD wStationID);
	//��������
	CListServer * SearchServerItem(WORD wKindID, WORD wServerID);

	//��������
public:
	//��������
	bool UpdateGameKind(WORD wKindID);
	//��������
	bool UpdateGameOnLineCount(DWORD dwOnLineCount);
	//��������
	bool UpdateGameKindOnLine(WORD wKindID, DWORD dwOnLineCount);
	//��������
	bool UpdateGameServerOnLine(CListServer * pListServer, DWORD dwOnLineCount);
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��Դ��Ϣ
struct tagGameResourceInfo
{
	WORD								wKindID;						//���ͱ�ʶ
	DWORD								dwImageIndex;					//��Դ����
};

//����˵��
typedef CArrayTemplate<tagGameResourceInfo> CGameResourceArray;

//////////////////////////////////////////////////////////////////////////

//�������б���ͼ
class CServerItemView : public CTreeCtrl, public IServerListSink
{
	//���ñ���
protected:
	bool								m_bShowOnLineCount;				//��ʾ����

	//������Ϣ
protected:
	CImageList							m_ImageList;					//ͼƬ��Դ
	CGameResourceArray					m_GameResourceArray;			//��Դ����

	//�ӿڱ���
protected:
	ITreeCtrlSink						* m_pITreeCtrlSink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CServerItemView();
	//��������
	virtual ~CServerItemView();

	//���ܺ���
public:
	//��ʾ����
	void ShowOnLineCount(bool bShowOnLineCount);
	//���ú���
	bool InitServerItemView(ITreeCtrlSink * pITreeCtrlSink);

	//֪ͨ�ӿ�
public:
	//չ���б�
	virtual bool __cdecl ExpandListItem(CListItem * pListItem);
	//����֪ͨ
	virtual void __cdecl OnListItemUpdate(CListItem * pListItem);
	//����֪ͨ
	virtual void __cdecl OnListItemInserted(CListItem * pListItem);

	//��������
private:
	//��ť����
	HTREEITEM GetCurrentTreeItem();

	//ͼ�꺯��
private:
	//��ȡͼ��
	DWORD GetGameImageIndex(CListKind * pListKind);
	//��ȡͼ��
	DWORD GetGameImageIndex(CListServer * pListServer);
	//��ȡͼ��
	DWORD GetGameImageIndex(LPCTSTR pszProcess, WORD wKindID);

	//���⺯��
private:
	//��ȡ����
	LPCTSTR GetGameItemTitle(CListKind * pListKind, LPTSTR pszTitle, WORD wBufferSize);
	//��ȡ����
	LPCTSTR GetGameItemTitle(CListServer * pListServer, LPTSTR pszTitle, WORD wBufferSize);

	//��Ϣӳ��
protected:
	//����б�
	afx_msg void OnNMClick(NMHDR * pNMHDR, LRESULT * pResult);
	//�һ��б�
	afx_msg void OnNMRclick(NMHDR * pNMHDR, LRESULT * pResult);
	//���˫��
	afx_msg void OnNMDblclk(NMHDR * pNMHDR, LRESULT * pResult);
	//�Ҽ�˫��
	afx_msg void OnNMRdblclk(NMHDR * pNMHDR, LRESULT * pResult);
	//ѡ��ı�
	afx_msg void OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�չ��
	afx_msg void OnTvnItemexpanded(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif