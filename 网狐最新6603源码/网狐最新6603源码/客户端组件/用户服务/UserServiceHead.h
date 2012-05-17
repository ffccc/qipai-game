#ifndef USER_SERVICE_HEAD_FILE
#define USER_SERVICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//MFC 文件
#include <Nb30.h>
#include <Afxmt.h>
#include <AtlBase.h>

//平台文件
#include "..\..\全局定义\Platform.h"

//组件文件
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef USER_SERVICE_CLASS
	#ifdef  USER_SERVICE_DLL
		#define USER_SERVICE_CLASS _declspec(dllexport)
	#else
		#define USER_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define USER_SERVICE_DLL_NAME	TEXT("UserService.dll")				//组件名字
#else
	#define USER_SERVICE_DLL_NAME	TEXT("UserServiceD.dll")			//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

//名片定义
#define NAME_CARD_CY				80									//名片高度
#define NAME_CARD_CX				260									//名片宽度

//关系信息
struct tagCompanionInfo
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	BYTE							cbCompanion;						//用户关系
	TCHAR							szUserNote[LEN_USERNOTE];			//用户备注
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IClientUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IClientUserItem={0x7c3908c4,0xa275,0x4abb,0x008b,0x78,0x09,0x4d,0xc9,0xf1,0x29,0x56};
#else
	#define VER_IClientUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IClientUserItem={0x22dfa4e3,0xa6b9,0x4716,0x0086,0x46,0xf8,0xb1,0x3b,0x51,0x2d,0x7b};
#endif

//用户接口
interface IClientUserItem : public IUnknownEx
{
	//属性接口
public:
	//重要等级
	virtual LONG GetImportOrder()=NULL;
	//用户信息
	virtual tagUserInfo * GetUserInfo()=NULL;
	//自定头像
	virtual tagCustomFaceInfo * GetCustomFaceInfo()=NULL;

	//头像信息
public:
	//头像索引
	virtual WORD GetFaceID()=NULL;
	//自定索引
	virtual DWORD GetCustomID()=NULL;

	//属性信息
public:
	//用户性别
	virtual BYTE GetGender()=NULL;
	//用户标识
	virtual DWORD GetUserID()=NULL;
	//游戏标识
	virtual DWORD GetGameID()=NULL;
	//社团标识
	virtual DWORD GetGroupID()=NULL;
	//用户昵称
	virtual LPCTSTR GetNickName()=NULL;
	//社团名字
	virtual LPCTSTR GetGroupName()=NULL;
	//个性签名
	virtual LPCTSTR GetUnderWrite()=NULL;

	//经验魅力
public:
	//用户奖牌
	virtual DWORD GetUserMedal()=NULL;
	//经验数值
	virtual DWORD GetUserExperience()=NULL;
	//魅力数值
	virtual DWORD GetUserLoveLiness()=NULL;

	//等级信息
public:
	//会员等级
	virtual BYTE GetMemberOrder()=NULL;
	//管理等级
	virtual BYTE GetMasterOrder()=NULL;

	//用户状态
public:
	//用户桌子
	virtual WORD GetTableID()=NULL;
	//用户椅子
	virtual WORD GetChairID()=NULL;
	//用户状态
	virtual BYTE GetUserStatus()=NULL;

	//游戏信息
public:
	//积分数值
	virtual SCORE GetUserScore()=NULL;
	//成绩数值
	virtual SCORE GetUserGrade()=NULL;
	//银行数值
	virtual SCORE GetUserInsure()=NULL;

	//游戏信息
public:
	//胜利盘数
	virtual DWORD GetUserWinCount()=NULL;
	//失败盘数
	virtual DWORD GetUserLostCount()=NULL;
	//和局盘数
	virtual DWORD GetUserDrawCount()=NULL;
	//逃跑盘数
	virtual DWORD GetUserFleeCount()=NULL;
	//游戏局数
	virtual DWORD GetUserPlayCount()=NULL;

	//比率信息
public:
	//用户胜率
	virtual float GetUserWinRate()=NULL;
	//用户输率
	virtual float GetUserLostRate()=NULL;
	//用户和率
	virtual float GetUserDrawRate()=NULL;
	//用户逃率
	virtual float GetUserFleeRate()=NULL;

	//用户关系
public:
	//获取关系
	virtual BYTE GetUserCompanion()=NULL;
	//设置关系
	virtual VOID SetUserCompanion(BYTE cbCompanion)=NULL;

	//用户备注
public:
	//获取备注
	virtual LPCTSTR GetUserNoteInfo()=NULL;
	//设置备注
	virtual VOID SetUserNoteInfo(LPCTSTR pszUserNote)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserManagerSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserManagerSink={0x28e4435e,0xdcb8,0x4ad1,0x008a,0x58,0x60,0xe8,0xc4,0xd4,0x94,0xc7};
#else
	#define VER_IUserManagerSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserManagerSink={0x6b48deaf,0x28f0,0x4cd4,0x00bf,0x08,0x0d,0x99,0x93,0x4e,0x5d,0x37};
#endif

//管理通知
interface IUserManagerSink : public IUnknownEx
{
	//用户激活
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem)=NULL;
	//用户删除
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem)=NULL;
	//用户更新
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem)=NULL;
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)=NULL;
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IPlazaUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IPlazaUserManager={0xa2992ab9,0x2fdd,0x4f9f,0x00bb,0xac,0xcf,0x8b,0x60,0x70,0x80,0xcd};
#else
	#define VER_IPlazaUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IPlazaUserManager={0xe733fb10,0x4b3c,0x442e,0x008c,0xc2,0x37,0x42,0x8b,0x67,0x35,0x0b};
#endif

//用户管理
interface IPlazaUserManager : public IUnknownEx
{
	//配置接口
public:
	//设置接口
	virtual bool SetUserInformation(IUnknownEx * pIUnknownEx)=NULL;
	//设置接口
	virtual bool SetUserManagerSink(IUnknownEx * pIUnknownEx)=NULL;

	//管理接口
public:
	//删除用户
	virtual bool DeleteUserItem(IClientUserItem * pIClientUserItem)=NULL;
	//增加用户
	virtual IClientUserItem * ActiveUserItem(tagUserInfo & UserInfo, tagCustomFaceInfo & CustomFaceInfo)=NULL;

	//更新接口
public:
	//更新积分
	virtual bool UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore)=NULL;
	//更新状态
	virtual bool UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus)=NULL;
	//更新头像
	virtual bool UpdateUserCustomFace(IClientUserItem * pIClientUserItem, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)=NULL;

	//查找接口
public:
	//枚举用户
	virtual IClientUserItem * EnumUserItem(WORD wEnumIndex)=NULL;
	//查找用户
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID)=NULL;
	//查找用户
	virtual IClientUserItem * SearchUserByGameID(DWORD dwGameID)=NULL;
	//查找用户
	virtual IClientUserItem * SearchUserByNickName(LPCTSTR pszNickName)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameUserManager={0xb0e334b9,0xf693,0x4c5f,0x0087,0x0f,0x66,0x77,0x0a,0xb9,0x93,0xb2};
#else
	#define VER_IGameUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameUserManager={0x2e5d87ff,0x3150,0x4897,0x008c,0x1f,0xbb,0x3d,0x29,0xfc,0x51,0x2e};
#endif

//用户管理
interface IGameUserManager : public IUnknownEx
{
	//配置接口
public:
	//设置接口
	virtual bool SetUserManagerSink(IUnknownEx * pIUnknownEx)=NULL;
	//获取接口
	virtual VOID * GetUserManagerSink(REFGUID Guid, DWORD dwQueryVer)=NULL;

	//管理接口
public:
	//重置用户
	virtual bool ResetUserItem()=NULL;
	//删除用户
	virtual bool DeleteUserItem(IClientUserItem * pIClientUserItem)=NULL;
	//增加用户
	virtual IClientUserItem * ActiveUserItem(tagUserInfo & UserInfo, tagCustomFaceInfo & CustomFaceInfo, BYTE cbCompanion, LPCTSTR pszUserNote)=NULL;

	//更新接口
public:
	//更新积分
	virtual bool UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore)=NULL;
	//更新状态
	virtual bool UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus)=NULL;
	//更新头像
	virtual bool UpdateUserCustomFace(IClientUserItem * pIClientUserItem, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)=NULL;

	//查询接口
public:
	//游戏用户
	virtual IClientUserItem * GetTableUserItem(WORD wChariID)=NULL;
	//旁观用户
	virtual IClientUserItem * EnumLookonUserItem(WORD wEnumIndex)=NULL;

	//查找接口
public:
	//查找用户
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID)=NULL;
	//查找用户
	virtual IClientUserItem * SearchUserByGameID(DWORD dwGameID)=NULL;
	//查找用户
	virtual IClientUserItem * SearchUserByNickName(LPCTSTR pszNickName)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameLevelParser INTERFACE_VERSION(1,1)
	static const GUID IID_IGameLevelParser={0x12823198,0x27b6,0x46c5,0x009c,0x2c,0x9c,0x37,0x08,0x58,0xbf,0xfa};
#else
	#define VER_IGameLevelParser INTERFACE_VERSION(1,1)
	static const GUID IID_IGameLevelParser={0x05d71a6a,0x4378,0x4213,0x00a1,0x51,0x0f,0x96,0x85,0x22,0x71,0x58};
#endif

//等级解释
interface IGameLevelParser : public IUnknownEx
{
	//配置接口
public:
	//游戏名字
	virtual LPCTSTR GetKindName()=NULL;
	//游戏名字
	virtual VOID SetKindName(LPCTSTR pszKindName)=NULL;

	//描述接口
public:
	//等级描述
	virtual LPCTSTR GetLevelDescribe(SCORE lScore)=NULL;
	//等级描述
	virtual LPCTSTR GetLevelDescribe(IClientUserItem * pIClientUserItem)=NULL;

	//数据接口
public:
	//获取等级
	virtual WORD GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount)=NULL;
	//设置等级
	virtual bool SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount)=NULL;
	//加载等级
	virtual bool LoadGameLevelItem(LPCTSTR pszKindName, LPCTSTR pszDirectory, WORD wGameGenre)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserOrderParser INTERFACE_VERSION(1,1)
	static const GUID IID_IUserOrderParser={0xeff89495,0xe9da,0x4ea3,0x83,0x64,0xaa,0xb2,0xaf,0x7f,0x03,0xce};
#else
	#define VER_IUserOrderParser INTERFACE_VERSION(1,1)
	static const GUID IID_IUserOrderParser={0xac3b2420,0x672a,0x4b8d,0xb4,0xd6,0x9e,0xc7,0xbd,0xb8,0x9d,0x1a};
#endif

//等级接口
interface IUserOrderParser : public IUnknownEx
{
	//配置接口
public:
	//解释数目
	virtual BYTE GetMemberParserItemCount()=NULL;
	//解释数目
	virtual BYTE GetMasterParserItemCount()=NULL;
	//设置解释
	virtual VOID SetMemberParser(tagMemberItem MemberItem[], WORD wItemCount)=NULL;
	//设置解释
	virtual VOID SetMasterParser(tagMasterItem MasterItem[], WORD wItemCount)=NULL;

	//功能接口
public:
	//获取描述
	virtual LPCTSTR GetMemberDescribe(BYTE cbMemberOrder)=NULL;
	//获取描述
	virtual LPCTSTR GetMasterDescribe(BYTE cbMasterOrder)=NULL;
	//获取解释
	virtual BYTE GetMemberParser(tagMemberItem MemberItem[], WORD wItemCount)=NULL;
	//获取解释
	virtual BYTE GetMasterParser(tagMasterItem MasterItem[], WORD wItemCount)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserInformation INTERFACE_VERSION(1,1)
	static const GUID IID_IUserInformation={0xb3bcac27,0x7e21,0x479a,0x00b7,0x49,0xa3,0xdc,0x79,0xfe,0x8d,0xd1};
#else
	#define VER_IUserInformation INTERFACE_VERSION(1,1)
	static const GUID IID_IUserInformation={0x77084681,0xf172,0x4352,0x00a3,0x27,0x11,0x22,0x17,0x03,0x3c,0x47};
#endif

//用户信息
interface IUserInformation : public IUnknownEx
{
	//管理接口
public:
	//加载信息
	virtual bool LoadInformation()=NULL;
	//设置接口
	virtual bool SetUserCompanionSink(IUnknownEx * pIUnknownEx)=NULL;

	//关系接口
public:
	//枚举关系
	virtual tagCompanionInfo * EmunCompanionInfo(WORD wIndex)=NULL;
	//寻找关系
	virtual tagCompanionInfo * SearchCompanionInfo(DWORD dwUserID)=NULL;
	//插入关系
	virtual tagCompanionInfo * InsertCompanionInfo(IClientUserItem * pIClientUserItem, BYTE cbCompanion)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserCompanionSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserCompanionSink={0xd145ece3,0xe8be,0x4178,0x0083,0xa6,0x8f,0x5b,0x12,0x33,0x4c,0x82};
#else
	#define VER_IUserCompanionSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserCompanionSink={0xd687ffa9,0x21ca,0x47ec,0x008a,0x99,0x7d,0xb9,0xec,0xf0,0x6b,0x96};
#endif

//关系回调
interface IUserCompanionSink : public IUnknownEx
{
	//插入通知
	virtual bool OnCompanionInsert(tagCompanionInfo * pCompanionInfo)=NULL;
	//删除通知
	virtual bool OnCompanionDelete(tagCompanionInfo * pCompanionInfo)=NULL;
	//更新通知
	virtual bool OnCompanionUpdate(tagCompanionInfo * pCompanionInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//组件辅助类

DECLARE_MODULE_HELPER(UserOrderParser,USER_SERVICE_DLL_NAME,"CreateUserOrderParser")
DECLARE_MODULE_HELPER(GameLevelParser,USER_SERVICE_DLL_NAME,"CreateGameLevelParser")
DECLARE_MODULE_HELPER(GameUserManager,USER_SERVICE_DLL_NAME,"CreateGameUserManager")
DECLARE_MODULE_HELPER(PlazaUserManager,USER_SERVICE_DLL_NAME,"CreatePlazaUserManager")

//////////////////////////////////////////////////////////////////////////////////

#endif