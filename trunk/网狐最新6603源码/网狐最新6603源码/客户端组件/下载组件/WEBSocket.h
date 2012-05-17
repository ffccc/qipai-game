#ifndef DOWN_LOAD_SOCKET_HEAD_FILE
#define DOWN_LOAD_SOCKET_HEAD_FILE

#pragma once

#include "WinSock2.h"
#include "DownLoadHead.h"

//////////////////////////////////////////////////////////////////////////////////

//下载连接
class DOWN_LOAD_CLASS CWEBSocket : public CWnd
{
	//状态变量
protected:
	WORD							m_wRequestID;						//请求标识
	bool							m_bHeadValid;						//有效标志
	BYTE							m_cbSocketStatus;					//网络状态

	//数据包头
protected:
	WORD							m_wHeadSize;						//包头长度
	CHAR							m_szHeadData[1024];					//数据包头

	//缓冲变量
protected:
	bool							m_bNeedBuffer;						//缓冲状态
	DWORD							m_dwBufferData;						//缓冲数据
	DWORD							m_dwBufferSize;						//缓冲大小
	LPBYTE							m_pcbDataBuffer;					//缓冲数据

	//内核变量
protected:
	SOCKET							m_hSocket;							//连接句柄
	IWEBSocketSink *				m_pIWEBSocketSink;					//回调接口

	//代理信息
protected:
	BYTE							m_cbProxyType;						//代理类型
	tagProxyServer					m_ProxyServer;						//代理信息

	//函数定义
public:
	//构造函数
	CWEBSocket();
	//析构函数
	virtual ~CWEBSocket();

	//配置函数
public:
	//设置接口
	bool SetWEBSocketSink(IUnknownEx * pIUnknownEx);
	//设置代理
	bool SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer);

	//操作接口
public:
	//关闭连接
	VOID CloseSocket(BYTE cbShutReason);
	//连接操作
	BYTE ConnectServer(LPCTSTR pszServer, WORD wPort);
	//发送请求
	WORD SendRequestData(VOID * pData, WORD wDataSize);

	//辅助函数
protected:
	//连接代理
	BYTE ConnectProxyServer();
	//缓冲数据
	VOID AmortizeBuffer(VOID * pData, WORD wDataSize);

	//处理函数
protected:
	//网络读取
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//网络发送
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	//网络关闭
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//网络连接
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	//消息映射
protected:
	//网络消息
	LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif