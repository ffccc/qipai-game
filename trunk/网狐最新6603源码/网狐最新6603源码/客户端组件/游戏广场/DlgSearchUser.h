#ifndef DLG_SEARCH_USER_HEAD_FILE
#define DLG_SEARCH_USER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerViewItem.h"

//////////////////////////////////////////////////////////////////////////////////

//本地数据
struct tagNativeListData
{
	CServerViewItem *				pServerViewItem;					//房间接口
	IClientUserItem *				pIClientUserItem;					//用户指针
	IGameLevelParser *				pIGameLevelParser;					//等级接口
};

//远程数据
struct tagRemoteListData
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//辅助信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//位置信息
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	TCHAR							szGameKind[LEN_KIND];				//类型位置
	TCHAR							szGameServer[LEN_SERVER];			//房间位置
};

//数组定义
typedef CWHArray<tagNativeListData *> CNativeListDataArray;				//本地数据
typedef CWHArray<tagRemoteListData *> CRemoteListDataArray;				//远程数据

//////////////////////////////////////////////////////////////////////////////////

//本地列表
class CUserListNative : public CUserListControl
{
	//变量定义
protected:
	CNativeListDataArray			m_NativeListDataActive;				//本地数据
	CNativeListDataArray			m_NativeListDataBuffer;				//本地数据

	//函数定义
public:
	//构造函数
	CUserListNative();
	//析构函数
	virtual ~CUserListNative();

	//重载函数
protected:
	//控件绑定
	virtual VOID PreSubclassWindow();

	//重载函数
protected:
	//用户解释
	virtual IClientUserItem * GetClientUserItem(VOID * pItemData);
	//描述字符
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);

	//插入函数
public:
	//插入数据
	VOID InsertUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);
	//更新数据
	VOID UpdateUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);
	//删除数据
	VOID DeleteUserItem(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);

	//辅助函数
protected:
	//获取数据
	tagNativeListData * CreateNativeListData();
	//删除数据
	VOID DeleteNativeListData(tagNativeListData * pNativeListData);
};

//////////////////////////////////////////////////////////////////////////////////

//远程列表
class CUserListRemote : public CSkinListCtrl
{
	//列表信息
protected:
	WORD							m_wColumnCount;						//列表数目
	BYTE							m_cbDataDescribe[MAX_COLUMN];		//数据描述

	//资源变量
protected:
	CImageList						m_ImageUserStatus;					//状态位图

	//变量定义
protected:
	CRemoteListDataArray			m_RemoteListDataActive;				//本地数据
	CRemoteListDataArray			m_RemoteListDataBuffer;				//本地数据

	//函数定义
public:
	//构造函数
	CUserListRemote();
	//析构函数
	virtual ~CUserListRemote();

	//重载函数
protected:
	//控件绑定
	virtual VOID PreSubclassWindow();

	//重载函数
protected:
	//排列数据
	virtual INT SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort);
	//描述字符
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);
	//绘画数据
	virtual VOID DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect & rcSubItem, INT nColumnIndex);

	//插入函数
public:
	//插入数据
	VOID InsertUserItem(tagUserRemoteInfo * pUserRemoteInfo, LPCTSTR pszKindName);

	//辅助函数
protected:
	//获取数据
	tagRemoteListData * CreateRemoteListData();
	//删除数据
	VOID DeleteRemoteListData(tagRemoteListData * pRemoteListData);
};

//////////////////////////////////////////////////////////////////////////////////

//查找用户
class CDlgSearchUser : public CSkinDialog, public CMissionItem
{
	//变量定义
protected:
	DWORD							m_dwGameID;							//游戏标识
	TCHAR							m_szNickName[LEN_NICKNAME];			//用户昵称

	//按钮变量
protected:
	CSkinButton						m_btOk;								//搜索按钮
	CSkinButton						m_btCancel;							//关闭按钮

	//控件变量
protected:
	CSkinEdit						m_edGameID;							//游戏标识
	CSkinEdit						m_edNickName;						//用户昵称
	CSkinComboBox					m_ComSearchArea;					//房间列表

	//模式按钮
protected:
	CButton							m_SearchRemote;						//远程模式
	CButton							m_SearchByGameID;					//标识模式

	//控件变量
protected:
	CStatic							m_StaticImage;						//图像控件
	CUserListRemote					m_UserListRemote;					//用户列表
	CUserListNative					m_UserListNative;					//用户列表

	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理

	//静态变量
protected:
	static CDlgSearchUser *			m_pDlgSearchUser;					//对象指针

	//函数定义
public:
	//构造函数
	CDlgSearchUser();
	//析构函数
	virtual ~CDlgSearchUser();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//重载函数
protected:
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//任务事件
private:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//事件函数
public:
	//房间关闭
	VOID OnViewItemDelete(CServerViewItem * pServerViewItem);
	//用户删除
	VOID OnUserItemDelete(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);
	//用户更新
	VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, CServerViewItem * pServerViewItem);

	//辅助函数
protected:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//静态函数
public:
	//获取对象
	static CDlgSearchUser * GetInstance() { return m_pDlgSearchUser; }

	//消息函数
protected:
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);

	//消息函数
protected:
	//远程查找
	VOID OnBnClickedSearchRemote();
	//标识模式
	VOID OnBnClickedSearchByGameID();
	//右键列表
	VOID OnNMRclickNativeList(NMHDR * pNMHDR, LRESULT * pResult);
	//双击列表
	VOID OnNMDblclkNativeList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif