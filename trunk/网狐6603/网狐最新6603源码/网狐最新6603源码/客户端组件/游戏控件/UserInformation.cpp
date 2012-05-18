#include "StdAfx.h"
#include "UserInformation.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CUserInformation *	CUserInformation::m_pUserInformation=NULL;			//关系接口

//////////////////////////////////////////////////////////////////////////////////

//文件名字
#define STREAM_COMPANION			L"CompanionInfo"					//关系名字
#define INFORMATION_FILE			L"UserInformation.DAT"				//文件名字

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CUserInformation::CUserInformation()
{
	//设置变量
	m_pIUserCompanionSink=NULL;

	//设置变量
	ASSERT(m_pUserInformation==NULL);
	if (m_pUserInformation==NULL) m_pUserInformation=this;

	return;
}

//析构函数
CUserInformation::~CUserInformation()
{
	//设置变量
	ASSERT(m_pUserInformation==this);
	if (m_pUserInformation==this) m_pUserInformation=NULL;

	//删除关系
	for (INT_PTR i=0;i<m_CompanionInfoActive.GetCount();i++)
	{
		tagCompanionInfo * pCompanionInfo=m_CompanionInfoActive[i];
		SafeDelete(pCompanionInfo);
	}
	for (INT_PTR i=0;i<m_CompanionInfoBuffer.GetCount();i++)
	{
		tagCompanionInfo * pCompanionInfo=m_CompanionInfoBuffer[i];
		SafeDelete(pCompanionInfo);
	}

	//删除数组
	m_CompanionInfoActive.RemoveAll();
	m_CompanionInfoBuffer.RemoveAll();

	return;
}

//接口查询
VOID * CUserInformation::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserInformation,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IUserInformation,Guid,dwQueryVer);
	return NULL;
}

//加载信息
bool CUserInformation::LoadInformation()
{
	//变量定义
	IStream * pIStreamSub=NULL;
	IStorage * pIStorageSub=NULL;
	IStorage * pIStorageRoot=NULL;
	IEnumSTATSTG * pIEnumSTATSTG=NULL;

	//读取数据
	try
	{
		//变量定义
		STATSTG Statstg;
		tagCompanionInfo CompanionInfo;
		ZeroMemory(&Statstg,sizeof(Statstg));
		ZeroMemory(&CompanionInfo,sizeof(CompanionInfo));

		//工作目录
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//打开文件
		WCHAR szInfomationFile[MAX_PATH]=L"";
		_snwprintf(szInfomationFile,CountArray(szInfomationFile),L"%s\\%s",(LPCWSTR)(CT2CW(szDirectory)),INFORMATION_FILE);

		//打开文件
		StgOpenStorage(szInfomationFile,NULL,STGM_READ|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageRoot);
		if (pIStorageRoot==NULL) return false;

		//创建枚举
		pIStorageRoot->EnumElements(0,NULL,0,&pIEnumSTATSTG);
		if (pIEnumSTATSTG==NULL) throw TEXT("EnumElements Create pIEnumSTATSTG Error");

		//枚举对象
		while (pIEnumSTATSTG->Next(1,&Statstg,NULL)==NOERROR)
		{
			//目录类型
			if (Statstg.type==STGTY_STORAGE)
			{
				//打开存储
				pIStorageRoot->OpenStorage(Statstg.pwcsName,NULL,STGM_READ|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageSub);
				if (pIStorageSub==NULL) throw TEXT("CreateStorage Create pIStorageSub Error");

				//打开数据
				pIStorageSub->OpenStream(STREAM_COMPANION,NULL,STGM_READ|STGM_SHARE_EXCLUSIVE,0,&pIStreamSub);

				//读取数据
				if (pIStreamSub!=NULL)
				{
					//读取关系
					ULONG cbReadCount=0L;
					pIStreamSub->Read(&CompanionInfo,sizeof(CompanionInfo),&cbReadCount);

					//读取效验
					ASSERT(cbReadCount==sizeof(CompanionInfo));
					if (cbReadCount!=sizeof(CompanionInfo)) throw TEXT("Read Data CompanionInfo Error");

					//创建对象
					tagCompanionInfo * pCompanionInfo=CreateCompanionItem();
					if (pCompanionInfo==NULL) throw TEXT("CreateCompanionItem Error");

					//设置信息
					CopyMemory(pCompanionInfo,&CompanionInfo,sizeof(CompanionInfo));
				}

				//释放接口
				SafeRelease(pIStreamSub);
				SafeRelease(pIStorageSub);
			}

			//释放内存
			CoTaskMemFree(Statstg.pwcsName);
		}

		//释放接口
		SafeRelease(pIEnumSTATSTG);
		SafeRelease(pIStreamSub);
		SafeRelease(pIStorageSub);
		SafeRelease(pIStorageRoot);

		return true;
	}
	catch (...)
	{
		//错误断言
		ASSERT(FALSE);

		//释放接口
		SafeRelease(pIEnumSTATSTG);
		SafeRelease(pIStreamSub);
		SafeRelease(pIStorageSub);
		SafeRelease(pIStorageRoot);
	}

	return false;
}

//设置接口
bool CUserInformation::SetUserCompanionSink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserCompanionSink)!=NULL);
		m_pIUserCompanionSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserCompanionSink);

		//成功判断
		if (m_pIUserCompanionSink==NULL) return false;
	}
	else m_pIUserCompanionSink=NULL;

	return true;
}

//枚举关系
tagCompanionInfo * CUserInformation::EmunCompanionInfo(WORD wIndex)
{
	//枚举子项
	if (wIndex<m_CompanionInfoActive.GetCount())
	{
		tagCompanionInfo * pCompanionInfo=m_CompanionInfoActive[wIndex];
		return pCompanionInfo;
	}
	
	return NULL;
}

//寻找关系
tagCompanionInfo * CUserInformation::SearchCompanionInfo(DWORD dwUserID)
{
	//搜索关系
	for (INT_PTR i=0;i<m_CompanionInfoActive.GetCount();i++)
	{
		tagCompanionInfo * pCompanionInfo=m_CompanionInfoActive[i];
		if (pCompanionInfo->dwUserID==dwUserID) return pCompanionInfo;
	}

	return NULL;
}

//设置关系
tagCompanionInfo * CUserInformation::InsertCompanionInfo(IClientUserItem * pIClientUserItem, BYTE cbCompanion)
{
	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return NULL;

	//用户搜索
	DWORD dwUserID=pIClientUserItem->GetUserID();
	tagCompanionInfo * pCompanionInfo=SearchCompanionInfo(dwUserID);

	//设置信息
	if (pCompanionInfo==NULL)
	{
		//创建对象
		pCompanionInfo=CreateCompanionItem();
		if (pCompanionInfo==NULL) return NULL;

		//设置信息
		pCompanionInfo->cbCompanion=cbCompanion;
		pCompanionInfo->dwGameID=pIClientUserItem->GetGameID();
		pCompanionInfo->dwUserID=pIClientUserItem->GetUserID();
		lstrcpyn(pCompanionInfo->szNickName,pIClientUserItem->GetNickName(),CountArray(pCompanionInfo->szNickName));
		lstrcpyn(pCompanionInfo->szUserNote,pIClientUserItem->GetUserNoteInfo(),CountArray(pCompanionInfo->szUserNote));

		//插入通知
		if (m_pIUserCompanionSink!=NULL) m_pIUserCompanionSink->OnCompanionInsert(pCompanionInfo);
	}
	else
	{
		//修改判断
		bool bModify=false;
		if (pCompanionInfo->cbCompanion!=cbCompanion) bModify=true;
		if ((bModify==false)&&(lstrcmp(pCompanionInfo->szNickName,pIClientUserItem->GetNickName())!=0)) bModify=true;
		if ((bModify==false)&&(lstrcmp(pCompanionInfo->szUserNote,pIClientUserItem->GetUserNoteInfo())!=0)) bModify=true;

		//修改判断
		if (bModify=false) return pCompanionInfo;

		//设置信息
		pCompanionInfo->cbCompanion=cbCompanion;
		pCompanionInfo->dwGameID=pIClientUserItem->GetGameID();
		pCompanionInfo->dwUserID=pIClientUserItem->GetUserID();
		lstrcpyn(pCompanionInfo->szNickName,pIClientUserItem->GetNickName(),CountArray(pCompanionInfo->szNickName));
		lstrcpyn(pCompanionInfo->szUserNote,pIClientUserItem->GetUserNoteInfo(),CountArray(pCompanionInfo->szUserNote));

		//更新通知
		if (m_pIUserCompanionSink!=NULL) m_pIUserCompanionSink->OnCompanionUpdate(pCompanionInfo);
	}

	//变量定义
	IStream * pIStreamSub=NULL;
	IStorage * pIStorageSub=NULL;
	IStorage * pIStorageRoot=NULL;

	//写入数据
	try
	{
		//构造名字
		WCHAR szStorageName[16]=L"";
		_snwprintf(szStorageName,CountArray(szStorageName),L"%ld",dwUserID);

		//工作目录
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//打开文件
		WCHAR szInfomationFile[MAX_PATH]=L"";
		_snwprintf(szInfomationFile,CountArray(szInfomationFile),L"%s\\%s",(LPCWSTR)(CT2CW(szDirectory)),INFORMATION_FILE);

		//打开文件
		StgOpenStorage(szInfomationFile,NULL,STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageRoot);
		if (pIStorageRoot==NULL) StgCreateDocfile(szInfomationFile,STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pIStorageRoot);

		//错误判断
		if (pIStorageRoot==NULL) throw TEXT("StgCreateDocfile Create pIStorageRoot Error");

		//打开存储
		pIStorageRoot->CreateStorage(szStorageName,STGM_WRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageSub);
		if (pIStorageSub==NULL) pIStorageRoot->CreateStorage(szStorageName,STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageSub);

		//错误处理
		if (pIStorageSub==NULL) throw TEXT("CreateStorage Create pIStorageSub Error");

		//创建数据
		pIStorageSub->CreateStream(STREAM_COMPANION,STGM_WRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStreamSub);
		if (pIStreamSub==NULL) pIStorageSub->CreateStream(STREAM_COMPANION,STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStreamSub);

		//错误处理
		if (pIStreamSub==NULL) throw TEXT("CreateStream Create pIStreamSub Error");

		//写入数据
		ULONG cbWriteCount=0L;
		pIStreamSub->Write(pCompanionInfo,sizeof(tagCompanionInfo),&cbWriteCount);

		//释放接口
		SafeRelease(pIStreamSub);
		SafeRelease(pIStorageSub);
		SafeRelease(pIStorageRoot);

		return pCompanionInfo;
	}
	catch (...)
	{
		//错误断言
		ASSERT(FALSE);

		//释放接口
		SafeRelease(pIStreamSub);
		SafeRelease(pIStorageSub);
		SafeRelease(pIStorageRoot);
	}

	return NULL;
}

//创建关系
tagCompanionInfo * CUserInformation::CreateCompanionItem()
{
	//变量定义
	tagCompanionInfo * pCompanionInfo=NULL;

	//获取缓冲
	if (m_CompanionInfoBuffer.GetCount()>0)
	{
		INT_PTR nItemCount=m_CompanionInfoBuffer.GetCount();
		pCompanionInfo=m_CompanionInfoBuffer[nItemCount-1];
		m_CompanionInfoBuffer.RemoveAt(nItemCount-1);
	}

	//创建对象
	if (pCompanionInfo==NULL)
	{
		pCompanionInfo=new tagCompanionInfo;
		if (pCompanionInfo==NULL) return NULL;
	}

	//还原变量
	ZeroMemory(pCompanionInfo,sizeof(tagCompanionInfo));

	//插入子项
	m_CompanionInfoActive.Add(pCompanionInfo);

	return pCompanionInfo;
}

//释放关系
bool CUserInformation::DeleteCompanionItem(tagCompanionInfo * pCompanionInfo)
{
	//搜索对象
	for (INT_PTR i=0;i<m_CompanionInfoActive.GetCount();i++)
	{
		if (m_CompanionInfoActive[i]==pCompanionInfo)
		{
			//删除对象
			m_CompanionInfoActive.RemoveAt(i);
			m_CompanionInfoBuffer.Add(pCompanionInfo);

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//////////////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(UserInformation);

//////////////////////////////////////////////////////////////////////////////////
