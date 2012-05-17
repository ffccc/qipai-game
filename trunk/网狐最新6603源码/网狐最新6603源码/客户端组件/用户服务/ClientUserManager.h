#ifndef CLIENT_USER_MANAGER_HEAD_FILE
#define CLIENT_USER_MANAGER_HEAD_FILE

#pragma once

#include "UserServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CClientUserItem;

//用户数组
typedef CWHArray<CClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////////////

//用户信息
class CClientUserItem : public IClientUserItem
{
	//友元定义
	friend class CGameUserManager;
	friend class CPlazaUserManager;

	//属性变量
protected:
	tagUserInfo						m_UserInfo;							//用户信息
	tagCustomFaceInfo				m_CustomFaceInfo;					//自定头像

	//扩展属性
protected:
	BYTE							m_cbCompanion;						//用户关系
	TCHAR							m_szUserNote[LEN_USERNOTE];			//用户备注

	//函数定义
public:
	//构造函数
	CClientUserItem();
	//析构函数
	virtual ~CClientUserItem();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//属性接口
public:
	//重要等级
	virtual LONG GetImportOrder();
	//用户信息
	virtual tagUserInfo * GetUserInfo() { return &m_UserInfo; }
	//自定头像
	virtual tagCustomFaceInfo * GetCustomFaceInfo() { return &m_CustomFaceInfo; }

	//头像信息
public:
	//头像索引
	virtual WORD GetFaceID() { return m_UserInfo.wFaceID; }
	//自定索引
	virtual DWORD GetCustomID() { return m_UserInfo.dwCustomID; }

	//属性信息
public:
	//用户性别
	virtual BYTE GetGender() { return m_UserInfo.cbGender; }
	//用户标识
	virtual DWORD GetUserID() { return m_UserInfo.dwUserID; }
	//游戏标识
	virtual DWORD GetGameID() { return m_UserInfo.dwGameID; }
	//社团标识
	virtual DWORD GetGroupID() { return m_UserInfo.dwGroupID; }
	//用户昵称
	virtual LPCTSTR GetNickName() { return m_UserInfo.szNickName; }
	//社团名字
	virtual LPCTSTR GetGroupName() { return m_UserInfo.szGroupName; }
	//个性签名
	virtual LPCTSTR GetUnderWrite() { return m_UserInfo.szUnderWrite; }

	//经验魅力
public:
	//用户奖牌
	virtual DWORD GetUserMedal() { return m_UserInfo.dwUserMedal; }
	//经验数值
	virtual DWORD GetUserExperience() { return m_UserInfo.dwExperience; }
	//魅力数值
	virtual DWORD GetUserLoveLiness() { return m_UserInfo.dwLoveLiness; }

	//等级信息
public:
	//会员等级
	virtual BYTE GetMemberOrder() { return m_UserInfo.cbMemberOrder; }
	//管理等级
	virtual BYTE GetMasterOrder() { return m_UserInfo.cbMasterOrder; }

	//用户状态
public:
	//用户桌子
	virtual WORD GetTableID() { return m_UserInfo.wTableID; }
	//用户椅子
	virtual WORD GetChairID() { return m_UserInfo.wChairID; }
	//用户状态
	virtual BYTE GetUserStatus() { return m_UserInfo.cbUserStatus; }

	//积分信息
public:
	//积分数值
	virtual SCORE GetUserScore() { return m_UserInfo.lScore; }
	//成绩数值
	virtual SCORE GetUserGrade() { return m_UserInfo.lGrade; }
	//银行数值
	virtual SCORE GetUserInsure() { return m_UserInfo.lInsure; }

	//游戏信息
public:
	//胜利盘数
	virtual DWORD GetUserWinCount() { return m_UserInfo.dwWinCount; }
	//失败盘数
	virtual DWORD GetUserLostCount() { return m_UserInfo.dwLostCount; }
	//和局盘数
	virtual DWORD GetUserDrawCount() { return m_UserInfo.dwDrawCount; }
	//逃跑盘数
	virtual DWORD GetUserFleeCount() { return m_UserInfo.dwFleeCount; }
	//游戏局数
	virtual DWORD GetUserPlayCount() { return m_UserInfo.dwWinCount+m_UserInfo.dwLostCount+m_UserInfo.dwDrawCount+m_UserInfo.dwFleeCount; }

	//比率信息
public:
	//用户胜率
	virtual float GetUserWinRate();
	//用户输率
	virtual float GetUserLostRate();
	//用户和率
	virtual float GetUserDrawRate();
	//用户逃率
	virtual float GetUserFleeRate();

	//用户关系
public:
	//获取关系
	virtual BYTE GetUserCompanion() { return m_cbCompanion; }
	//设置关系
	virtual VOID SetUserCompanion(BYTE cbCompanion) { m_cbCompanion=cbCompanion; }

	//用户备注
public:
	//设置备注
	virtual VOID SetUserNoteInfo(LPCTSTR pszUserNote);
	//获取备注
	virtual LPCTSTR GetUserNoteInfo() { return m_szUserNote; }
};

//////////////////////////////////////////////////////////////////////////////////

//用户管理
class CPlazaUserManager : public IPlazaUserManager
{
	//变量定义
protected:
	CClientUserItemArray			m_UserItemActive;					//活动数组
	static CClientUserItemArray		m_UserItemStorage;					//存储数组

	//组件接口
protected:
	IUserManagerSink *				m_pIUserManagerSink;				//通知接口
	IUserInformation *				m_pIUserInformation;				//用户信息

	//函数定义
public:
	//构造函数
	CPlazaUserManager();
	//析构函数
	virtual ~CPlazaUserManager();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//设置接口
	virtual bool SetUserInformation(IUnknownEx * pIUnknownEx);
	//设置接口
	virtual bool SetUserManagerSink(IUnknownEx * pIUnknownEx);

	//管理接口
public:
	//删除用户
	virtual bool DeleteUserItem(IClientUserItem * pIClientUserItem);
	//增加用户
	virtual IClientUserItem * ActiveUserItem(tagUserInfo & UserInfo, tagCustomFaceInfo & CustomFaceInfo);

	//更新接口
public:
	//更新积分
	virtual bool UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore);
	//更新状态
	virtual bool UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus);
	//更新头像
	virtual bool UpdateUserCustomFace(IClientUserItem * pIClientUserItem, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);

	//查找接口
public:
	//枚举用户
	virtual IClientUserItem * EnumUserItem(WORD wEnumIndex);
	//查找用户
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID);
	//查找用户
	virtual IClientUserItem * SearchUserByGameID(DWORD dwGameID);
	//查找用户
	virtual IClientUserItem * SearchUserByNickName(LPCTSTR pszNickName);
};

//////////////////////////////////////////////////////////////////////////////////

//用户管理
class CGameUserManager : public IGameUserManager
{
	//用户变量
protected:
	CClientUserItemArray			m_UserItemLookon;					//旁观数组
	CClientUserItemArray			m_UserItemStorage;					//存储数组

	//变量定义
protected:
	CClientUserItem *				m_pTableUserItem[MAX_CHAIR];		//游戏用户

	//组件接口
protected:
	IUserManagerSink *				m_pIUserManagerSink;				//通知接口

	//函数定义
public:
	//构造函数
	CGameUserManager();
	//析构函数
	virtual ~CGameUserManager();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//设置接口
	virtual bool SetUserManagerSink(IUnknownEx * pIUnknownEx);
	//获取接口
	virtual VOID * GetUserManagerSink(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//重置用户
	virtual bool ResetUserItem();
	//删除用户
	virtual bool DeleteUserItem(IClientUserItem * pIClientUserItem);
	//增加用户
	virtual IClientUserItem * ActiveUserItem(tagUserInfo & UserInfo, tagCustomFaceInfo & CustomFaceInfo, BYTE cbCompanion, LPCTSTR pszUserNote);

	//更新接口
public:
	//更新积分
	virtual bool UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore);
	//更新状态
	virtual bool UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus);
	//更新头像
	virtual bool UpdateUserCustomFace(IClientUserItem * pIClientUserItem, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);

	//查询接口
public:
	//游戏用户
	virtual IClientUserItem * GetTableUserItem(WORD wChariID);
	//旁观用户
	virtual IClientUserItem * EnumLookonUserItem(WORD wEnumIndex);

	//查找接口
public:
	//查找用户
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID);
	//查找用户
	virtual IClientUserItem * SearchUserByGameID(DWORD dwGameID);
	//查找用户
	virtual IClientUserItem * SearchUserByNickName(LPCTSTR pszNickName);
};

//////////////////////////////////////////////////////////////////////////////////

#endif