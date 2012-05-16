#include "Stdafx.h"
#include "TCPSocket.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define ID_SOCKET_WND				10								//SOCKET ���� ID
#define WM_SOCKET_NOTIFY			WM_USER+10						//SOCKET ��Ϣ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTCPSocket, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY, OnSocketNotify)
END_MESSAGE_MAP()


//���캯��
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

//��������
CTCPSocket::~CTCPSocket()
{
	CloseSocket(SHUT_REASON_NORMAL);
}

//�ӿڲ�ѯ
void * __cdecl CTCPSocket::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocket, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocket, Guid, dwQueryVer);
	return NULL;
}


//���ýӿ�
bool __cdecl CTCPSocket::SetTCPSocketSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx != NULL);
	m_pITCPSocketSink = (ITCPSocketSink *)pIUnknownEx->QueryInterface(IID_ITCPSocketSink, VER_ITCPSocketSink);
	ASSERT(m_pITCPSocketSink != NULL);
	return (m_pITCPSocketSink != NULL);
}

//��ȡ�ӿ�
void * __cdecl CTCPSocket::GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer)
{
	if (m_pITCPSocketSink == NULL) return NULL;
	return m_pITCPSocketSink->QueryInterface(Guid, dwQueryVer);
}

//���ӷ�����
DWORD __cdecl CTCPSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	//Ч�����
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(m_cbSocketStatus == SOCKET_STATUS_IDLE);

	//Ч��״̬
	if (m_hSocket != INVALID_SOCKET) throw TEXT("���� SOCKET ����Ѿ�����");
	if (m_cbSocketStatus != SOCKET_STATUS_IDLE) throw TEXT("����״̬���ǵȴ�����״̬");
	if (dwServerIP == INADDR_NONE) throw TEXT("Ŀ���������ַ��ʽ����ȷ��������ٴγ��ԣ�");

	//���ò���
	m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0x12345678;
	m_dwRecvXorKey = 0x12345678;
	m_dwSendTickCount = GetTickCount() / 1000L;
	m_dwRecvTickCount = GetTickCount() / 1000L;

	try
	{
		//���� SOCKET
		m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_hSocket == INVALID_SOCKET) throw TEXT("SOCKET ����ʧ��");
		if ((m_hWnd == NULL) && (!Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), GetDesktopWindow(), ID_SOCKET_WND, NULL)))
			throw TEXT("SOCKET �ڲ����ڴ���ʧ��");

		//��д��������ַ
		SOCKADDR_IN SocketAddr;
		memset(&SocketAddr, 0, sizeof(SocketAddr));
		SocketAddr.sin_family = AF_INET;
		SocketAddr.sin_port = htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr = dwServerIP;

		//���ӷ�����
		int iErrorCode = 0;
		WSASetLastError(0);
		if (m_ProxyServerType != ProxyType_None)
		{
			throw TEXT("δ֧�ִ��������");
		}
		else
		{
			//�󶨴���
			iErrorCode = WSAAsyncSelect(m_hSocket, m_hWnd, WM_SOCKET_NOTIFY, FD_READ | FD_CONNECT | FD_CLOSE);
			if (iErrorCode == SOCKET_ERROR) throw TEXT("���ڲ����ڴ���");

			//���ӷ�����
			iErrorCode = connect(m_hSocket, (SOCKADDR *) & SocketAddr, sizeof(SocketAddr));
			if (iErrorCode == SOCKET_ERROR)
			{
				iErrorCode = WSAGetLastError();
				if (iErrorCode != WSAEWOULDBLOCK)
				{
					static TCHAR szBuffer[64];
					_snprintf(szBuffer, sizeof(szBuffer), TEXT("���ӷ������󣬴������ [ %d ]"), iErrorCode);
					throw szBuffer;
				}
			}

			//���ñ���
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
		throw TEXT("���Ӳ���δ֪�쳣����");
	}

	return CONNECT_FAILURE;
}

//���ӷ�����
DWORD __cdecl CTCPSocket::Connect(LPCTSTR szServerIP, WORD wPort)
{
	//Ч������
	ASSERT(wPort != 0);
	ASSERT(szServerIP != NULL);
	if ((szServerIP == NULL) || (wPort == 0)) return false;
	return Connect(TranslateAddr(szServerIP), wPort);
}

//���ͺ���
DWORD __cdecl CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч��״̬
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//��������
	BYTE cbDataBuffer[SOCKET_BUFFER];
	CMD_Head * pHead = (CMD_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	//��������
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(CMD_Head), sizeof(cbDataBuffer));

	//��������
	return SendDataBuffer(cbDataBuffer, wSendSize);
}

//���ͺ���
DWORD __cdecl CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч��״̬
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//Ч���С
	ASSERT(wDataSize <= SOCKET_BUFFER);
	if (wDataSize > SOCKET_BUFFER) return false;

	//��������
	BYTE cbDataBuffer[SOCKET_BUFFER];
	CMD_Head * pHead = (CMD_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize > 0)
	{
		ASSERT(pData != NULL);
		CopyMemory(pHead + 1, pData, wDataSize);
	}

	//��������
	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(CMD_Head) + wDataSize, sizeof(cbDataBuffer));

	//��������
	return SendDataBuffer(cbDataBuffer, wSendSize);
}

//�ر�����
VOID __cdecl CTCPSocket::CloseSocket()
{
	//�ر�����
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

	//�ָ�����
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

//�ر�����
VOID CTCPSocket::CloseSocket(BYTE bNotify)
{
	//�ر�����
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

	//�ָ�����
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

//��������
DWORD CTCPSocket::SendDataBuffer(void * pBuffer, WORD wSendSize)
{
	//Ч�����
	ASSERT(wSendSize != 0);
	ASSERT(pBuffer != NULL);

	//��������
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

//���ӳ��
WORD CTCPSocket::SeedRandMap(WORD wSeed)
{
	DWORD dwHold = wSeed;
	return (WORD)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

//ӳ�䷢������
BYTE CTCPSocket::MapSendByte(BYTE const cbData)
{
	BYTE cbMap = g_SendByteMap[(BYTE)(cbData+m_cbSendRound)];
	m_cbSendRound += 3;
	return cbMap;
}

//ӳ���������
BYTE CTCPSocket::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}

//���ͷ�������ַ
DWORD CTCPSocket::TranslateAddr(LPCTSTR pszServerAddr)
{
	//ת����ַ
	DWORD dwServerIP = inet_addr(pszServerAddr);
	if (dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname(pszServerAddr);
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	return dwServerIP;
}

// ���ʹ���
// LPCTSTR CTCPSocket::GetConnectError(int iErrorCode, LPTSTR pszBuffer, WORD wBufferSize)
// {
// 	//Ч�����
// 	ASSERT(pszBuffer != NULL);
// 	if (pszBuffer == NULL) return NULL;
// 
// 	//���ʹ���
// 	switch (iErrorCode)
// 	{
// 		case 0:					//û�д���
// 		{
// 			lstrcpyn(pszBuffer, TEXT("����ִ�гɹ�"), wBufferSize);
// 			break;
// 		}
// 		case WSAEADDRNOTAVAIL:	//��ַ��ʽ����
// 		{
// 			lstrcpyn(pszBuffer, TEXT("Ŀ���������ַ��ʽ����ȷ��������ٴγ��ԣ�"), wBufferSize);
// 			break;
// 		}
// 		case WSAECONNREFUSED:	//������û������
// 		{
// 			lstrcpyn(pszBuffer, TEXT("Ŀ���������æ����û��������"), wBufferSize);
// 			break;
// 		}
// 		case WSAETIMEDOUT:		//���ӳ�ʱ
// 		{
// 			lstrcpyn(pszBuffer, TEXT("���ӳ�ʱ��������Ŀ������������ڻ��߷�������ַ��ʽ����ȷ��"), wBufferSize);
// 			break;
// 		}
// 		case WSAEHOSTUNREACH:
// 		{
// 			lstrcpyn(pszBuffer, TEXT("��������ʧ�ܣ������Ƿ��Ѿ��ɹ����ź����� Internet ��"), wBufferSize);
// 			break;
// 		}
// 		default:				//Ĭ�ϴ���
// 		{
// 			_snprintf(pszBuffer, wBufferSize, TEXT("���Ӵ���ţ�%ld����ϸ������Ϣ��ο����������ֲᣡ"), iErrorCode);
// 			break;
// 		}
// 	}
// 
// 	return pszBuffer;
// }

//��������
WORD CTCPSocket::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	int i = 0;
	//Ч�����
	ASSERT(wDataSize >= sizeof(CMD_Head));
	ASSERT(wBufferSize >= (wDataSize + 2*sizeof(DWORD)));
	ASSERT(wDataSize <= (sizeof(CMD_Head) + SOCKET_BUFFER));

	//��������
	WORD wEncryptSize = wDataSize - sizeof(CMD_Command), wSnapCount = 0;
	if ((wEncryptSize % sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wEncryptSize % sizeof(DWORD);
		memset(pcbDataBuffer + sizeof(CMD_Info) + wEncryptSize, 0, wSnapCount);
	}

	//Ч�������ֽ�ӳ��
	BYTE cbCheckCode = 0;
	for (WORD i = sizeof(CMD_Info); i < wDataSize; i++)
	{
		cbCheckCode += pcbDataBuffer[i];
		pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);
	}

	//��д��Ϣͷ
	CMD_Head * pHead = (CMD_Head *)pcbDataBuffer;
	pHead->CmdInfo.cbCheckCode = ~cbCheckCode + 1;
	pHead->CmdInfo.wPacketSize = wDataSize;
	pHead->CmdInfo.cbVersion = SOCKET_VER;

	//������Կ
	DWORD dwXorKey = m_dwSendXorKey;
	if (m_dwSendPacketCount == 0)
	{
		//���ɵ�һ���������
		GUID Guid;
		CoCreateGuid(&Guid);
		dwXorKey = GetTickCount() * GetTickCount();
		dwXorKey ^= Guid.Data1;
		dwXorKey ^= Guid.Data2;
		dwXorKey ^= Guid.Data3;
		dwXorKey ^= *((DWORD *)Guid.Data4);

		//���ӳ������
		dwXorKey = SeedRandMap((WORD)dwXorKey);
		dwXorKey |= ((DWORD)SeedRandMap((WORD)(dwXorKey >> 16))) << 16;
		dwXorKey ^= g_dwPacketKey;
		m_dwSendXorKey = dwXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	//��������
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

	//������Կ
	if (m_dwSendPacketCount == 0)
	{
		MoveMemory(pcbDataBuffer + sizeof(CMD_Head) + sizeof(DWORD), pcbDataBuffer + sizeof(CMD_Head), wDataSize);
		*((DWORD *)(pcbDataBuffer + sizeof(CMD_Head))) = m_dwSendXorKey;
		pHead->CmdInfo.wPacketSize += sizeof(DWORD);
		wDataSize += sizeof(DWORD);
	}

	//���ñ���
	m_dwSendPacketCount++;
	m_dwSendXorKey = dwXorKey;

	return wDataSize;
}

//��������
WORD CTCPSocket::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	//Ч�����
	ASSERT(m_dwSendPacketCount > 0);
	ASSERT(wDataSize >= sizeof(CMD_Head));
	ASSERT(((CMD_Head *)pcbDataBuffer)->CmdInfo.wPacketSize == wDataSize);

	//��������
	WORD wSnapCount = 0;
	if ((wDataSize % sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wDataSize % sizeof(DWORD);
		memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
	}

	//��������
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

	//Ч�������ֽ�ӳ��
	CMD_Head * pHead = (CMD_Head *)pcbDataBuffer;
	BYTE cbCheckCode = pHead->CmdInfo.cbCheckCode;
	for (int i = sizeof(CMD_Info); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode += pcbDataBuffer[i];
	}
	if (cbCheckCode != 0) throw TEXT("���ݰ�Ч�������");

	return wDataSize;
}

//��������
LRESULT CTCPSocket::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//�ж�״̬
	int iErrorCode = WSAGETSELECTERROR(lParam);
	if (iErrorCode == 0) m_cbSocketStatus = SOCKET_STATUS_CONNECT;
	else CloseSocket(SHUT_REASON_TIME_OUT);

	//����֪ͨ
	m_pITCPSocketSink->OnEventTCPSocketLink(GetSocketID(), GetLastError());

	return 1;
}

//�����ȡ
LRESULT CTCPSocket::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//��ȡ����
		int iRetCode = recv(m_hSocket, (char *)m_cbRecvBuf + m_wRecvSize, sizeof(m_cbRecvBuf) - m_wRecvSize, 0);
		if (iRetCode == SOCKET_ERROR) throw TEXT("�������ӹرգ���ȡ����ʧ��");
		ASSERT(m_dwSendPacketCount > 0);
		m_wRecvSize += iRetCode;
		m_dwRecvTickCount = GetTickCount() / 1000L;

		//��������
		WORD wPacketSize = 0;
		BYTE cbDataBuffer[SOCKET_BUFFER+sizeof(CMD_Head)];
		CMD_Head * pHead = (CMD_Head *)m_cbRecvBuf;

		while (m_wRecvSize >= sizeof(CMD_Head))
		{
			//Ч�����
			wPacketSize = pHead->CmdInfo.wPacketSize;
			ASSERT(pHead->CmdInfo.cbVersion == SOCKET_VER);
			ASSERT(wPacketSize <= (SOCKET_BUFFER + sizeof(CMD_Head)));
			if (pHead->CmdInfo.cbVersion != SOCKET_VER) throw TEXT("���ݰ��汾����");
			if (wPacketSize > (SOCKET_BUFFER + sizeof(CMD_Head))) throw TEXT("���ݰ�̫��");
			if (m_wRecvSize < wPacketSize) return 1;

			//��������
			m_dwRecvPacketCount++;
			CopyMemory(cbDataBuffer, m_cbRecvBuf, wPacketSize);
			m_wRecvSize -= wPacketSize;
			MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);

			//��������
			WORD wRealySize = CrevasseBuffer(cbDataBuffer, wPacketSize);
			ASSERT(wRealySize >= sizeof(CMD_Head));

			//��������
			WORD wDataSize = wRealySize - sizeof(CMD_Head);
			void * pDataBuffer = cbDataBuffer + sizeof(CMD_Head);
			CMD_Command Command = ((CMD_Head *)cbDataBuffer)->CommandInfo;

			//�ں�����
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
					case SUB_KN_DETECT_SOCKET:	//������
					{
						//��������
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
						break;
					}
				}
				continue;
			}

			//��������
			bool bSuccess = m_pITCPSocketSink->OnEventTCPSocketRead(GetSocketID(), Command, pDataBuffer, wDataSize);
			if (bSuccess == false) throw TEXT("�������ݰ�����ʧ��");
		};
	}
	catch (...)
	{
		CloseSocket(SHUT_REASON_EXCEPTION);
	}

	return 1;
}

//����ر�
LRESULT CTCPSocket::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	CloseSocket(SHUT_REASON_REMOTE);
	return 1;
}

//SOCKET ��Ϣ�������
LRESULT	CTCPSocket::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_CONNECT:	//��������
		{
			return OnSocketNotifyConnect(wParam, lParam);
		}
		case FD_READ:		//���ݶ�ȡ
		{
			return OnSocketNotifyRead(wParam, lParam);
		}
		case FD_CLOSE:		//����ر�
		{
			return OnSocketNotifyClose(wParam, lParam);
		}
	}

	return 0;
}


//�������
DWORD __cdecl CTCPSocket::ProxyServerTesting()
{
	return false;
}

//���ô���
bool __cdecl CTCPSocket::SetProxyServerInfo(enProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)
{
	m_ProxyServerType = ProxyServerType;					//��������
	m_ProxyServerInfo = ProxyServerInfo;					//������Ϣ
	return true;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateTCPSocket(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CTCPSocket * pClientSocket = NULL;
	try
	{
		pClientSocket = new CTCPSocket();
		if (pClientSocket == NULL) throw TEXT("����ʧ��");
		void * pObject = pClientSocket->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pClientSocket);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
