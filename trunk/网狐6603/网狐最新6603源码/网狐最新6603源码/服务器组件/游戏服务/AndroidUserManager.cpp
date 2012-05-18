#include "StdAfx.h"
#include "AndroidUserManager.h"

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define ANDROID_LOGON_COUNT			32									//登录数目

//时间标识
#define IDI_ANDROID_INOUT			(IDI_REBOT_MODULE_START+0)			//进出时间
#define IDI_ANDROID_PULSE			(IDI_REBOT_MODULE_START+1)			//脉冲时间

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserManager::CAndroidUserManager()
{
	//系统参数
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件接口
	m_pITimerEngine=NULL;
	m_pIServerUserManager=NULL;
	m_pIGameServiceManager=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//配置变量
	m_wAutoAndroidCount=ANDROID_LOGON_COUNT;

	//库存变量
	m_wStockCount=0;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	//设置索引
	m_AndroidUserItemMap.InitHashTable(PRIME_ANDROID_USER);

	return;
}

//析构函数
CAndroidUserManager::~CAndroidUserManager()
{
	//清理断言
	ASSERT(m_AndroidUserItemMap.GetCount()==0L);
	ASSERT(m_AndroidUserItemArray.GetCount()==0L);
	ASSERT(m_AndroidUserItemBuffer.GetCount()==0L);

	return;
}

//接口查询
VOID * CAndroidUserManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserManager,Guid,dwQueryVer);
	return NULL;
}

//启动服务
bool CAndroidUserManager::StartService()
{
	//启动时间
	m_pITimerEngine->SetTimer(IDI_ANDROID_INOUT,TIME_ANDROID_INOUT*1000L,TIMES_INFINITY,0);
	m_pITimerEngine->SetTimer(IDI_ANDROID_PULSE,TIME_ANDROID_PULSE*1000L,TIMES_INFINITY,0);

	return true;
}

//停止服务
bool CAndroidUserManager::ConcludeService()
{
	//删除存储
	for (INT_PTR i=0;i<m_AndroidUserItemArray.GetCount();i++)
	{
		SafeRelease(m_AndroidUserItemArray[i]);
	}

	//清理数组
	m_AndroidUserItemMap.RemoveAll();
	m_AndroidUserItemArray.RemoveAll();
	m_AndroidUserItemBuffer.RemoveAll();

	//库存用户
	m_wStockCount=0;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	return true;
}

//用户状况
WORD CAndroidUserManager::GetAndroidUserInfo(tagAndroidUserInfo & AndroidUserInfo)
{
	//设置变量
	ZeroMemory(&AndroidUserInfo,sizeof(AndroidUserInfo));

	//变量定义
	POSITION Position=m_AndroidUserItemMap.GetStartPosition();

	//枚举对象
	while (Position!=NULL)
	{
		//获取对象
		DWORD dwUserID=0L;
		CAndroidUserItem * pAndroidUserItem=NULL;
		m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

		//效验参数
		ASSERT((dwUserID!=0L)&&(pAndroidUserItem!=NULL));
		if ((dwUserID==0L)||(pAndroidUserItem==NULL)) break;

		//绑定判断
		if (pAndroidUserItem->m_pIServerUserItem==NULL) continue;

		//变量定义
		IServerUserItem * pIServerUserItem=pAndroidUserItem->m_pIServerUserItem;

		//状态判断
		switch (pIServerUserItem->GetUserStatus())
		{
		case US_FREE:
			{
				ASSERT(AndroidUserInfo.wFreeUserCount<CountArray(AndroidUserInfo.pIAndroidUserFree));
				AndroidUserInfo.pIAndroidUserFree[AndroidUserInfo.wFreeUserCount++]=pAndroidUserItem;
				break;
			}
		case US_SIT:
		case US_READY:
			{
				ASSERT(AndroidUserInfo.wSitdownUserCount<CountArray(AndroidUserInfo.pIAndroidUserSitdown));
				AndroidUserInfo.pIAndroidUserSitdown[AndroidUserInfo.wSitdownUserCount++]=pAndroidUserItem;
				break;
			}
		case US_PLAYING:
		case US_OFFLINE:
			{
				ASSERT(AndroidUserInfo.wPlayUserCount<CountArray(AndroidUserInfo.pIAndroidUserPlay));
				AndroidUserInfo.pIAndroidUserPlay[AndroidUserInfo.wPlayUserCount++]=pAndroidUserItem;
				break;
			}
		}
	};

	return AndroidUserInfo.wFreeUserCount+AndroidUserInfo.wPlayUserCount+AndroidUserInfo.wSitdownUserCount;
}

//配置组件
bool CAndroidUserManager::InitAndroidUser(tagAndroidUserParameter & AndroidUserParameter)
{
	//设置变量
	m_pGameParameter=AndroidUserParameter.pGameParameter;
	m_pGameServiceAttrib=AndroidUserParameter.pGameServiceAttrib;
	m_pGameServiceOption=AndroidUserParameter.pGameServiceOption;

	//组件接口
	m_pITimerEngine=AndroidUserParameter.pITimerEngine;
	m_pIServerUserManager=AndroidUserParameter.pIServerUserManager;
	m_pIGameServiceManager=AndroidUserParameter.pIGameServiceManager;
	m_pITCPNetworkEngineEvent=AndroidUserParameter.pITCPNetworkEngineEvent;

	return true;
}

//设置库存
bool CAndroidUserManager::SetAndroidStock(tagAndroidParameter AndroidParameter[], WORD wStockCount)
{
	//设置用户
	for (WORD i=0;i<wStockCount;i++)
	{
		//数目效验
		ASSERT(m_wStockCount<CountArray(m_AndroidParameter));
		if (m_wStockCount>=CountArray(m_AndroidParameter)) break;

		//设置变量
		CAndroidUserItem * pAndroidUserItem=NULL;
		m_AndroidUserItemMap.Lookup(m_AndroidParameter[i].dwUserID,pAndroidUserItem);

		//设置变量
		if (pAndroidUserItem==NULL)
		{
			m_AndroidParameter[m_wStockCount++]=AndroidParameter[i];
		}
	}

	return true;
}

//删除机器
bool CAndroidUserManager::DeleteAndroidUserItem(DWORD dwAndroidID)
{
	//获取对象
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);

	//对象效验
	ASSERT((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRoundID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRoundID!=HIWORD(dwAndroidID))) return false;

	//关闭事件
	try
	{
		m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(dwAndroidID,0,0L);
	}
	catch (...)
	{
		//错误断言
		ASSERT(FALSE);
	}

	//删除对象
	FreeAndroidUserItem(pAndroidUserItem->GetUserID());

	return true;
}

//查找机器
IAndroidUserItem * CAndroidUserManager::SearchAndroidUserItem(DWORD dwUserID, DWORD dwContextID)
{
	//查找机器
	CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemMap[dwUserID];

	//机器判断
	if (pAndroidUserItem!=NULL)
	{
		WORD wRoundID=pAndroidUserItem->m_wRoundID;
		WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
		if (MAKELONG(wAndroidIndex,wRoundID)==dwContextID) return pAndroidUserItem;
	}

	return NULL;
}

//创建机器
IAndroidUserItem * CAndroidUserManager::CreateAndroidUserItem(tagAndroidParameter & AndroidParameter)
{
	//效验参数
	ASSERT(AndroidParameter.dwUserID!=0L);
	if (AndroidParameter.dwUserID==0L) return NULL;

	//创建对象
	CAndroidUserItem * pAndroidUserItem=ActiveAndroidUserItem(AndroidParameter);
	if (pAndroidUserItem==NULL) return NULL;

	//属性变量
	WORD wRoundID=pAndroidUserItem->m_wRoundID;
	WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;

	//连接模拟
	try
	{
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkBind(MAKELONG(wAndroidIndex,wRoundID),0L)==false)
		{
			throw 0;
		}
	}
	catch (...)
	{
		//错误断言
		ASSERT(FALSE);

		//释放用户
		FreeAndroidUserItem(AndroidParameter.dwUserID);

		return NULL;
	}

	//变量定义
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//版本信息
	LogonUserID.dwPlazaVersion=VERSION_PLAZA;
	LogonUserID.dwFrameVersion=VERSION_FRAME;
	LogonUserID.dwProcessVersion=m_pGameServiceAttrib->dwClientVersion;

	//用户信息
	LogonUserID.dwUserID=AndroidParameter.dwUserID;
	lstrcpyn(LogonUserID.szPassword,TEXT(""),CountArray(LogonUserID.szPassword));

	//构造数据
	TCP_Command Command;
	Command.wMainCmdID=MDM_GR_LOGON;
	Command.wSubCmdID=SUB_GR_LOGON_USERID;

	//消息处理
	try
	{
		//发送数据
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(MAKELONG(wAndroidIndex,wRoundID),Command,&LogonUserID,sizeof(LogonUserID))==false)
		{
			throw 0;
		}
	}
	catch (...) 
	{
		//错误断言
		ASSERT(FALSE);

		//删除机器
		DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));

		return NULL;
	}

	return pAndroidUserItem;
}

//脉冲事件
bool CAndroidUserManager::OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_ANDROID_INOUT:			//进出处理
		{
			//变量定义
			DWORD dwUserID=0L;
			INT_PTR nActiveCount=m_AndroidUserItemMap.GetCount();
			POSITION Position=m_AndroidUserItemMap.GetStartPosition();

			//登录处理
			if (CServerRule::IsAllowAndroidUser(m_pGameServiceOption->dwServerRule)==true)
			{
				//库存处理
				if ((m_wStockCount>0)&&(nActiveCount<m_wAutoAndroidCount))
				{
					//获取用户
					WORD wIndex=rand()%m_wStockCount;

					//创建机器
					if (CreateAndroidUserItem(m_AndroidParameter[wIndex])!=NULL)
					{
						//删除存存
						m_wStockCount--;
						m_AndroidParameter[wIndex]=m_AndroidParameter[m_wStockCount];
					}

					return true;
				}
			}

			//变量定义
			DWORD dwCurrentTime=(DWORD)time(NULL);
			DWORD dwServerRule=m_pGameServiceOption->dwServerRule;

			//退出处理
			while (Position!=NULL)
			{
				//获取对象
				CAndroidUserItem * pAndroidUserItem=NULL;
				m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

				//退出判断
				if (pAndroidUserItem->m_pIServerUserItem!=NULL)
				{
					//变量定义
					IServerUserItem * pIServerUserItem=pAndroidUserItem->m_pIServerUserItem;

					//服务状态
					BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
					if ((cbUserStatus!=US_FREE)&&(cbUserStatus!=US_SIT)) continue;

					//服务时间
					DWORD dwLogonTime=pIServerUserItem->GetLogonTime();
					DWORD dwReposeTime=pAndroidUserItem->m_dwReposeTime;
					if (((dwLogonTime+dwLogonTime)>dwCurrentTime)&&(CServerRule::IsAllowAndroidUser(dwServerRule)==true)) continue;

					//删除用户
					WORD wRoundID=pAndroidUserItem->m_wRoundID;
					WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
					DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));

					return true;
				}
			}

			return true;
		}
	case IDI_ANDROID_PULSE:		//用户脉冲
		{
			//变量定义
			DWORD dwUserID=0L;
			CAndroidUserItem * pAndroidUserItem=NULL;
			POSITION Position=m_AndroidUserItemMap.GetStartPosition();

			//用户处理
			while (Position!=NULL)
			{
				//获取对象
				m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

				//时间处理
				try
				{
					ASSERT(pAndroidUserItem!=NULL);
					if (pAndroidUserItem->m_pIServerUserItem!=NULL) pAndroidUserItem->OnTimerPulse(dwTimerID,dwBindParameter);
				}
				catch (...)
				{
					ASSERT(FALSE);
					DeleteAndroidUserItem(MAKELONG(pAndroidUserItem->m_wAndroidIndex,pAndroidUserItem->m_wRoundID));
				}
			}

			return true;
		}
	}

	return false;
}

//发送数据
bool CAndroidUserManager::SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//发送数据
	for (INT_PTR i=0;i<m_AndroidUserItemArray.GetCount();i++)
	{
		//获取对象
		CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemArray[i];

		//状态判断
		if (pAndroidUserItem->m_pIServerUserItem==NULL) continue;
		if (pAndroidUserItem->m_AndroidParameter.dwUserID==0L) continue;

		//消息处理
		try
		{
			if (pAndroidUserItem->OnSocketRead(wMainCmdID,wSubCmdID,pData,wDataSize)==false)
			{
				throw 0;
			}
		}
		catch (...) 
		{
			//错误断言
			ASSERT(FALSE);

			//断开用户
			WORD wRoundID=pAndroidUserItem->m_wRoundID;
			WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
			DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));
		}
	}

	return true;
}

//发送数据
bool CAndroidUserManager::SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//获取对象
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);

	//对象效验
	ASSERT((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRoundID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRoundID!=HIWORD(dwAndroidID))) return false;

	//消息处理
	try
	{
		if (pAndroidUserItem->OnSocketRead(wMainCmdID,wSubCmdID,pData,wDataSize)==false)
		{
			ASSERT(FALSE);
			throw 0;
		}
	}
	catch (...) 
	{
		ASSERT(FALSE);
		DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//发送数据
bool CAndroidUserManager::SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//构造数据
	TCP_Command Command;
	Command.wSubCmdID=wSubCmdID;
	Command.wMainCmdID=wMainCmdID;

	//消息处理
	try
	{
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(dwAndroidID,Command,pData,wDataSize)==false)
		{
			throw 0;
		}
	}
	catch (...) 
	{
		ASSERT(FALSE);
		DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//获取对象
CAndroidUserItem * CAndroidUserManager::GetAndroidUserItem(WORD wIndex)
{
	//效验索引
	ASSERT(wIndex>=INDEX_ANDROID);
	if (wIndex<INDEX_ANDROID) return NULL;

	//效验索引
	ASSERT((wIndex-INDEX_ANDROID)<m_AndroidUserItemArray.GetCount());
	if (((wIndex-INDEX_ANDROID)>=m_AndroidUserItemArray.GetCount())) return NULL;

	//获取对象
	WORD wBufferIndex=wIndex-INDEX_ANDROID;
	CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemArray[wBufferIndex];

	return pAndroidUserItem;
}

//释放对象
VOID CAndroidUserManager::FreeAndroidUserItem(DWORD dwUserID)
{
	//效验参数
	ASSERT(dwUserID!=0);
	if (dwUserID==0) return;

	//变量定义
	CAndroidUserItem * pAndroidUserItem=NULL;
	m_AndroidUserItemMap.Lookup(dwUserID,pAndroidUserItem);

	//对象判断
	if (pAndroidUserItem==NULL)
	{
		ASSERT(FALSE);
		return;
	}

	//复位对象
	pAndroidUserItem->RepositUserItem();

	//设置索引
	m_AndroidUserItemMap.RemoveKey(dwUserID);
	m_AndroidUserItemBuffer.Add(pAndroidUserItem);

	return;
}

//激活对象
CAndroidUserItem * CAndroidUserManager::ActiveAndroidUserItem(tagAndroidParameter & AndroidParameter)
{
	//变量定义
	CAndroidUserItem * pAndroidUserItem=NULL;
	INT_PTR nFreeItemCount=m_AndroidUserItemBuffer.GetCount();

	//获取对象
	if (nFreeItemCount>0)
	{
		//获取对象
		INT_PTR nItemPostion=nFreeItemCount-1;
		pAndroidUserItem=m_AndroidUserItemBuffer[nItemPostion];

		//删除数组
		m_AndroidUserItemBuffer.RemoveAt(nItemPostion);
	}

	//创建对象
	if (pAndroidUserItem==NULL)
	{
		//数目判断
		if (m_AndroidUserItemArray.GetCount()>=MAX_ANDROID)
		{
			ASSERT(FALSE);
			return NULL;
		}

		try
		{
			//变量定义
			IAndroidUserItemSink * pIAndroidUserItemSink=(IAndroidUserItemSink *)
				m_pIGameServiceManager->CreateAndroidUserItemSink(IID_IAndroidUserItemSink,VER_IAndroidUserItemSink);

			//创建对象
			try
			{
				pAndroidUserItem=new CAndroidUserItem;
			}
			catch (...)
			{
				//错误断言
				ASSERT(FALSE);

				//删除对象
				SafeRelease(pIAndroidUserItemSink);
			}

			//错误判断
			if (pAndroidUserItem==NULL)
			{
				//错误断言
				ASSERT(FALSE);

				//删除对象
				SafeRelease(pIAndroidUserItemSink);

				return NULL;
			}

			//设置用户
			if ((pIAndroidUserItemSink!=NULL)&&(pIAndroidUserItemSink->Initialization(pAndroidUserItem)==false))
			{
				//错误断言
				ASSERT(FALSE);

				//删除对象
				SafeRelease(pAndroidUserItem);
				SafeRelease(pIAndroidUserItemSink);

				return NULL;
			}

			//设置数组
			WORD wCurrentIndex=(WORD)m_AndroidUserItemArray.Add(pAndroidUserItem);

			//设置接口
			pAndroidUserItem->m_wAndroidIndex=wCurrentIndex+INDEX_ANDROID;
			pAndroidUserItem->m_pIServerUserManager=m_pIServerUserManager;
			pAndroidUserItem->m_pIAndroidUserItemSink=pIAndroidUserItemSink;
			pAndroidUserItem->m_pIAndroidUserManager=QUERY_ME_INTERFACE(IAndroidUserManager);
		}
		catch (...) 
		{ 
			ASSERT(FALSE);
			return NULL; 
		}
	}

	//服务时间
	if ((AndroidParameter.dwMaxReposeTime!=0L)&&(AndroidParameter.dwMinReposeTime!=0L))
	{
		//变量定义
		DWORD dwMaxReposeTime=AndroidParameter.dwMaxReposeTime;
		DWORD dwMinReposeTime=AndroidParameter.dwMinReposeTime;

		//调整时间
		if ((dwMaxReposeTime-dwMinReposeTime)>0L)
		{
			pAndroidUserItem->m_dwReposeTime=dwMinReposeTime+rand()%(dwMaxReposeTime-dwMinReposeTime);
		}
		else
		{
			pAndroidUserItem->m_dwReposeTime=dwMinReposeTime;
		}
	}
	else
	{
		pAndroidUserItem->m_dwReposeTime=TIME_ANDROID_REPOSE_TIME;
	}

	//设置变量
	pAndroidUserItem->m_AndroidParameter=AndroidParameter;

	//设置索引
	m_AndroidUserItemMap[AndroidParameter.dwUserID]=pAndroidUserItem;

	return pAndroidUserItem;
}

//////////////////////////////////////////////////////////////////////////////////
