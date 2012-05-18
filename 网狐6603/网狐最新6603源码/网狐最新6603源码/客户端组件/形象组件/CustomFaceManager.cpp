#include "StdAfx.h"
#include "CustomFaceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//文件结构
struct tagCustomFaceFile
{
	DWORD							dwCustomID;							//自定标识
	tagCustomFaceInfo				CustomFaceInfo;						//头像信息
};

//静态变量
CCustomFaceManager * CCustomFaceManager::m_pCustomFaceManager=NULL;		//对象指针

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCustomFaceManager::CCustomFaceManager()
{
	//设置变量
	m_pICustomFaceEvent=NULL;
	ZeroMemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));

	//设置对象
	ASSERT(m_pCustomFaceManager==NULL);
	if (m_pCustomFaceManager==NULL) m_pCustomFaceManager=this;

	return;
}

//析构函数
CCustomFaceManager::~CCustomFaceManager()
{
	//释放对象
	ASSERT(m_pCustomFaceManager==this);
	if (m_pCustomFaceManager==this) m_pCustomFaceManager=NULL;

	return;
}

//接口查询
VOID * CCustomFaceManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE(ICustomFaceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICustomFaceManager,Guid,dwQueryVer);
	return NULL;
}

//设置接口
bool CCustomFaceManager::SetCustomFaceEvent(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent)!=NULL);
		m_pICustomFaceEvent=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent);

		//成功判断
		if (m_pICustomFaceEvent==NULL) return false;
	}
	else m_pICustomFaceEvent=NULL;

	return true;
}

//加载头像
bool CCustomFaceManager::LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID)
{
	//查找现有
	for (INT_PTR i=0;i<m_CustomFaceIndexArray.GetCount();i++)
	{
		//获取对象
		tagCustomFaceIndex * pCustomFaceIndex=&m_CustomFaceIndexArray[i];

		//对象判断
		if (pCustomFaceIndex->dwUserID==dwUserID)
		{
			//版本一致
			if (pCustomFaceIndex->dwCustomID==dwCustomID)
			{
				return true;
			}

			//版本差异
			if (pCustomFaceIndex->dwCustomID!=dwCustomID)
			{
				if (i==0)
				{
					//变量定义
					tagCustomFaceIndex CustomFaceIndex;
					ZeroMemory(&CustomFaceIndex,sizeof(CustomFaceIndex));

					//设置变量
					CustomFaceIndex.dwUserID=dwUserID;
					CustomFaceIndex.dwCustomID=dwCustomID;

					//插入数组
					m_CustomFaceIndexArray.Add(CustomFaceIndex);
				}
				else
				{
					//改写现有
					pCustomFaceIndex->dwCustomID=dwCustomID;
				}

				return true;
			}
		}
	}

	//变量定义
	tagCustomFaceIndex CustomFaceIndex;
	ZeroMemory(&CustomFaceIndex,sizeof(CustomFaceIndex));

	//设置变量
	CustomFaceIndex.dwUserID=dwUserID;
	CustomFaceIndex.dwCustomID=dwCustomID;

	//插入数组
	m_CustomFaceIndexArray.Add(CustomFaceIndex);

	//下载数据
	if (m_CustomFaceIndexArray.GetCount()==1L)
	{
		PerformDownLoad(dwUserID,dwCustomID);
	}

	return true;
}

//加载头像
bool CCustomFaceManager::LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//工作目录
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//目标目录
	TCHAR szCustomFacePath[MAX_PATH]=TEXT("");
	_sntprintf(szCustomFacePath,CountArray(szCustomFacePath),TEXT("%s\\CustomFace\\%ld.DAT"),szWorkDirectory,dwUserID);

	//打开文件
	CFile DataFile;
	if (DataFile.Open(szCustomFacePath,CFile::modeRead)==FALSE) return false;

	//效验长度
	ASSERT(DataFile.GetLength()==sizeof(tagCustomFaceFile));
	if (DataFile.GetLength()!=sizeof(tagCustomFaceFile)) return false;

	//读取数据
	tagCustomFaceFile CustomFaceFile;
	UINT uReadCount=DataFile.Read(&CustomFaceFile,sizeof(CustomFaceFile));

	//读取效验
	if (uReadCount!=sizeof(CustomFaceFile)) return false;
	if (CustomFaceFile.dwCustomID!=dwCustomID) return false;
	if (CustomFaceFile.CustomFaceInfo.dwDataSize!=sizeof(CustomFaceFile.CustomFaceInfo)) return false;

	//设置数据
	CopyMemory(&CustomFaceInfo,&CustomFaceFile.CustomFaceInfo,sizeof(CustomFaceInfo));

	return true;
}

//保存数据
bool CCustomFaceManager::SaveUserCustomFace(DWORD dwUserID, DWORD dwCustomID, DWORD dwCustomFace[FACE_CX*FACE_CY])
{
	//工作目录
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//目标目录
	TCHAR szCustomDirectory[MAX_PATH]=TEXT("");
	_sntprintf(szCustomDirectory,CountArray(szCustomDirectory),TEXT("%s\\CustomFace"),szWorkDirectory);

	//目标文件
	TCHAR szCustomFacePath[MAX_PATH]=TEXT("");
	_sntprintf(szCustomFacePath,CountArray(szCustomFacePath),TEXT("%s\\CustomFace\\%ld.DAT"),szWorkDirectory,dwUserID);

	//创建目录
	CreateDirectory(szCustomDirectory,NULL);

	//打开文件
	CFile DataFile;
	if (DataFile.Open(szCustomFacePath,CFile::modeWrite|CFile::modeCreate)==TRUE)
	{
		//变量定义
		tagCustomFaceFile CustomFaceFile;
		ZeroMemory(&CustomFaceFile,sizeof(CustomFaceFile));

		//填写资料
		CustomFaceFile.dwCustomID=dwCustomID;
		CustomFaceFile.CustomFaceInfo.dwDataSize=sizeof(tagCustomFaceInfo);
		CopyMemory(CustomFaceFile.CustomFaceInfo.dwCustomFace,dwCustomFace,sizeof(CustomFaceFile.CustomFaceInfo.dwCustomFace));

		//写入文件
		DataFile.Write(&CustomFaceFile,sizeof(CustomFaceFile));
		DataFile.SetLength(sizeof(CustomFaceFile));

		//关闭文件
		DataFile.Close();
	}

	return true;
}

//下载异常
bool CCustomFaceManager::OnDownLoadError(enDownLoadError DownLoadError)
{
	//状态效验
	ASSERT(m_CustomFaceIndexArray.GetCount()>0L);
	if (m_CustomFaceIndexArray.GetCount()==0L) return false;

	//删除数据
	m_CustomFaceIndexArray.RemoveAt(0);

	//下载数据
	if (m_CustomFaceIndexArray.GetCount()>=1L)
	{
		tagCustomFaceIndex * pCustomFaceIndex=&m_CustomFaceIndexArray[0];
		PerformDownLoad(pCustomFaceIndex->dwUserID,pCustomFaceIndex->dwCustomID);
	}

	return true;
}

//下载状态
bool CCustomFaceManager::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	//下载完成
	if (DownLoadStatus==DownLoadStatus_Conclude)
	{
		//状态效验
		ASSERT(m_CustomFaceIndexArray.GetCount()>0L);
		if (m_CustomFaceIndexArray.GetCount()==0L) return false;

		//保存文件
		tagCustomFaceIndex CustomFaceIndex=m_CustomFaceIndexArray[0];
		SaveUserCustomFace(CustomFaceIndex.dwUserID,CustomFaceIndex.dwCustomID,m_CustomFaceInfo.dwCustomFace);

		//删除数据
		m_CustomFaceIndexArray.RemoveAt(0);

		//事件通知
		ASSERT(m_pICustomFaceEvent!=NULL);
		m_pICustomFaceEvent->OnEventCustomFace(CustomFaceIndex.dwUserID,CustomFaceIndex.dwCustomID,m_CustomFaceInfo);

		//下载数据
		if (m_CustomFaceIndexArray.GetCount()>=1L)
		{
			tagCustomFaceIndex * pCustomFaceIndex=&m_CustomFaceIndexArray[0];
			PerformDownLoad(pCustomFaceIndex->dwUserID,pCustomFaceIndex->dwCustomID);
		}
	}

	return true;
}

//下载数据
bool CCustomFaceManager::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//效验数据
	ASSERT((wStreamSize+m_CustomFaceInfo.dwDataSize)<=sizeof(m_CustomFaceInfo.dwCustomFace));
	if ((wStreamSize+m_CustomFaceInfo.dwDataSize)>sizeof(m_CustomFaceInfo.dwCustomFace)) return false;

	//保存数据
	DWORD dwSourceIndex=m_CustomFaceInfo.dwDataSize;
	m_CustomFaceInfo.dwDataSize=m_CustomFaceInfo.dwDataSize+wStreamSize;
	CopyMemory((BYTE *)(m_CustomFaceInfo.dwCustomFace)+dwSourceIndex,pcbMailData,wStreamSize);

	return true;
}

//下载信息
bool CCustomFaceManager::OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	return true;
}

//执行下载
bool CCustomFaceManager::PerformDownLoad(DWORD dwUserID, DWORD dwCustomID)
{
	//设置变量
	ZeroMemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));

	//构造地址
	TCHAR szCustomFace[128]=TEXT("");
	_sntprintf(szCustomFace,CountArray(szCustomFace),TEXT("%s/CustomFace.asp?UserID=%ld&CustomID=%ld"),szPlatformLink,dwUserID,dwCustomID);

	//下载头像
	ASSERT(m_DownLoad.GetDownLoadStatus()==DownLoadStatus_Conclude);
	m_DownLoad.PerformDownLoad(szCustomFace,QUERY_ME_INTERFACE(IUnknownEx));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(CustomFaceManager);

//////////////////////////////////////////////////////////////////////////////////
