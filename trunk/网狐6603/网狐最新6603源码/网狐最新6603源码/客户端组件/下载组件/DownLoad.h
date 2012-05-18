#ifndef DOWN_LOAD_HEAD_FILE
#define DOWN_LOAD_HEAD_FILE

#pragma once

#include "WEBSocket.h"
#include "HeadTranslate.h"

//////////////////////////////////////////////////////////////////////////////////

//网站下载
class DOWN_LOAD_CLASS CDownLoad : public IWEBSocketSink
{
	//文件信息
protected:
	DWORD							m_dwTotalSize;						//文件大小
	DWORD							m_dwDownLoadSize;					//下载大小

	//状态变量
protected:
	tagDownLoadInfo					m_DownLoadInfo;						//下载信息
	enDownLoadStatus				m_DownLoadStatus;					//下载状态

	//组件变量
protected:
	CWEBSocket						m_WEBSocket;						//网络连接
	CHeadTranslate					m_HeadTranslate;					//包头解释

	//接口变量
protected:
	IDownLoadSink *					m_pIDownLoadSink;					//状态回调

	//函数定义
public:
	//构造函数
	CDownLoad();
	//析构函数
	virtual ~CDownLoad();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//状态接口
protected:
	//连接消息
	virtual bool OnSocketLink(INT nErrorCode);
	//关闭消息
	virtual bool OnSocketShut(BYTE cbShutReason);

	//数据接口
protected:
	//数据包流
	virtual VOID OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize);
	//数据包头
	virtual VOID OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode);

	//信息函数
public:
	//文件大小
	DWORD GetTotalSize() { return m_dwTotalSize; }
	//下载大小
	DWORD GetDownLoadSize() { return m_dwDownLoadSize; }
	//下载状态
	enDownLoadStatus GetDownLoadStatus() { return m_DownLoadStatus; }

	//功能函数
public:
	//关闭下载
	bool CloseDownLoad();
	//执行下载
	bool PerformDownLoad(LPCTSTR pszUrl, IUnknownEx * pIUnknownEx);

	//辅助函数
protected:
	//请求连接
	bool ConnectServer(LPCTSTR pszUrl);
	//发送请求
	bool SendRequestData(LPCTSTR pszHostName, LPCTSTR pszUrlPath);
	//获取信息
	INTERNET_SCHEME GetDownLoadInfo(LPCTSTR pszUrl, tagDownLoadInfo & DownLoadInfo);
};

//////////////////////////////////////////////////////////////////////////////////

#endif