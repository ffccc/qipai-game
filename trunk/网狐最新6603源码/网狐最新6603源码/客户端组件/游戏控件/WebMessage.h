#ifndef WEB_MESSAGE_HEAD_FILE
#define WEB_MESSAGE_HEAD_FILE

#pragma once

#include "AfxHtml.h"
#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//网页消息
class SHARE_CONTROL_CLASS CWebMessage : public CWnd, public IDownLoadSink
{
	//配置变量
protected:
	COLORREF						m_crBackGroup;						//背景颜色

	//数据变量
protected:
	DWORD							m_dwDataSize;						//数据大小
	DWORD							m_dwBufferSize;						//缓冲大小
	LPBYTE							m_pcbDataBuffer;					//数据缓冲

	//组件变量
protected:
	CDownLoad						m_DownLoad;							//下载组件

	//函数定义
public:
	//构造函数
	CWebMessage();
	//析构函数
	virtual ~CWebMessage();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//状态接口
public:
	//下载异常
	virtual bool OnDownLoadError(enDownLoadError DownLoadError);
	//下载状态
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus);

	//序列接口
public:
	//下载数据
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize);
	//下载信息
	virtual bool OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation);

	//消息定义
protected:
	//绘画消息
	VOID OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif