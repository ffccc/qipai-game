#ifndef CUSTOM_FACE_MANAGER_HEAD_FILE
#define CUSTOM_FACE_MANAGER_HEAD_FILE

#pragma once

#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//数组定义
typedef CWHArray<tagCustomFaceIndex> CCustomFaceIndexArray;

//////////////////////////////////////////////////////////////////////////////////

//头像管理
class AVATAR_CONTROL_CLASS CCustomFaceManager : public ICustomFaceManager, public IDownLoadSink
{
	//缓冲变量
protected:
	tagCustomFaceInfo				m_CustomFaceInfo;					//自定头像

	//变量定义
protected:
	CDownLoad						m_DownLoad;							//下载组件
	CCustomFaceIndexArray			m_CustomFaceIndexArray;				//等待数组

	//接口变量
protected:
	ICustomFaceEvent *				m_pICustomFaceEvent;				//回调接口

	//静态变量
protected:
	static CCustomFaceManager *		m_pCustomFaceManager;				//对象指针

	//函数定义
public:
	//构造函数
	CCustomFaceManager();
	//析构函数
	virtual ~CCustomFaceManager();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能接口
public:
	//设置接口
	virtual bool SetCustomFaceEvent(IUnknownEx * pIUnknownEx);
	//加载头像
	virtual bool LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID);
	//加载头像
	virtual bool LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);
	//保存头像
	virtual bool SaveUserCustomFace(DWORD dwUserID, DWORD dwCustomID, DWORD dwCustomFace[FACE_CX*FACE_CY]);

	//状态接口
protected:
	//下载异常
	virtual bool OnDownLoadError(enDownLoadError DownLoadError);
	//下载状态
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus);

	//序列接口
protected:
	//下载数据
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize);
	//下载信息
	virtual bool OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation);

	//内部函数
private:
	//执行下载
	bool PerformDownLoad(DWORD dwUserID, DWORD dwCustomID);

	//功能函数
public:
	//获取实例
	static CCustomFaceManager * GetInstance() { return m_pCustomFaceManager; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif