#ifndef GAME_SERVICE_HEAD_HEAD_FILE
#define GAME_SERVICE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//平台定义
#include "..\..\全局定义\Platform.h"

//消息定义
#include "..\..\消息定义\CMD_Commom.h"
#include "..\..\消息定义\CMD_Correspond.h"
#include "..\..\消息定义\CMD_GameServer.h"

//平台文件
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"
#include "..\..\服务器组件\内核引擎\KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////
//导出定义

//导出定义
#ifndef GAME_SERVICE_CLASS
	#ifdef  GAME_SERVICE_DLL
		#define GAME_SERVICE_CLASS _declspec(dllexport)
	#else
		#define GAME_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define GAME_SERVICE_DLL_NAME	TEXT("GameService.dll")				//组件 DLL 名字
#else
	#define GAME_SERVICE_DLL_NAME	TEXT("GameServiceD.dll")			//组件 DLL 名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//接口说明

//用户接口
interface IServerUserItem;
interface IServerUserManager;
interface IServerUserItemSink;

//代打接口
interface IAndroidUserItem;
interface IAndroidUserManager;
interface IAndroidUserItemSink;

//桌子接口
interface ITableFrame;
interface ITableFrameSink;
interface ITableUserAction;

//服务接口
interface IMainServiceFrame;
interface IGameServiceManager;
interface IGameServiceCustomRule;

//////////////////////////////////////////////////////////////////////////////////
//常量定义

//群发掩码
#define BG_MOBILE					(BYTE)(0x01)						//手机群发
#define BG_COMPUTER					(BYTE)(0x02)						//电脑群发
#define BG_ALL_CLIENT				(BYTE)(0xFF)						//全体群发

//索引定义
#define INDEX_SOCKET				(WORD)(0x0000)						//网络索引
#define INDEX_ANDROID				(WORD)(0x2000)						//机器索引

//创建函数
#define GAME_SERVICE_CREATE_NAME	"CreateGameServiceManager"			//创建函数

//////////////////////////////////////////////////////////////////////////////////
//常量定义

//结束原因
#define GER_NORMAL					0x00								//常规结束
#define GER_DISMISS					0x01								//游戏解散
#define GER_USER_LEAVE				0x02								//用户离开
#define GER_NETWORK_ERROR			0x03								//网络错误

//离开原因
#define LER_NORMAL					0x00								//常规离开
#define LER_SYSTEM					0x01								//系统原因
#define LER_NETWORK					0x02								//网络原因
#define LER_USER_IMPACT				0x03								//用户冲突
#define LER_SERVER_FULL				0x04								//人满为患
#define LER_SERVER_CONDITIONS		0x05								//条件限制

//积分类型
#define SCORE_TYPE_NULL				0x00								//无效积分
#define SCORE_TYPE_WIN				0x01								//胜局积分
#define SCORE_TYPE_LOSE				0x02								//输局积分
#define SCORE_TYPE_DRAW				0x03								//和局积分
#define SCORE_TYPE_FLEE				0x04								//逃局积分
#define SCORE_TYPE_PRESENT			0x10								//赠送积分
#define SCORE_TYPE_SERVICE			0x11								//服务积分

//开始模式
#define START_MODE_ALL_READY		0x00								//所有准备
#define START_MODE_FULL_READY		0x01								//满人开始
#define START_MODE_PAIR_READY		0x02								//配对开始
#define START_MODE_TIME_CONTROL		0x10								//时间控制
#define START_MODE_MASTER_CONTROL	0x11								//管理控制

//分组选项
#define DISTRIBUTE_ALLOW			0x01								//允许分组
#define DISTRIBUTE_IMMEDIATE		0x02								//入座选项
#define DISTRIBUTE_LAST_TABLE		0x04								//同桌选项
#define DISTRIBUTE_SAME_ADDRESS		0x08								//地址选项

//////////////////////////////////////////////////////////////////////////////////
//时间标识

//调度范围
#define IDI_MAIN_MODULE_START		1									//起始标识
#define IDI_MAIN_MODULE_FINISH		99									//终止标识

//机器范围
#define IDI_REBOT_MODULE_START		100									//起始标识
#define IDI_REBOT_MODULE_FINISH		199									//终止标识

//桌子范围
#define IDI_TABLE_MODULE_START		10000								//起始标识
#define IDI_TABLE_MODULE_FINISH		50000								//终止标识

//////////////////////////////////////////////////////////////////////////////////
//时间范围

//游戏时间
#define TIME_TABLE_SINK_RANGE		20									//标识范围
#define TIME_TABLE_KERNEL_RANGE		30									//标识范围
#define TIME_TABLE_MODULE_RANGE		50									//标识范围

//////////////////////////////////////////////////////////////////////////////////
//常量定义

//常量定义
#define RESERVE_USER_COUNT			40L									//保留数目

//系统时间
#define TIME_DISTRIBUTE_ANDROID		15L									//分配用户
#define TIME_REPORT_SERVER_INFO		60L									//上报时间

//辅助时间
#define TIME_LOAD_SYSTEM_MESSAGE	900L								//系统消息
#define TIME_SEND_SYSTEM_MESSAGE	300L								//系统消息

//机器时间
#define TIME_ANDROID_INOUT			5L									//登录时间
#define TIME_ANDROID_PULSE			1L									//脉冲时间
#define TIME_LOAD_ANDROID_INFO		900L								//加载用户
#define TIME_ANDROID_REPOSE_TIME	1800L								//服务时间

//////////////////////////////////////////////////////////////////////////////////
//手机定义

//视图模式
#define	VIEW_MODE_ALL				0x0001								//全部可视
#define	VIEW_MODE_PART				0x0002								//部分可视

//信息模式
#define VIEW_INFO_LEVEL_1			0x0010								//部分信息
#define VIEW_INFO_LEVEL_2			0x0020								//部分信息
#define VIEW_INFO_LEVEL_3			0x0040								//部分信息
#define VIEW_INFO_LEVEL_4			0x0080								//部分信息

//其他配置
#define RECVICE_GAME_CHAT			0x0100								//接收聊天
#define RECVICE_ROOM_CHAT			0x0200								//接收聊天
#define RECVICE_ROOM_WHISPER		0x0400								//接收私聊

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//配置参数
struct tagGameParameter
{
	WORD							wMedalRate;							//奖牌汇率
	WORD							wRevenueRate;						//银行税收
};

//服务属性
struct tagGameServiceAttrib
{
	//内核属性
	WORD							wKindID;							//名称号码
	WORD							wChairCount;						//椅子数目
	WORD							wSupporType;						//支持类型
	TCHAR							szGameName[LEN_KIND];				//游戏名字

	//功能标志
	BYTE							cbAndroidUser;						//机器标志
	BYTE							cbDynamicJoin;						//动态加入
	BYTE							cbOffLineTrustee;					//断线代打

	//服务属性
	DWORD							dwServerVersion;					//游戏版本
	DWORD							dwClientVersion;					//游戏版本
	TCHAR							szDataBaseName[32];					//游戏库名
	TCHAR							szServerDLLName[LEN_PROCESS];		//进程名字
	TCHAR							szClientEXEName[LEN_PROCESS];		//进程名字
};

//服务配置
struct tagGameServiceOption
{
	//挂接属性
	WORD							wKindID;							//挂接类型
	WORD							wNodeID;							//挂接节点
	WORD							wSortID;							//排列标识
	WORD							wServerID;							//房间标识

	//税收配置
	BYTE							cbRevenueRatio;						//税收比例

	//房间配置
	LONG							lCellScore;							//单位积分
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分

	//会员限制
	BYTE							cbMinEnterMember;					//最低会员
	BYTE							cbMaxEnterMember;					//最高会员

	//房间配置
	DWORD							dwServerRule;						//房间规则
	DWORD							dwAttachUserRight;					//附加权限

	//房间属性
	WORD							wMaxPlayer;							//最大数目
	WORD							wTableCount;						//桌子数目
	WORD							wServerPort;						//服务端口
	WORD							wServerType;						//房间类型
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//分组设置
	BYTE							cbDistributeRule;					//分组规则
	WORD							wMinDistributeUser;					//最少人数
	WORD							wMaxDistributeUser;					//最多人数
	WORD							wDistributeTimeSpace;				//分组间隔
	WORD							wDistributeDrawCount;				//分组局数
	WORD							wDistributeStartDelay;				//开始延时

	//连接设置
	TCHAR							szDataBaseAddr[16];					//连接地址
	TCHAR							szDataBaseName[32];					//数据库名

	//自定规则
	BYTE							cbCustomRule[1024];					//自定规则
};

//桌子参数
struct tagTableFrameParameter
{
	//内核组件
	ITimerEngine *					pITimerEngine;						//时间引擎
	IDataBaseEngine *				pIRecordDataBaseEngine;				//数据引擎
	IDataBaseEngine *				pIKernelDataBaseEngine;				//数据引擎

	//服务组件
	IMainServiceFrame *				pIMainServiceFrame;					//服务框架
	IGameServiceManager *			pIGameServiceManager;				//服务管理

	//配置变量
	tagGameParameter *				pGameParameter;						//配置参数
	tagGameServiceAttrib *			pGameServiceAttrib;					//桌子属性
	tagGameServiceOption *			pGameServiceOption;					//配置参数
};

//机器参数
struct tagAndroidUserParameter
{
	//配置变量
	tagGameParameter *				pGameParameter;						//配置参数
	tagGameServiceAttrib *			pGameServiceAttrib;					//桌子属性
	tagGameServiceOption *			pGameServiceOption;					//配置参数

	//内核组件
	ITimerEngine *					pITimerEngine;						//时间引擎
	IServerUserManager *			pIServerUserManager;				//用户管理
	IGameServiceManager *			pIGameServiceManager;				//服务管理
	ITCPNetworkEngineEvent *		pITCPNetworkEngineEvent;			//事件接口
};

//////////////////////////////////////////////////////////////////////////////////
//辅助结构

//积分信息
struct tagScoreInfo
{
	BYTE							cbType;								//积分类型
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lRevenue;							//游戏税收
};

//变更信息
struct tagVariationInfo
{
	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行
	SCORE							lRevenue;							//游戏税收	

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwPlayTimeCount;					//游戏时长

	//全局信息
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//用户经验
	DWORD							dwLoveLiness;						//用户魅力
};

//用户规则
struct tagUserRule
{
	//规则标志
	bool							bLimitSameIP;						//效验地址
	bool							bLimitWinRate;						//限制胜率
	bool							bLimitFleeRate;						//限制断线
	bool							bLimitGameScore;					//限制分数

	//规则属性
	WORD							wMinWinRate;						//最低胜率
	WORD							wMaxFleeRate;						//最高逃率
	LONG							lMaxGameScore;						//最高分数 
	LONG							lMinGameScore;						//最低分数
	TCHAR							szPassword[LEN_PASSWORD];			//桌子密码
};

//用户信息
struct tagUserInfoPlus
{
	//登录信息
	DWORD							dwLogonTime;						//登录时间
	DWORD							dwInoutIndex;						//进出标识

	//连接信息
	WORD							wBindIndex;							//绑定索引
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识

	//附加变量
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限
	SCORE							lRestrictScore;						//限制积分

	//辅助变量
	bool							bMobileUser;						//手机用户
	bool							bAndroidUser;						//机器用户
	TCHAR							szPassword[LEN_MD5];				//桌子密码
};

//请求结果
struct tagRequestResult
{
	BYTE							cbFailureCode;						//失败原因
	TCHAR							szFailureReason[128];				//失败原因
};

//桌子状况
struct tagTableUserInfo
{
	WORD							wMinUserCount;						//最少人数
	WORD							wTableUserCount;					//用户数目
	WORD							wTableReadyCount;					//准备数目
	WORD							wTableAndroidCount;					//机器数目
};

//机器状况
struct tagAndroidUserInfo
{
	WORD							wFreeUserCount;						//用户数目
	WORD							wPlayUserCount;						//用户数目
	WORD							wSitdownUserCount;					//用户数目
	IAndroidUserItem *				pIAndroidUserFree[MAX_ANDROID];		//机器接口
	IAndroidUserItem *				pIAndroidUserPlay[MAX_ANDROID];		//机器接口
	IAndroidUserItem *				pIAndroidUserSitdown[MAX_ANDROID];	//机器接口
};

//机器参数
struct tagAndroidParameter
{
	DWORD							dwUserID;							//用户标识
	SCORE							lMinTakeScore;						//携带积分
	SCORE							lMaxTakeScore;						//携带积分
	DWORD							dwMinPlayDraw;						//游戏局数
	DWORD							dwMaxPlayDraw;						//游戏局数
	DWORD							dwMinReposeTime;					//服务时间
	DWORD							dwMaxReposeTime;					//服务时间
	DWORD							dwServiceGender;					//服务类型
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameServiceManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceManager={0xa975cceb,0x0331,0x4553,0xa1,0xe0,0xa7,0xc7,0x7a,0x7c,0x4e,0xfd};
#else
	#define VER_IGameServiceManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceManager={0x4b2b9d8f,0xce1b,0x44f3,0xa5,0x22,0x65,0x1a,0x65,0xc9,0x0a,0x25};
#endif

//游戏接口
interface IGameServiceManager : public IUnknownEx
{
	//创建接口
public:
	//创建桌子
	virtual VOID * CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)=NULL;
	//创建机器
	virtual VOID * CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)=NULL;
	//创建数据
	virtual VOID * CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer)=NULL;

	//参数接口
public:
	//组件属性
	virtual bool GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)=NULL;
	//调整参数
	virtual bool RectifyParameter(tagGameServiceOption & GameServiceOption)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameServiceCustomRule INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceCustomRule={0x74a43b7d,0x7c29,0x4fb8,0xa5,0x00,0x8e,0x8d,0xff,0x6e,0x2a,0xdd};
#else
	#define VER_IGameServiceCustomRule INTERFACE_VERSION(1,1)
	static const GUID IID_IGameServiceCustomRule={0xc7ace01d,0x75f8,0x4af7,0xb1,0x80,0xa8,0x53,0xcd,0x2e,0x0a,0xb6};
#endif

//配置接口
interface IGameServiceCustomRule : public IUnknownEx
{
	//获取配置
	virtual bool SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)=NULL;
	//默认配置
	virtual bool DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)=NULL;
	//创建窗口
	virtual HWND CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameDataBaseEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IGameDataBaseEngine={0x47dcd531,0x2a19,0x4c0a,0x89,0xb6,0x9f,0xd8,0xe0,0xa7,0x85,0xee};
#else
	#define VER_IGameDataBaseEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IGameDataBaseEngine={0x4310e733,0xc49b,0x4592,0xa3,0xb1,0x73,0x18,0xd1,0x53,0x2f,0x3e};
#endif

//游戏数据
interface IGameDataBaseEngine : public IUnknownEx
{
	//配置参数
public:
	//自定配置
	virtual VOID * GetCustomRule()=NULL;
	//服务属性
	virtual tagGameServiceAttrib * GetGameServiceAttrib()=NULL;
	//服务配置
	virtual tagGameServiceOption * GetGameServiceOption()=NULL;

	//获取对象
public:
	//获取对象
	virtual VOID * GetDataBase(REFGUID Guid, DWORD dwQueryVer)=NULL;
	//获取对象
	virtual VOID * GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer)=NULL;

	//功能接口
public:
	//投递结果
	virtual bool PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameDataBaseEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IGameDataBaseEngineSink={0x9b9111d9,0x7a71,0x41a1,0xae,0x78,0xd4,0xf3,0x20,0x08,0x24,0xdf};
#else
	#define VER_IGameDataBaseEngineSink INTERFACE_VERSION(1,1)
	static const GUID IID_IGameDataBaseEngineSink={0xa6c5e2cc,0x34c1,0x422c,0xa0,0x1b,0x54,0xab,0x68,0xfa,0xe6,0x81};
#endif

//数据接口
interface IGameDataBaseEngineSink : public IUnknownEx
{
	//配置接口
	virtual bool InitializeSink(IUnknownEx * pIUnknownEx)=NULL;
	//数据处理
	virtual bool OnGameDataBaseRequest(DWORD dwUserID, WORD wTableID, WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItem={0xcd43dce8,0x1e12,0x43be,0x8b,0x4f,0x94,0x95,0x92,0xa4,0xf6,0x19};
#else
	#define VER_IServerUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItem={0xb5ce01a7,0x5cd1,0x4788,0x94,0x6c,0xa1,0xef,0x5b,0x30,0x2c,0xb7};
#endif

//用户接口
interface IServerUserItem : public IUnknownEx
{
	//属性信息
public:
	//用户索引
	virtual WORD GetBindIndex()=NULL;
	//用户地址
	virtual DWORD GetClientAddr()=NULL;
	//机器标识
	virtual LPCTSTR GetMachineID()=NULL;

	//登录信息
public:
	//登录时间
	virtual DWORD GetLogonTime()=NULL;
	//记录索引
	virtual DWORD GetInoutIndex()=NULL;

	//用户信息
public:
	//用户信息
	virtual tagUserInfo * GetUserInfo()=NULL;
	//用户规则
	virtual tagUserRule * GetUserRule()=NULL;

	//属性信息
public:
	//用户性别
	virtual BYTE GetGender()=NULL;
	//用户标识
	virtual DWORD GetUserID()=NULL;
	//游戏标识
	virtual DWORD GetGameID()=NULL;
	//用户昵称
	virtual LPCTSTR GetNickName()=NULL;

	//状态接口
public:
	//桌子号码
	virtual WORD GetTableID()=NULL;
	//椅子号码
	virtual WORD GetChairID()=NULL;
	//用户状态
	virtual BYTE GetUserStatus()=NULL;

	//权限信息
public:
	//用户权限
	virtual DWORD GetUserRight()=NULL;
	//管理权限
	virtual DWORD GetMasterRight()=NULL;

	//权限信息
public:
	//用户权限
	virtual BYTE GetMemberOrder()=NULL;
	//管理权限
	virtual BYTE GetMasterOrder()=NULL;

	//积分信息
public:
	//用户积分
	virtual SCORE GetUserScore()=NULL;
	//用户成绩
	virtual SCORE GetUserGrade()=NULL;
	//用户银行
	virtual SCORE GetUserInsure()=NULL;

	//托管信息
public:
	//托管积分
	virtual SCORE GetTrusteeScore()=NULL;
	//锁定积分
	virtual SCORE GetFrozenedScore()=NULL;

	//积分信息
public:
	//用户胜率
	virtual WORD GetUserWinRate()=NULL;
	//用户输率
	virtual WORD GetUserLostRate()=NULL;
	//用户和率
	virtual WORD GetUserDrawRate()=NULL;
	//用户逃率
	virtual WORD GetUserFleeRate()=NULL;
	//游戏局数
	virtual DWORD GetUserPlayCount()=NULL;

	//效验接口
public:
	//对比帐号
	virtual bool ContrastNickName(LPCTSTR pszNickName)=NULL;
	//对比密码
	virtual bool ContrastLogonPass(LPCTSTR pszPassword)=NULL;

	//游戏状态
public:
	//连接状态
	virtual bool IsClientReady()=NULL;
	//设置连接
	virtual VOID SetClientReady(bool bClientReady)=NULL;

	//手机用户
public:
	//控制状态
	virtual bool IsMobileUser()=NULL;
	//设置控制
	virtual VOID SetMobileUser(bool bMobileUser)=NULL;

	//控制用户
public:
	//控制状态
	virtual bool IsAndroidUser()=NULL;
	//设置控制
	virtual VOID SetAndroidUser(bool bbMachineUser)=NULL;

	//记录接口
public:
	//变更判断
	virtual bool IsVariation()=NULL;
	//查询记录
	virtual bool QueryRecordInfo(tagVariationInfo & UserRecordInfo)=NULL;
	//提取变更
	virtual bool DistillVariation(tagVariationInfo & UserVariationInfo)=NULL;

	//管理接口
public:
	//设置状态
	virtual bool SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID)=NULL;
	//写入积分
    virtual bool WriteUserScore(SCORE lScore, SCORE lGrade, SCORE lRevenue, DWORD dwUserMedal, BYTE cbScoreType, DWORD dwPlayTimeCount)=NULL;

	//冻结接口
public:
	//冻结积分
	virtual bool FrozenedUserScore(SCORE lScore)=NULL;
	//解冻积分
	virtual bool UnFrozenedUserScore(SCORE lScore)=NULL;

	//高级接口
public:
	//解除绑定
	virtual bool DetachBindStatus()=NULL;
	//银行操作
	virtual bool ModifyUserInsure(SCORE lScore, SCORE lInsure, SCORE lRevenue)=NULL;
	//设置参数
	virtual bool SetUserParameter(DWORD dwClientAddr, WORD wBindIndex, TCHAR szMachineID[LEN_MACHINE_ID], bool bAndroidUser, bool bClientReady)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserService INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserService={0x2f4e25a9,0xad87,0x4a37,0x98,0x96,0x5c,0x50,0xf9,0x91,0x05,0x31};
#else
	#define VER_IServerUserService INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserService={0xcc4e885f,0x3357,0x4d68,0xb2,0x6b,0x78,0x8f,0x4b,0x3e,0x64,0xc3};
#endif

//用户服务
interface IServerUserService : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItemSink={0x415be3e4,0xd48d,0x4a77,0x94,0xb7,0xd1,0x05,0xcd,0xa3,0x82,0x81};
#else
	#define VER_IServerUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserItemSink={0x9d0cfe02,0x0fe9,0x4a8b,0x97,0x95,0xac,0x32,0x67,0x5a,0xf8,0xb1};
#endif

//状态接口
interface IServerUserItemSink : public IUnknownEx
{
	//用户积分
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem)=NULL;
	//用户状态
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserManager={0xeb413ed6,0x185b,0x4ceb,0xa8,0xbd,0x54,0x74,0x19,0x89,0x6a,0x53};
#else
	#define VER_IServerUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IServerUserManager={0x77a3c4df,0x1d95,0x48c6,0xac,0x9d,0x75,0xd7,0x6c,0x2a,0x3c,0x0e};
#endif

//用户管理
interface IServerUserManager : public IUnknownEx
{
	//配置接口
public:
	//设置接口
	virtual bool SetServerUserItemSink(IUnknownEx * pIUnknownEx)=NULL;

	//查找接口
public:
	//枚举用户
	virtual IServerUserItem * EnumUserItem(WORD wEnumIndex)=NULL;
	//查找用户
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID)=NULL;
	//查找用户
	virtual IServerUserItem * SearchUserItem(LPCTSTR pszNickName)=NULL;

	//统计接口
public:
	//在线人数
	virtual DWORD GetUserItemCount()=NULL;

	//管理接口
public:
	//删除用户
	virtual bool DeleteUserItem()=NULL;
	//删除用户
	virtual bool DeleteUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//插入用户
	virtual bool InsertUserItem(IServerUserItem * * pIServerUserResult, tagUserInfo & UserInfo, tagUserInfoPlus & UserInfoPlus)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrame={0x2e577d5f,0x1e01,0x44ff,0x9f,0xf4,0x01,0x16,0x23,0x1b,0x76,0x15};
#else
	#define VER_ITableFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrame={0x860cd1cf,0x1a4f,0x4e35,0xb8,0x0c,0xd2,0x46,0xa4,0xef,0xfd,0xfb};
#endif

//桌子接口
interface ITableFrame : public IUnknownEx
{
	//属性接口
public:
	//桌子号码
	virtual WORD GetTableID()=NULL;
	//游戏人数
	virtual WORD GetChairCount()=NULL;

	//配置参数
public:
	//自定配置
	virtual VOID * GetCustomRule()=NULL;
	//服务属性
	virtual tagGameServiceAttrib * GetGameServiceAttrib()=NULL;
	//服务配置
	virtual tagGameServiceOption * GetGameServiceOption()=NULL;

	//配置接口
public:
	//开始模式
	virtual BYTE GetStartMode()=NULL;
	//开始模式
	virtual VOID SetStartMode(BYTE cbStartMode)=NULL;

	//单元积分
public:
	//单元积分
	virtual LONG GetCellScore()=NULL;
	//单元积分
	virtual VOID SetCellScore(LONG lCellScore)=NULL;

	//信息接口
public:
	//锁定状态
	virtual bool IsTableLocked()=NULL;
	//游戏状态
	virtual bool IsGameStarted()=NULL;
	//游戏状态
	virtual bool IsDrawStarted()=NULL;
	//游戏状态
	virtual bool IsTableStarted()=NULL;

	//状态接口
public:
	//获取状态
	virtual BYTE GetGameStatus()=NULL;
	//设置状态
	virtual VOID SetGameStatus(BYTE bGameStatus)=NULL;

	//控制接口
public:
	//开始游戏
	virtual bool StartGame()=NULL;
	//解散游戏
	virtual bool DismissGame()=NULL;
	//结束游戏
	virtual bool ConcludeGame(BYTE cbGameStatus)=NULL;

	//写分接口
public:
	//写入积分
	virtual bool WriteUserScore(WORD wChairID, SCORE lScore, BYTE cbScoreType)=NULL;
	//写入积分
	virtual bool WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount)=NULL;

	//计算接口
public:
	//计算税收
	virtual SCORE CalculateRevenue(WORD wChairID, SCORE lScore)=NULL;
	//消费限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)=NULL;

	//用户接口
public:
	//寻找用户
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID)=NULL;
	//游戏用户
	virtual IServerUserItem * GetTableUserItem(WORD wChairID)=NULL;
	//旁观用户
	virtual IServerUserItem * EnumLookonUserItem(WORD wEnumIndex)=NULL;

	//时间接口
public:
	//设置时间
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)=NULL;
	//删除时间
	virtual bool KillGameTimer(DWORD dwTimerID)=NULL;

	//网络接口
public:
	//发送数据
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID)=NULL;
	//发送数据
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//发送数据
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID)=NULL;
	//发送数据
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//发送数据
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)=NULL;
	//发送数据
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//功能接口
public:
	//发送消息
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType)=NULL;
	//游戏消息
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)=NULL;
	//房间消息
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)=NULL;

	//功能接口
public:
	//发送场景
	virtual bool SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrameSink={0x9476b154,0x8beb,0x4f7e,0xaf,0x64,0xd2,0xb1,0x1a,0xda,0x5e,0xc4};
#else
	#define VER_ITableFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableFrameSink={0x38a74df5,0x6245,0x46c7,0xb6,0xce,0x53,0xf9,0xd5,0xbf,0x6d,0xe6};
#endif

//回调接口
interface ITableFrameSink : public IUnknownEx
{
	//管理接口
public:
	//复位接口
	virtual VOID RepositionSink()=NULL;
	//配置接口
	virtual bool Initialization(IUnknownEx * pIUnknownEx)=NULL;

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)=NULL;
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart()=NULL;
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)=NULL;
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)=NULL;

	//事件接口
public:
	//时间事件
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter)=NULL;
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)=NULL;
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)=NULL;

	//网络接口
public:
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
	//框架消息
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableUserAction INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserAction={0x0f9aa3f9,0xdba4,0x49cb,0x88,0x4f,0xd9,0x11,0xaf,0x24,0xfb,0x8d};
#else
	#define VER_ITableUserAction INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserAction={0xc97c060b,0xcf0e,0x40b7,0x93,0x30,0x97,0xa4,0xf6,0x8c,0xca,0x84};
#endif

//用户动作
interface ITableUserAction : public IUnknownEx
{
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//用户起来
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableUserRequest INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserRequest={0x7a810ebe,0x3835,0x41b5,0xba,0x7e,0x02,0x97,0x8c,0x13,0x73,0x50};
#else
	#define VER_ITableUserRequest INTERFACE_VERSION(1,1)
	static const GUID IID_ITableUserRequest={0x7ad17e89,0xcb5b,0x472a,0xac,0xeb,0x84,0x4d,0x4f,0xa1,0x4c,0x38};
#endif

//用户请求
interface ITableUserRequest : public IUnknownEx
{
	//旁观请求
	virtual bool OnUserRequestLookon(WORD wChairID, IServerUserItem * pIServerUserItem, tagRequestResult & RequestResult)=NULL;
	//坐下请求
	virtual bool OnUserRequestSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, tagRequestResult & RequestResult)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IMainServiceFrame INTERFACE_VERSION(1,1)
	static const GUID IID_IMainServiceFrame={0xef3efa64,0x788b,0x4299,0x80,0x99,0xdd,0xed,0x08,0xde,0x57,0xc1};
#else
	#define VER_IMainServiceFrame INTERFACE_VERSION(1,1)
	static const GUID IID_IMainServiceFrame={0xbaaf5584,0xf9b4,0x41b6,0xae,0x6b,0xef,0x4d,0x54,0x41,0xf8,0x32};
#endif

//服务框架
interface IMainServiceFrame : public IUnknownEx
{
	//消息接口
public:
	//房间消息
	virtual bool SendRoomMessage(LPCTSTR lpszMessage, WORD wType)=NULL;
	//游戏消息
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType)=NULL;
	//房间消息
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)=NULL;
	//游戏消息
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)=NULL;

	//网络接口
public:
	//发送数据
	virtual bool SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//发送数据
	virtual bool SendData(DWORD dwContextID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//发送数据
	virtual bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAndroidUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItem={0xf6856fe1,0xc93e,0x4166,0xbc,0x92,0xf7,0x43,0xad,0x97,0x1c,0xa8};
#else
	#define VER_IAndroidUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItem={0xb1faa2f4,0x9804,0x4c6f,0x9d,0xfc,0xb1,0x0a,0x08,0x8a,0x22,0x69};
#endif

//机器人接口
interface IAndroidUserItem : public IUnknownEx
{
	//信息接口
public:
	//获取 I D
	virtual DWORD GetUserID()=NULL;
	//桌子号码
	virtual WORD GetTableID()=NULL;
	//椅子号码
	virtual WORD GetChairID()=NULL;

	//状态函数
public:
	//获取状态
	virtual BYTE GetGameStatus()=NULL;
	//设置状态
	virtual VOID SetGameStatus(BYTE cbGameStatus)=NULL;

	//配置信息
public:
	//获取配置
	virtual tagAndroidParameter * GetAndroidParameter()=NULL;

	//功能接口
public:
	//获取自己
	virtual IServerUserItem * GetMeUserItem()=NULL;
	//游戏用户
	virtual IServerUserItem * GetTableUserItem(WORD wChariID)=NULL;

	//银行接口
public:
	//存入金币
	virtual bool PerformSaveScore(SCORE lScore)=NULL;
	//提取金币
	virtual bool PerformTakeScore(SCORE lScore)=NULL;

	//网络接口
public:
	//发送函数
	virtual bool SendSocketData(WORD wSubCmdID)=NULL;
	//发送函数
	virtual bool SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;

	//功能接口
public:
	//删除时间
	virtual bool KillGameTimer(UINT nTimerID)=NULL;
	//设置时间
	virtual bool SetGameTimer(UINT nTimerID, UINT nElapse)=NULL;
	//发送准备
	virtual bool SendUserReady(VOID * pData, WORD wDataSize)=NULL;
	//发送聊天
	virtual bool SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crFontColor)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAndroidUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItemSink={0x0967632c,0x93da,0x4f7f,0x98,0xe4,0x6f,0x9f,0xf2,0xca,0x7b,0xc4};
#else
	#define VER_IAndroidUserItemSink INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserItemSink={0x1e8a1918,0x572b,0x453b,0xbc,0x0b,0x6b,0x61,0x70,0xa3,0x3c,0xca};
#endif

//机器人接口
interface IAndroidUserItemSink : public IUnknownEx
{
	//控制接口
public:
	//重置接口
	virtual bool RepositionSink()=NULL;
	//初始接口
	virtual bool Initialization(IUnknownEx * pIUnknownEx)=NULL;

	//游戏事件
public:
	//时间消息
	virtual bool OnEventTimer(UINT nTimerID)=NULL;
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//游戏消息
	virtual bool OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)=NULL;

	//用户事件
public:
	//用户进入
	virtual VOID OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//用户离开
	virtual VOID OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//用户积分
	virtual VOID OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
	//用户状态
	virtual VOID OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IAndroidUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserManager={0x0c963240,0xa798,0x4e33,0x81,0x28,0x97,0x3d,0x05,0xe7,0x7f,0x89};
#else
	#define VER_IAndroidUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IAndroidUserManager={0xba43054d,0x924b,0x4013,0xb2,0x6d,0xa6,0x91,0xb6,0x20,0x23,0xb2};
#endif

//机器人接口
interface IAndroidUserManager : public IUnknownEx
{
	//控制接口
public:
	//启动服务
	virtual bool StartService()=NULL;
	//停止服务
	virtual bool ConcludeService()=NULL;

	//状态接口
public:
	//用户状况
	virtual WORD GetAndroidUserInfo(tagAndroidUserInfo & AndroidUserInfo)=NULL;

	//配置接口
public:
	//配置组件
	virtual bool InitAndroidUser(tagAndroidUserParameter & AndroidUserParameter)=NULL;
	//设置库存
	virtual bool SetAndroidStock(tagAndroidParameter AndroidParameter[], WORD wStockCount)=NULL;

	//管理接口
public:
	//删除机器
	virtual bool DeleteAndroidUserItem(DWORD dwAndroidID)=NULL;
	//查找机器
	virtual IAndroidUserItem * SearchAndroidUserItem(DWORD dwUserID, DWORD dwContextID)=NULL;
	//创建机器
	virtual IAndroidUserItem * CreateAndroidUserItem(tagAndroidParameter & AndroidParameter)=NULL;

	//事件接口
public:
	//脉冲事件
	virtual bool OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)=NULL;

	//网络接口
public:
	//发送数据
	virtual bool SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//发送数据
	virtual bool SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//发送数据
	virtual bool SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//包含文件
#ifndef GAME_SERVICE_DLL
	#include "AndroidUserItem.h"
	#include "ServerUserManager.h"
	#include "AndroidUserManager.h"
	#include "GamePropertyManager.h"
#endif

//游戏服务
DECLARE_MODULE_DYNAMIC(GameServiceManager)
DECLARE_MODULE_DYNAMIC(AndroidUserItemSink)

//////////////////////////////////////////////////////////////////////////////////

#endif