#include "Stdafx.h"
#include "WEBSocket.h"

//////////////////////////////////////////////////////////////////////////////////

//�궨��
#define ID_SOCKET_WND				100									//���ڱ�ʶ
#define WM_SOCKET_NOTIFY			WM_USER+100							//��Ϣ��ʶ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWEBSocket, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY,OnSocketNotify)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWEBSocket::CWEBSocket()
{
	//��������
	m_wRequestID=1;
	m_bHeadValid=false;
	m_cbSocketStatus=SOCKET_STATUS_IDLE;

	//���ݰ�ͷ
	m_wHeadSize=0;
	ZeroMemory(m_szHeadData,sizeof(m_szHeadData));

	//�������
	m_dwBufferData=0L;
	m_dwBufferSize=0L;
	m_bNeedBuffer=false;
	m_pcbDataBuffer=NULL;

	//�ں˱���
	m_pIWEBSocketSink=NULL;
	m_hSocket=INVALID_SOCKET;

	//������Ϣ
	m_cbProxyType=PROXY_NONE;
	ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

	return;
}

//��������
CWEBSocket::~CWEBSocket()
{
	//�ر�����
	if (m_hSocket!=INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
	}

	//ɾ������
	SafeDeleteArray(m_pcbDataBuffer);

	return;
}

//���ýӿ�
bool CWEBSocket::SetWEBSocketSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IWEBSocketSink)!=NULL);
		m_pIWEBSocketSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IWEBSocketSink);

		//�ɹ��ж�
		if (m_pIWEBSocketSink==NULL) return false;
	}
	else m_pIWEBSocketSink=NULL;

	return true;
}

//���ô���
bool CWEBSocket::SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer)
{
	switch (cbProxyType)
	{
	case PROXY_NONE:	//û�д���
		{
			//���ñ���
			m_cbProxyType=cbProxyType;
			ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

			return true;
		}
	case PROXY_HTTP:	//HTTP ����
	case PROXY_SOCKS4:	//SOKKS4 ����
	case PROXY_SOCKS5:	//SOKKS5 ����
		{
			//���ñ���
			m_cbProxyType=cbProxyType;
			m_ProxyServer=ProxyServer;

			return true;
		}
	default: 
		{ 
			ASSERT(FALSE); 
			return false;
		}
	}

	return false;
}

//�ر�����
VOID CWEBSocket::CloseSocket(BYTE cbShutReason)
{
	//�رմ���
	DestroyWindow();

	//�������
	m_dwBufferData=0L;
	m_dwBufferSize=0L;
	m_bNeedBuffer=false;
	SafeDeleteArray(m_pcbDataBuffer);

	//״̬����
	m_bHeadValid=false;
	m_cbSocketStatus=SOCKET_STATUS_IDLE;
	m_wRequestID=__max(1,m_wRequestID+1);

	//�ر��ж�
	if (m_hSocket!=INVALID_SOCKET)
	{
		//�ر�����
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;

		//�¼�֪ͨ
		if (cbShutReason!=SHUT_REASON_INSIDE)
		{
			ASSERT(m_pIWEBSocketSink!=NULL);
			if (m_pIWEBSocketSink!=NULL) m_pIWEBSocketSink->OnSocketShut(cbShutReason);
		}
	}

	return;
}

//���Ӳ���
BYTE CWEBSocket::ConnectServer(LPCTSTR pszServer, WORD wPort)
{
	//Ч�����
	ASSERT(m_hSocket==INVALID_SOCKET);
	ASSERT(m_cbSocketStatus==SOCKET_STATUS_IDLE);

	//���ò���
	m_wHeadSize=0;
	ZeroMemory(m_szHeadData,sizeof(m_szHeadData));

	//ת����ַ
	CT2CA HttpServer(pszServer);
	DWORD dwServerIP=inet_addr(HttpServer);

	//��������
	if (dwServerIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(HttpServer);
		if (lpHost!=NULL) dwServerIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}

	try
	{
		//���� SOCKET
		m_hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (m_hSocket==INVALID_SOCKET) throw CONNECT_EXCEPTION;

		//��������
		if (m_hWnd==NULL)
		{
			CRect rcSocketWnd;
			Create(NULL,NULL,WS_CHILD,rcSocketWnd,GetDesktopWindow(),100);
		}

		//��������
		SOCKADDR_IN SocketAddr;
		ZeroMemory(&SocketAddr,sizeof(SocketAddr));

		//���ñ���
		SocketAddr.sin_family=AF_INET;
		SocketAddr.sin_port=htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr=dwServerIP;

		//��������
		if (m_cbProxyType!=PROXY_NONE)
		{
			//���Ӵ���
			DWORD dwResult=ConnectProxyServer();
			if (dwResult!=CONNECT_SUCCESS) throw dwResult;

			//��������
			switch (m_cbProxyType)
			{
			case PROXY_HTTP:	//HTTP ����
				{
					//��������
					char cbDataBuffer[512];
					BYTE * pcbServerIP=(BYTE *)&dwServerIP;
					_snprintf(cbDataBuffer,CountArray(cbDataBuffer),"CONNECT %d.%d.%d.%d:%d HTTP/1.0\r\nUser-Agent: WHPlatform/0.1\r\n\r\n",
						pcbServerIP[0],pcbServerIP[1],pcbServerIP[2],pcbServerIP[3],wPort);

					//��������
					INT nRequestLen=(INT)strlen(cbDataBuffer);
					if (send(m_hSocket,cbDataBuffer,nRequestLen,0)!=nRequestLen) throw CONNECT_PROXY_FAILURE;

					//������Ӧ
					INT nRecvCount=recv(m_hSocket,cbDataBuffer,sizeof(cbDataBuffer),0);
					if (nRecvCount>0) cbDataBuffer[nRecvCount]=0;

					//����ж�
					bool bSuccess=false;
					for (INT i=0;i<nRecvCount;i++)
					{
						if ((cbDataBuffer[i]==' ')&&(cbDataBuffer[i+1]=='2')&&(cbDataBuffer[i+2]=='0')&&(cbDataBuffer[i+3]=='0'))
						{
							bSuccess=true;
							break;
						}
					}

					//����ж�
					if (bSuccess==false) throw CONNECT_PROXY_FAILURE;

					break;
				}
			case PROXY_SOCKS5:	//Socks5 ����
				{
					//��������
					INT nOff=4;
					char cbDataBuffer[256]={5,1,0,1};

					//���ӵ�ַ
					CopyMemory(cbDataBuffer+nOff,&SocketAddr.sin_addr,sizeof(SocketAddr.sin_addr));
					nOff+=sizeof(SocketAddr.sin_addr);
					
					//���Ӷ˿�
					*(WORD*)(cbDataBuffer+nOff)=SocketAddr.sin_port;
					nOff+=sizeof(WORD);

					//��������
					if (send(m_hSocket,cbDataBuffer,nOff,0)!=nOff) throw CONNECT_PROXY_FAILURE;

					//������Ӧ
					INT nRecvCount=recv(m_hSocket,cbDataBuffer,4,0);
					if (nRecvCount!=4) throw CONNECT_PROXY_FAILURE;
					if (cbDataBuffer[0]!=5) throw CONNECT_PROXY_FAILURE;

					//ʧ���ж�
					if (cbDataBuffer[1]==0) throw CONNECT_PROXY_FAILURE;

					//��������
					INT nLeftDataLen=(cbDataBuffer[3]==1)?6:cbDataBuffer[4]+2;
					if (recv(m_hSocket,cbDataBuffer,nLeftDataLen,0)!=nLeftDataLen) throw CONNECT_PROXY_FAILURE;

					break;
				}
			case PROXY_SOCKS4:	//Socks4 ����
				{
					//��������
					INT nOff=2;
					char cbDataBuffer[256]={4,1};

					//���Ӷ˿�
					*(WORD*)(cbDataBuffer+nOff)=SocketAddr.sin_port;
					nOff+=sizeof(WORD);

					//���ӵ�ַ
					CopyMemory(cbDataBuffer+nOff,&SocketAddr.sin_addr,sizeof(SocketAddr.sin_addr));
					nOff+=sizeof(SocketAddr.sin_addr);

					//�����ֶ�
					cbDataBuffer[nOff++]=0;

					//��������
					INT nResult=send(m_hSocket,cbDataBuffer,nOff,0);
					if (nResult!=nOff) throw CONNECT_PROXY_FAILURE;

					//������Ӧ
					nResult=recv(m_hSocket,cbDataBuffer,8,0);
					if (nResult!=8) throw CONNECT_PROXY_FAILURE;
					if (cbDataBuffer[0]!=0) throw CONNECT_PROXY_FAILURE;

					//ʧ���ж�
					if (cbDataBuffer[1]!=90) throw CONNECT_PROXY_FAILURE;

					break;
				}
			default:		//Ĭ�ϴ���
				{
					ASSERT(FALSE);
					throw CONNECT_PROXY_FAILURE;
				}
			}

			//���ӳɹ�
			WSASetLastError(0);

			//���ñ���
			m_cbSocketStatus=SOCKET_STATUS_CONNECT;

			//������Ϣ
			PostMessage(WM_SOCKET_NOTIFY,0,MAKELONG(FD_CONNECT,0));

			//�󶨴���
			WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY,FD_READ|FD_CONNECT|FD_CLOSE|FD_WRITE);
		}
		else
		{
			//�󶨴���
			WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY,FD_READ|FD_CONNECT|FD_CLOSE|FD_WRITE);

			//��������
			INT nErrorCode=connect(m_hSocket,(SOCKADDR *)&SocketAddr,sizeof(SocketAddr));
			if ((nErrorCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSAEWOULDBLOCK)) throw CONNECT_EXCEPTION;

			//���ñ���
			m_cbSocketStatus=SOCKET_STATUS_WAIT;
		}
	}
	catch (...)
	{
		//�ر�����
		CloseSocket(SHUT_REASON_INSIDE);

		return CONNECT_EXCEPTION;
	}

	return CONNECT_SUCCESS;
}

//��������
WORD CWEBSocket::SendRequestData(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT((wDataSize!=0)&&(pData!=NULL));
	if ((wDataSize==0)||(pData==NULL)) return 0L;

	//Ч��״̬
	if (m_hSocket==INVALID_SOCKET) return 0L;
	if (m_cbSocketStatus!=SOCKET_STATUS_CONNECT) return 0L;

	//���ñ���
	m_wHeadSize=0;
	m_bHeadValid=false;
	m_wRequestID=__max(1,m_wRequestID+1);

	//��������
	WORD wTotalCount=0;

	//��������
	while ((m_bNeedBuffer==false)&&(wTotalCount<wDataSize))
	{
		//��������
		INT nSendCount=send(m_hSocket,(char *)pData+wTotalCount,wDataSize-wTotalCount,0);

		//�����ж�
		if (nSendCount==SOCKET_ERROR)
		{
			//�����ж�
			if (WSAGetLastError()==WSAEWOULDBLOCK)
			{
				AmortizeBuffer((LPBYTE)pData+wTotalCount,wDataSize-wTotalCount);
				return wDataSize;
			}

			//�ر�����
			CloseSocket(SHUT_REASON_EXCEPTION);

			return 0L;
		}
		else
		{
			//���ñ���
			wTotalCount+=nSendCount;
		}
	}

	//��������
	if (wTotalCount>wDataSize)
	{
		AmortizeBuffer((LPBYTE)pData+wTotalCount,wDataSize-wTotalCount);
	}

	return wDataSize;
}

//���Ӵ���
BYTE CWEBSocket::ConnectProxyServer()
{
	//Ч��״̬
	ASSERT(m_cbProxyType!=PROXY_NONE);
	if (m_cbProxyType==PROXY_NONE) return CONNECT_EXCEPTION;

	//ת����ַ
	CT2CA ProxyServer(m_ProxyServer.szProxyServer);
	DWORD dwProxyServer=inet_addr(ProxyServer);

	//��������
	if (dwProxyServer==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(ProxyServer);
		if (lpHost!=NULL) dwProxyServer=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}

	//��������
	SOCKADDR_IN SocketAddrProxy;
	ZeroMemory(&SocketAddrProxy,sizeof(SocketAddrProxy));

	//���ñ���
	SocketAddrProxy.sin_family=AF_INET;
	SocketAddrProxy.sin_addr.S_un.S_addr=dwProxyServer;
	SocketAddrProxy.sin_port=htons(m_ProxyServer.wProxyPort);

	//���Ӵ���
	INT nErrorCode=connect(m_hSocket,(sockaddr *)&SocketAddrProxy,sizeof(SocketAddrProxy));
	if (nErrorCode==SOCKET_ERROR) return CONNECT_PROXY_FAILURE;

	//��������
	if (m_cbProxyType==PROXY_SOCKS5)
	{
		//��������
		INT nOff=2;
		char cbDataBuffer[256]={5,1};

		//��������
		cbDataBuffer[nOff++]=(m_ProxyServer.szUserName[0]==0)?0:2;
		if (send(m_hSocket,cbDataBuffer,nOff,0)!=nOff) return CONNECT_PROXY_FAILURE;

		//��������
		if (recv(m_hSocket,cbDataBuffer,2,0)!=2) return CONNECT_PROXY_FAILURE;

		//���ݷ���
		if (cbDataBuffer[0]!=5) return CONNECT_PROXY_FAILURE;
		if ((cbDataBuffer[1]!=0)&&(cbDataBuffer[1]!=2)) return CONNECT_PROXY_FAILURE;

		//�����֤
		if (cbDataBuffer[1]==2)
		{
			//��������
			INT nStringLen=0;
			CT2CA strUserName(m_ProxyServer.szUserName);
			CT2CA strPassword(m_ProxyServer.szPassword);

			//��¼�ʺ�
			nOff=0;
			cbDataBuffer[nOff++]=1;
			nStringLen=lstrlenA(strUserName);
			cbDataBuffer[nOff++]=(char)nStringLen;
			CopyMemory(cbDataBuffer+nOff,(LPCSTR)strUserName,nStringLen);
			nOff+=nStringLen;

			//��¼����
			nStringLen=lstrlenA(strPassword);
			cbDataBuffer[nOff++]=(char)nStringLen;
			CopyMemory(cbDataBuffer+nOff,(LPCSTR)strPassword,nStringLen);
			nOff+=nStringLen;

			//��������
			if (send(m_hSocket,cbDataBuffer,nOff,0)!=nOff) return CONNECT_PROXY_FAILURE;

			//��������
			if (recv(m_hSocket,cbDataBuffer,2,0)!=2) return CONNECT_PROXY_FAILURE;

			//���ݷ���
			if ((cbDataBuffer[0]!=1)||(cbDataBuffer[1]!=0)) return CONNECT_PROXY_USER_INVALID;
		}
	}

	return CONNECT_SUCCESS;
}

//��������
VOID CWEBSocket::AmortizeBuffer(VOID * pData, WORD wDataSize)
{
	//���뻺��
	if ((m_dwBufferData+wDataSize)>m_dwBufferSize)
	{
		//��������
		LPBYTE pcbDataBuffer=NULL;
		LPBYTE pcbDeleteBuffer=m_pcbDataBuffer;

		//�����С
		DWORD dwNeedSize=m_dwBufferData+wDataSize;
		DWORD dwApplySize=__max(dwNeedSize,m_dwBufferSize*2L);

		//���뻺��
		try
		{
			pcbDataBuffer=new BYTE [dwApplySize];
		}
		catch (...)	{ }

		//ʧ���ж�
		if (pcbDataBuffer==NULL)
		{
			CloseSocket(SHUT_REASON_EXCEPTION);
			return;
		}

		//���ñ���
		m_dwBufferSize=dwApplySize;
		m_pcbDataBuffer=pcbDataBuffer;
		CopyMemory(m_pcbDataBuffer,pcbDeleteBuffer,m_dwBufferData);

		//ɾ������
		SafeDeleteArray(pcbDeleteBuffer);
	}

	//���ñ���
	m_bNeedBuffer=true;
	m_dwBufferData+=wDataSize;
	CopyMemory(m_pcbDataBuffer+m_dwBufferData-wDataSize,pData,wDataSize);

	return;
}

//�����ȡ
LRESULT CWEBSocket::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//��ȡ��ͷ
		if (m_bHeadValid==false)
		{
			//��ȡ����
			INT nResult=recv(m_hSocket,m_szHeadData+m_wHeadSize,sizeof(m_szHeadData)-m_wHeadSize,0);
			if (nResult==SOCKET_ERROR) throw TEXT("�������ӹرգ���ȡ����ʧ��");

			//����ͷβ
			WORD wRequestID=m_wRequestID;
			WORD wWholeSize=m_wHeadSize+nResult;
			LPCSTR pszFinality=strstr((LPCSTR)m_szHeadData,"\r\n\r\n");

			//����ж�
			if (pszFinality!=NULL)
			{
				//���ñ���
				m_bHeadValid=true;
				m_wHeadSize=(WORD)(pszFinality-m_szHeadData)+(WORD)strlen("\r\n\r\n");

				//��ȡ״̬
				INT nStatusCode=0;
				for (WORD i=0;i<m_wHeadSize;i++)
				{
					if (m_szHeadData[i]==' ')
					{
						//��������
						WORD nStatusOff=0;
						CHAR szStatusString[16]="";

						//��ȡ�ַ�
						do
						{
							szStatusString[nStatusOff++]=m_szHeadData[i++];
						} while (m_szHeadData[i]!=' ');

						//���ñ���
						szStatusString[nStatusOff]=0;
						nStatusCode=atol(szStatusString);

						break;
					}
				}

				//��ͷ�¼�
				ASSERT(m_pIWEBSocketSink!=NULL);
				m_pIWEBSocketSink->OnSocketDataHead(m_szHeadData,m_wHeadSize,nStatusCode);

				//�����¼�
				if ((wWholeSize>m_wHeadSize)&&(wRequestID==m_wRequestID))
				{
					ASSERT(m_pIWEBSocketSink!=NULL);
					m_pIWEBSocketSink->OnSocketDataMain(m_szHeadData+m_wHeadSize,wWholeSize-m_wHeadSize);
				}
			}

			return 1;
		}
		else
		{
			//��ȡ����
			char cbReadBuffer[4096];
			INT nReadSize=recv(m_hSocket,cbReadBuffer,sizeof(cbReadBuffer),0);
			if (nReadSize==SOCKET_ERROR) throw TEXT("�������ӹرգ���ȡ����ʧ��");

			//�¼�֪ͨ
			ASSERT(m_pIWEBSocketSink!=NULL);
			m_pIWEBSocketSink->OnSocketDataMain(cbReadBuffer,nReadSize);
		}
	}
	catch (...) 
	{ 
		CloseSocket(SHUT_REASON_EXCEPTION);
	}

	return 1;
}

//���緢��
LRESULT CWEBSocket::OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((m_bNeedBuffer==true)&&(m_dwBufferData>0L))
	{
		//��������
		DWORD dwTotalCount=0;
		DWORD dwPacketSize=4096;

		//��������
		while (dwTotalCount<m_dwBufferData)
		{
			//��������
			WORD wSendSize=(WORD)__min(dwPacketSize,m_dwBufferData-dwTotalCount);
			INT nSendCount=send(m_hSocket,(char *)m_pcbDataBuffer+dwTotalCount,wSendSize,0);

			//�����ж�
			if (nSendCount==SOCKET_ERROR)
			{
				//�����ж�
				if (WSAGetLastError()==WSAEWOULDBLOCK)
				{
					//���ñ���
					m_bNeedBuffer=false;
					m_dwBufferData-=dwTotalCount;

					//�ƶ��ڴ�
					if (m_dwBufferData>0L)
					{
						m_bNeedBuffer=true;
						MoveMemory(m_pcbDataBuffer,m_pcbDataBuffer+dwTotalCount,m_dwBufferData);
					}

					return 1L;
				}

				//�ر�����
				CloseSocket(SHUT_REASON_EXCEPTION);

				return 1L;
			}

			//���ñ���
			dwTotalCount+=nSendCount;
		}

		//���ñ���
		m_dwBufferData=0L;
		m_bNeedBuffer=false;
	}

	return 1L;
}

//����ر�
LRESULT CWEBSocket::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	//�ر�����
	CloseSocket(SHUT_REASON_REMOTE);

	return 1;
}

//��������
LRESULT CWEBSocket::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	INT nErrorCode=WSAGETSELECTERROR(lParam);

	//�ж�״̬
	if (nErrorCode==0)
	{
		m_cbSocketStatus=SOCKET_STATUS_CONNECT;
	}
	else
	{
		CloseSocket(SHUT_REASON_INSIDE);
	}

	//�¼�֪ͨ
	ASSERT(m_pIWEBSocketSink!=NULL);
	m_pIWEBSocketSink->OnSocketLink(nErrorCode);

	return 1;
}

//������Ϣ
LRESULT	CWEBSocket::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:		//���ݶ�ȡ
		{ 
			return OnSocketNotifyRead(wParam,lParam); 
		}
	case FD_CLOSE:		//����ر�
		{ 
			return OnSocketNotifyClose(wParam,lParam);	
		}
	case FD_CONNECT:	//��������
		{ 
			return OnSocketNotifyConnect(wParam,lParam); 
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
