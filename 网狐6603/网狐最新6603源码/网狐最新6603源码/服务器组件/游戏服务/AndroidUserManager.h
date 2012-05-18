#ifndef ANDROID_USER_MANAGER_HEAD_FILE
#define ANDROID_USER_MANAGER_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "AndroidUserItem.h"
#include "GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//数组数组
typedef CWHArray<CAndroidUserItem *> CAndroidUserItemArray;
typedef CMap<DWORD,DWORD,CAndroidUserItem *, CAndroidUserItem *> CAndroidUserItemMap;

//////////////////////////////////////////////////////////////////////////////////

//机器人管理
class GAME_SERVICE_CLASS CAndroidUserManager : public IAndroidUserManager
{
	//配置变量
protected:
	WORD							m_wAutoAndroidCount;				//自动数目

	//库存变量
protected:
	WORD							m_wStockCount;						//库存数目
	tagAndroidParameter				m_AndroidParameter[MAX_ANDROID];	//库存用户

	//组件接口
protected:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	IServerUserManager *			m_pIServerUserManager;				//用户管理
	IGameServiceManager *			m_pIGameServiceManager;				//服务管理
	ITCPNetworkEngineEvent *		m_pITCPNetworkEngineEvent;			//事件接口

	//系统配置
protected:
	tagGameParameter *				m_pGameParameter;					//配置参数
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//服务属性
	tagGameServiceOption *			m_pGameServiceOption;				//服务配置

	//用户数组
protected:
	CAndroidUserItemMap				m_AndroidUserItemMap;				//用户索引
	CAndroidUserItemArray			m_AndroidUserItemArray;				//存储对象
	CAndroidUserItemArray			m_AndroidUserItemBuffer;			//空闲对象

	//函数定义
public:
	//构造函数
	CAndroidUserManager();
	//析构函数
	virtual ~CAndroidUserManager();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//控制接口
public:
	//启动服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();

	//状态接口
public:
	//用户状况
	virtual WORD GetAndroidUserInfo(tagAndroidUserInfo & AndroidUserInfo);

	//配置接口
public:
	//配置组件
	virtual bool InitAndroidUser(tagAndroidUserParameter & AndroidUserParameter);
	//设置库存
	virtual bool SetAndroidStock(tagAndroidParameter AndroidParameter[], WORD wStockCount);

	//管理接口
public:
	//删除机器
	virtual bool DeleteAndroidUserItem(DWORD dwAndroidID);
	//查找机器
	virtual IAndroidUserItem * SearchAndroidUserItem(DWORD dwUserID, DWORD dwContextID);
	//创建机器
	virtual IAndroidUserItem * CreateAndroidUserItem(tagAndroidParameter & AndroidParameter);

	//系统事件
public:
	//脉冲事件
	virtual bool OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter);

	//网络接口
public:
	//发送数据
	virtual bool SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//发送数据
	virtual bool SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//发送数据
	virtual bool SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//对象管理
protected:
	//释放对象
	VOID FreeAndroidUserItem(DWORD dwUserID);
	//获取对象
	CAndroidUserItem * GetAndroidUserItem(WORD wIndex);
	//激活对象
	CAndroidUserItem * ActiveAndroidUserItem(tagAndroidParameter & AndroidParameter);
};

//////////////////////////////////////////////////////////////////////////////////

#endif