#include "Stdafx.h"
#include "TCPSocket.h"

//////////////////////////////////////////////////////////////////////////

//宏定义
#define ID_SOCKET_WND				10								//SOCKET 窗口 ID
#define WM_SOCKET_NOTIFY			WM_USER+10						//SOCKET 消息

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTCPSocket, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY, OnSocketNotify)
END_MESSAGE_MAP()


//构造函数
CTCPSocket::CTCPSocket()
{
	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	m_hSocket = INVALID_SOCKET;
	m_pITCPSocketSink = NULL;
	m_ProxyServerType = ProxyType_None;
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
	memset(&m_ProxyServerInfo, 0, sizeof(tagProxyServerInfo));
}

//析构函数
CTCPSocket::~CTCPSocket()
{
	CloseSocket(SHUT_REASON_NORMAL);
}

//接口查询
void * __cdecl CTCPSocket::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocket, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocket, Guid, dwQueryVer);
	return NULL;
}


//设置接口
bool __cdecl CTCPSocket::SetTCPSocketSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx != NULL);
	m_pITCPSocketSink = (ITCPSocketSink *)pIUnknownEx->QueryInterface(IID_ITCPSocketSink, VER_ITCPSocketSink);
	ASSERT(m_pITCPSocketSink != NULL);
	return (m_pITCPSocketSink != NULL);
}

//获取接口
void * __cdecl CTCPSocket::GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer)
{
	if (m_pITCPSocketSink == NULL) return NULL;
	return m_pITCPSocketSink->QueryInterface(Guid, dwQueryVer);
}

//连接服务器
DWORD __cdecl CTCPSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	//效验参数
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(m_cbSocketStatus == SOCKET_STATUS_IDLE);

	//效验状态
	if (m_hSocket != INVALID_SOCKET) throw TEXT("连接 SOCKET 句柄已经存在");
	if (m_cbSocketStatus != SOCKET_STATUS_IDLE) throw TEXT("连接状态不是等待连接状态");
	if (dwServerIP == INADDR_NONE) throw TEXT("目标服务器地址格式不正确，请检查后再次尝试！");

	//设置参数
	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0x12345678;
	m_dwRecvXorKey = 0x12345678;
	m_dwSendTickCount = GetTickCount() / 1000L;
	m_dwRecvTickCount = GetTickCount() / 1000L;

	try
	{
		//建立 SOCKET
		m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_hSocket == INVALID_SOCKET) throw TEXT("SOCKET 创建失败");
		if ((m_hWnd == NULL) && (!Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), GetDesktopWindow(), ID_SOCKET_WND, NULL)))
			throw TEXT("SOCKET 内部窗口创建失败");

		//填写服务器地址
		SOCKADDR_IN SocketAddr;
		memset(&SocketAddr, 0, sizeof(SocketAddr));
		SocketAddr.sin_family = AF_INET;
		SocketAddr.sin_port = htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr = dwServerIP;

		//连接服务器
		int iErrorCode = 0;
		WSASetLastError(0);
		if (m_ProxyServerType != ProxyType_None)
		{
			throw TEXT("未支持代理服务器");
		}
		else
		{
			//绑定窗口
			iErrorCode = WSAAsyncSelect(m_hSocket, m_hWnd, WM_SOCKET_NOTIFY, FD_READ | FD_CONNECT | FD_CLOSE);
			if (iErrorCode == SOCKET_ERROR) throw TEXT("绑定内部窗口错误");

			//连接服务器
			iErrorCode = connect(m_hSocket, (SOCKADDR *) & SocketAddr, sizeof(SocketAddr));
			if (iErrorCode == SOCKET_ERROR)
			{
				iErrorCode = WSAGetLastError();
				if (iErrorCode != WSAEWOULDBLOCK)
				{
					static TCHAR szBuffer[64];
					_snprintf(szBuffer, sizeof(szBuffer), TEXT("连接发生错误，错误代码 [ %d ]"), iErrorCode);
					throw szBuffer;
				}
			}

			//设置变量
			m_cbSocketStatus = SOCKET_STATUS_WAIT;
		}

		return CONNECT_SUCCESS;
	}
	catch (LPCTSTR pszError)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
		throw pszError;
	}
	catch (...)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
		throw TEXT("连接产生未知异常错误");
	}

	return CONNECT_FAILURE;
}

//连接服务器
DWORD __cdecl CTCPSocket::Connect(LPCTSTR szServerIP, WORD wPort)
{
	//效验数据
	ASSERT(wPort != 0);
	ASSERT(szServerIP != NULL);
	if ((szServerIP == NULL) || (wPort == 0)) return false;
	return Connect(TranslateAddr(szServerIP), wPort);
}

//发送函数
DWORD __cdecl CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//效验状态
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_BUFFER];
	CMD_Head * pHead = (CMD_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	//加密数据
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(CMD_Head), sizeof(cbDataBuffer));

	//发送数据
	return SendDataBuffer(cbDataBuffer, wSendSize);
}

//发送函数
DWORD __cdecl CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//效验状态
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//效验大小
	ASSERT(wDataSize <= SOCKET_BUFFER);
	if (wDataSize > SOCKET_BUFFER) return false;

	//构造数据
	BYTE cbDataBuffer[SOCKET_BUFFER];
	CMD_Head * pHead = (CMD_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize > 0)
	{
		ASSERT(pData != NULL);
		CopyMemory(pHead + 1, pData, wDataSize);
	}

	//加密数据
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(CMD_Head) + wDataSize, sizeof(cbDataBuffer));

	//发送数据
	return SendDataBuffer(cbDataBuffer, wSendSize);
}

//关闭连接
VOID __cdecl CTCPSocket::CloseSocket()
{
	//关闭连接
	bool bClose = (m_hSocket != INVALID_SOCKET);
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
	if (m_hSocket != INVALID_SOCKET)
	{
		WSAAsyncSelect(m_hSocket, GetSafeHwnd(), WM_SOCKET_NOTIFY, 0);
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		m_cbSocketStatus = SOCKET_STATUS_IDLE;
	}
	if ((bClose == true) && (m_pITCPSocketSink != NULL))
	{
		ASSERT(m_pITCPSocketSink != NULL);
		try
		{
			m_pITCPSocketSink->OnEventTCPSocketShut(GetSocketID(), SHUT_REASON_NORMAL);
		}
		catch (...) {}
	}

	//恢复数据
	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
	m_hSocket = INVALID_SOCKET;
}

//关闭连接
VOID CTCPSocket::CloseSocket(BYTE bNotify)
{
	//关闭连接
	bool bClose = (m_hSocket != INVALID_SOCKET);
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
	if (m_hSocket != INVALID_SOCKET)
	{
		WSAAsyncSelect(m_hSocket, GetSafeHwnd(), WM_SOCKET_NOTIFY, 0);
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		m_cbSocketStatus = SOCKET_STATUS_IDLE;
	}
	if ((bClose == true) && (m_pITCPSocketSink != NULL))
	{
		ASSERT(m_pITCPSocketSink != NULL);
		try
		{
			m_pITCPSocketSink->OnEventTCPSocketShut(GetSocketID(), bNotify);
		}
		catch (...) {}
	}

	//恢复数据
	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
	m_hSocket = INVALID_SOCKET;
	m_cbSocketStatus = SOCKET_STATUS_IDLE;
}

//发送数据
DWORD CTCPSocket::SendDataBuffer(void * pBuffer, WORD wSendSize)
{
	//效验参数
	ASSERT(wSendSize != 0);
	ASSERT(pBuffer != NULL);

	//发送数据
	WORD wSended = 0;
	while (wSended < wSendSize)
	{
		int iErrorCode = send(m_hSocket, (char *)pBuffer + wSended, wSendSize - wSended, 0);
		if (iErrorCode == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				m_dwSendTickCount = GetTickCount() / 1000L;
				return wSendSize;
			}
			return 0;
		}
		wSended += iErrorCode;
	}
	m_dwSendTickCount = GetTickCount() / 1000L;

	return wSended;
}

//随机映射
WORD CTCPSocket::SeedRandMap(WORD wSeed)
{
	DWORD dwHold = wSeed;
	return (WORD)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

//映射发送数据
BYTE CTCPSocket::MapSendByte(BYTE const cbData)
{
	BYTE cbMap = g_SendByteMap[(BYTE)(cbData+m_cbSendRound)];
	m_cbSendRound += 3;
	return cbMap;
}

//映射接收数据
BYTE CTCPSocket::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}

//解释服务器地址
DWORD CTCPSocket::TranslateAddr(LPCTSTR pszServerAddr)
{
	//转化地址
	DWORD dwServerIP = inet_addr(pszServerAddr);
	if (dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname(pszServerAddr);
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	return dwServerIP;
}

// 解释错误
// LPCTSTR CTCPSocket::GetConnectError(int iErrorCode, LPTSTR pszBuffer, WORD wBufferSize)
// {
// 	//效验参数
// 	ASSERT(pszBuffer != NULL);
// 	if (pszBuffer == NULL) return NULL;
// 
// 	//解释错误
// 	switch (iErrorCode)
// 	{
// 		case 0:					//没有错误
// 		{
// 			lstrcpyn(pszBuffer, TEXT("操作执行成功"), wBufferSize);
// 			break;
// 		}
// 		case WSAEADDRNOTAVAIL:	//地址格式错误
// 		{
// 			lstrcpyn(pszBuffer, TEXT("目标服务器地址格式不正确，请检查后再次尝试！"), wBufferSize);
// 			break;
// 		}
// 		case WSAECONNREFUSED:	//服务器没有启动
// 		{
// 			lstrcpyn(pszBuffer, TEXT("目标服务器繁忙或者没有启动！"), wBufferSize);
// 			break;
// 		}
// 		case WSAETIMEDOUT:		//连接超时
// 		{
// 			lstrcpyn(pszBuffer, TEXT("连接超时，可能是目标服务器不存在或者服务器地址格式不正确！"), wBufferSize);
// 			break;
// 		}
// 		case WSAEHOSTUNREACH:
// 		{
// 			lstrcpyn(pszBuffer, TEXT("网络连接失败，请检查是否已经成功拨号和连接 Internet ！"), wBufferSize);
// 			break;
// 		}
// 		default:				//默认错误
// 		{
// 			_snprintf(pszBuffer, wBufferSize, TEXT("连接错误号：%ld，详细错误信息请参考操作帮助手册！"), iErrorCode);
// 			break;
// 		}
// 	}
// 
// 	return pszBuffer;
// }

//加密数据
WORD CTCPSocket::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	int i = 0;
	//效验参数
	ASSERT(wDataSize >= sizeof(CMD_Head));
	ASSERT(wBufferSize >= (wDataSize + 2*sizeof(DWORD)));
	ASSERT(wDataSize <= (sizeof(CMD_Head) + SOCKET_BUFFER));

	//调整长度
	WORD wEncryptSize = wDataSize - sizeof(CMD_Command), wSnapCount = 0;
	if ((wEncryptSize % sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wEncryptSize % sizeof(DWORD);
		memset(pcbDataBuffer + sizeof(CMD_Info) + wEncryptSize, 0, wSnapCount);
	}

	//效验码与字节映射
	BYTE cbCheckCode = 0;
	for (WORD i = sizeof(CMD_Info); i < wDataSize; i++)
	{
		cbCheckCode += pcbDataBuffer[i];
		pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);
	}

	//填写信息头
	CMD_Head * pHead = (CMD_Head *)pcbDataBuffer;
	pHead->CmdInfo.cbCheckCode = ~cbCheckCode + 1;
	pHead->CmdInfo.wPacketSize = wDataSize;
	pHead->CmdInfo.cbVersion = SOCKET_VER;

	//创建密钥
	DWORD dwXorKey = m_dwSendXorKey;
	if (m_dwSendPacketCount == 0)
	{
		//生成第一次随机种子
		GUID Guid;
		CoCreateGuid(&Guid);
		dwXorKey = GetTickCount() * GetTickCount();
		dwXorKey ^= Guid.Data1;
		dwXorKey ^= Guid.Data2;
		dwXorKey ^= Guid.Data3;
		dwXorKey ^= *((DWORD *)Guid.Data4);

		//随机映射种子
		dwXorKey = SeedRandMap((WORD)dwXorKey);
		dwXorKey |= ((DWORD)SeedRandMap((WORD)(dwXorKey >> 16))) << 16;
		dwXorKey ^= g_dwPacketKey;
		m_dwSendXorKey = dwXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//加密数据
	WORD * pwSeed = (WORD *)(pcbDataBuffer + sizeof(CMD_Info));
	DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(CMD_Info));
	WORD wEncrypCount = (wEncryptSize + wSnapCount) / sizeof(DWORD);
	for (WORD i = 0; i < wEncrypCount; i++)
	{
		*pdwXor++ ^= dwXorKey;
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwPacketKey;
	}

	//插入密钥
	if (m_dwSendPacketCount == 0)
	{
		MoveMemory(pcbDataBuffer + sizeof(CMD_Head) + sizeof(DWORD), pcbDataBuffer + sizeof(CMD_Head), wDataSize);
		*((DWORD *)(pcbDataBuffer + sizeof(CMD_Head))) = m_dwSendXorKey;
		pHead->CmdInfo.wPacketSize += sizeof(DWORD);
		wDataSize += sizeof(DWORD);
	}

	//设置变量
	m_dwSendPacketCount++;
	m_dwSendXorKey = dwXorKey;

	return wDataSize;
}

//解密数据
WORD CTCPSocket::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	//效验参数
	ASSERT(m_dwSendPacketCount > 0);
	ASSERT(wDataSize >= sizeof(CMD_Head));
	ASSERT(((CMD_Head *)pcbDataBuffer)->CmdInfo.wPacketSize == wDataSize);

	//调整长度
	WORD wSnapCount = 0;
	if ((wDataSize % sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wDataSize % sizeof(DWORD);
		memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
	}

	//解密数据
	DWORD dwXorKey = m_dwRecvXorKey;
	DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(CMD_Info));
	WORD  * pwSeed = (WORD *)(pcbDataBuffer + sizeof(CMD_Info));
	WORD wEncrypCount = (wDataSize + wSnapCount - sizeof(CMD_Info)) / 4;
	for (WORD i = 0; i < wEncrypCount; i++)
	{
		if ((i == (wEncrypCount - 1)) && (wSnapCount > 0))
		{
			BYTE * pcbKey = ((BYTE *) & m_dwRecvXorKey) + sizeof(DWORD) - wSnapCount;
			CopyMemory(pcbDataBuffer + wDataSize, pcbKey, wSnapCount);
		}
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwPacketKey;
		*pdwXor++ ^= m_dwRecvXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//效验码与字节映射
	CMD_Head * pHead = (CMD_Head *)pcbDataBuffer;
	BYTE cbCheckCode = pHead->CmdInfo.cbCheckCode;
	for (int i = sizeof(CMD_Info); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode += pcbDataBuffer[i];
	}
	if (cbCheckCode != 0) throw TEXT("数据包效验码错误");

	return wDataSize;
}

//网络连接
LRESULT CTCPSocket::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//判断状态
	int iErrorCode = WSAGETSELECTERROR(lParam);
	if (iErrorCode == 0) m_cbSocketStatus = SOCKET_STATUS_CONNECT;
	else CloseSocket(SHUT_REASON_TIME_OUT);

	//发送通知
	m_pITCPSocketSink->OnEventTCPSocketLink(GetSocketID(), GetLastError());

	return 1;
}

//网络读取
LRESULT CTCPSocket::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//读取数据
		int iRetCode = recv(m_hSocket, (char *)m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize, 0);
		if (iRetCode == SOCKET_ERROR) throw TEXT("网络连接关闭，读取数据失败");
		ASSERT(m_dwSendPacketCount > 0);
		m_wRecvSize += iRetCode;
		m_dwRecvTickCount = GetTickCount() / 1000L;

		//变量定义
		WORD wPacketSize = 0;
		BYTE cbDataBuffer[SOCKET_BUFFER+sizeof(CMD_Head)];
		CMD_Head * pHead = (CMD_Head *)m_cbRecvBuf;

		while (m_wRecvSize >= sizeof(CMD_Head))
		{
			//效验参数
			wPacketSize = pHead->CmdInfo.wPacketSize;
			ASSERT(pHead->CmdInfo.cbVersion == SOCKET_VER);
			ASSERT(wPacketSize <= (SOCKET_BUFFER + sizeof(CMD_Head)));
			if (pHead->CmdInfo.cbVersion != SOCKET_VER) throw TEXT("数据包版本错误");
			if (wPacketSize > (SOCKET_BUFFER + sizeof(CMD_Head))) throw TEXT("数据包太大");
			if (m_wRecvSize < wPacketSize) return 1;

			//拷贝数据
			m_dwRecvPacketCount++;
			CopyMemory(cbDataBuffer, m_cbRecvBuf, wPacketSize);
			m_wRecvSize -= wPacketSize;
			MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);

			//解密数据
			WORD wRealySize = CrevasseBuffer(cbDataBuffer, wPacketSize);
			ASSERT(wRealySize >= sizeof(CMD_Head));

			//解释数据
			WORD wDataSize = wRealySize - sizeof(CMD_Head);
			void * pDataBuffer = cbDataBuffer + sizeof(CMD_Head);
			CMD_Command Command = ((CMD_Head *)cbDataBuffer)->CommandInfo;

			//内核命令
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
					case SUB_KN_DETECT_SOCKET:	//网络检测
					{
						//发送数据
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
						break;
					}
				}
				continue;
			}

			//处理数据
			bool bSuccess = m_pITCPSocketSink->OnEventTCPSocketRead(GetSocketID(), Command, pDataBuffer, wDataSize);
			if (bSuccess == false) throw TEXT("网络数据包处理失败");
		};
	}
	catch (...)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
	}

	return 1;
}

//网络关闭
LRESULT CTCPSocket::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	CloseSocket(SHUT_REASON_REMOTE);
	return 1;
}

//SOCKET 消息处理程序
LRESULT	CTCPSocket::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_CONNECT:	//网络连接
		{
			return OnSocketNotifyConnect(wParam, lParam);
		}
		case FD_READ:		//数据读取
		{
			return OnSocketNotifyRead(wParam, lParam);
		}
		case FD_CLOSE:		//网络关闭
		{
			return OnSocketNotifyClose(wParam, lParam);
		}
	}

	return 0;
}


//代理测试
DWORD __cdecl CTCPSocket::ProxyServerTesting()
{
	return false;
}

//设置代理
bool __cdecl CTCPSocket::SetProxyServerInfo(enProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)
{
	m_ProxyServerType = ProxyServerType;					//代理类型
	m_ProxyServerInfo = ProxyServerInfo;					//代理信息
	return true;
}

//////////////////////////////////////////////////////////////////////////

//建立对象函数
extern "C" __declspec(dllexport) void * __cdecl CreateTCPSocket(const GUID & Guid, DWORD dwInterfaceVer)
{
	//建立对象
	CTCPSocket * pClientSocket = NULL;
	try
	{
		pClientSocket = new CTCPSocket();
		if (pClientSocket == NULL) throw TEXT("创建失败");
		void * pObject = pClientSocket->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pClientSocket);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
