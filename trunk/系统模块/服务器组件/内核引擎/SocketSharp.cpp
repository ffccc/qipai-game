#include "StdAfx.h"
#include "SocketSharp.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define TIME_BREAK_READY			9000L								//�ж�ʱ��
#define TIME_BREAK_CONNECT			4000L								//�ж�ʱ��
#define TIME_DETECT_SOCKET			20000L								//���ʱ��

//////////////////////////////////////////////////////////////////////////

//��������
#define QUEUE_SEND_REQUEST			1									//���ͱ�ʶ
#define QUEUE_SAFE_CLOSE			2									//��ȫ�ر�
#define QUEUE_ALLOW_BATCH			3									//����Ⱥ��
#define QUEUE_DETECT_SOCKET			4									//�������

//��������ṹ
struct tagSendDataRequest
{
	WORD							wMainCmdID;							//��������
	WORD							wSubCmdID;							//��������
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	WORD							wDataSize;							//���ݴ�С
	BYTE							cbSendBuf[SOCKET_PACKET];			//���ͻ���
};

//����Ⱥ��
struct tagAllowBatchSend
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	BYTE							cbAllow;							//�����־
};

//��ȫ�ر�
struct tagSafeCloseSocket
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
};

//////////////////////////////////////////////////////////////////////////
/*
//�����ֽ�ӳ���
const BYTE g_SendByteMap[256]=				
{
	0x70,0x2F,0x40,0x5F,0x44,0x8E,0x6E,0x45,0x7E,0xAB,0x2C,0x1F,0xB4,0xAC,0x9D,0x91,
	0x0D,0x36,0x9B,0x0B,0xD4,0xC4,0x39,0x74,0xBF,0x23,0x16,0x14,0x06,0xEB,0x04,0x3E,
	0x12,0x5C,0x8B,0xBC,0x61,0x63,0xF6,0xA5,0xE1,0x65,0xD8,0xF5,0x5A,0x07,0xF0,0x13,
	0xF2,0x20,0x6B,0x4A,0x24,0x59,0x89,0x64,0xD7,0x42,0x6A,0x5E,0x3D,0x0A,0x77,0xE0,
	0x80,0x27,0xB8,0xC5,0x8C,0x0E,0xFA,0x8A,0xD5,0x29,0x56,0x57,0x6C,0x53,0x67,0x41,
	0xE8,0x00,0x1A,0xCE,0x86,0x83,0xB0,0x22,0x28,0x4D,0x3F,0x26,0x46,0x4F,0x6F,0x2B,
	0x72,0x3A,0xF1,0x8D,0x97,0x95,0x49,0x84,0xE5,0xE3,0x79,0x8F,0x51,0x10,0xA8,0x82,
	0xC6,0xDD,0xFF,0xFC,0xE4,0xCF,0xB3,0x09,0x5D,0xEA,0x9C,0x34,0xF9,0x17,0x9F,0xDA,
	0x87,0xF8,0x15,0x05,0x3C,0xD3,0xA4,0x85,0x2E,0xFB,0xEE,0x47,0x3B,0xEF,0x37,0x7F,
	0x93,0xAF,0x69,0x0C,0x71,0x31,0xDE,0x21,0x75,0xA0,0xAA,0xBA,0x7C,0x38,0x02,0xB7,
	0x81,0x01,0xFD,0xE7,0x1D,0xCC,0xCD,0xBD,0x1B,0x7A,0x2A,0xAD,0x66,0xBE,0x55,0x33,
	0x03,0xDB,0x88,0xB2,0x1E,0x4E,0xB9,0xE6,0xC2,0xF7,0xCB,0x7D,0xC9,0x62,0xC3,0xA6,
	0xDC,0xA7,0x50,0xB5,0x4B,0x94,0xC0,0x92,0x4C,0x11,0x5B,0x78,0xD9,0xB1,0xED,0x19,
	0xE9,0xA1,0x1C,0xB6,0x32,0x99,0xA3,0x76,0x9E,0x7B,0x6D,0x9A,0x30,0xD6,0xA9,0x25,
	0xC7,0xAE,0x96,0x35,0xD0,0xBB,0xD2,0xC8,0xA2,0x08,0xF3,0xD1,0x73,0xF4,0x48,0x2D,
	0x90,0xCA,0xE2,0x58,0xC1,0x18,0x52,0xFE,0xDF,0x68,0x98,0x54,0xEC,0x60,0x43,0x0F
};

//�����ֽ�ӳ���
const BYTE g_RecvByteMap[256]=				
{
	0x51,0xA1,0x9E,0xB0,0x1E,0x83,0x1C,0x2D,0xE9,0x77,0x3D,0x13,0x93,0x10,0x45,0xFF,
	0x6D,0xC9,0x20,0x2F,0x1B,0x82,0x1A,0x7D,0xF5,0xCF,0x52,0xA8,0xD2,0xA4,0xB4,0x0B,
	0x31,0x97,0x57,0x19,0x34,0xDF,0x5B,0x41,0x58,0x49,0xAA,0x5F,0x0A,0xEF,0x88,0x01,
	0xDC,0x95,0xD4,0xAF,0x7B,0xE3,0x11,0x8E,0x9D,0x16,0x61,0x8C,0x84,0x3C,0x1F,0x5A,
	0x02,0x4F,0x39,0xFE,0x04,0x07,0x5C,0x8B,0xEE,0x66,0x33,0xC4,0xC8,0x59,0xB5,0x5D,
	0xC2,0x6C,0xF6,0x4D,0xFB,0xAE,0x4A,0x4B,0xF3,0x35,0x2C,0xCA,0x21,0x78,0x3B,0x03,
	0xFD,0x24,0xBD,0x25,0x37,0x29,0xAC,0x4E,0xF9,0x92,0x3A,0x32,0x4C,0xDA,0x06,0x5E,
	0x00,0x94,0x60,0xEC,0x17,0x98,0xD7,0x3E,0xCB,0x6A,0xA9,0xD9,0x9C,0xBB,0x08,0x8F,
	0x40,0xA0,0x6F,0x55,0x67,0x87,0x54,0x80,0xB2,0x36,0x47,0x22,0x44,0x63,0x05,0x6B,
	0xF0,0x0F,0xC7,0x90,0xC5,0x65,0xE2,0x64,0xFA,0xD5,0xDB,0x12,0x7A,0x0E,0xD8,0x7E,
	0x99,0xD1,0xE8,0xD6,0x86,0x27,0xBF,0xC1,0x6E,0xDE,0x9A,0x09,0x0D,0xAB,0xE1,0x91,
	0x56,0xCD,0xB3,0x76,0x0C,0xC3,0xD3,0x9F,0x42,0xB6,0x9B,0xE5,0x23,0xA7,0xAD,0x18,
	0xC6,0xF4,0xB8,0xBE,0x15,0x43,0x70,0xE0,0xE7,0xBC,0xF1,0xBA,0xA5,0xA6,0x53,0x75,
	0xE4,0xEB,0xE6,0x85,0x14,0x48,0xDD,0x38,0x2A,0xCC,0x7F,0xB1,0xC0,0x71,0x96,0xF8,
	0x3F,0x28,0xF2,0x69,0x74,0x68,0xB7,0xA3,0x50,0xD0,0x79,0x1D,0xFC,0xCE,0x8A,0x8D,
	0x2E,0x62,0x30,0xEA,0xED,0x2B,0x26,0xB9,0x81,0x7C,0x46,0x89,0x73,0xA2,0xF7,0x72
};

//���ݼ�����Կ
const static DWORD g_dwPacketKey=0xA5BAA55A;
*/
//////////////////////////////////////////////////////////////////////////

//���캯��
COverLapped::COverLapped(enOperationType OperationType) : m_OperationType(OperationType)
{
	memset(&m_WSABuffer,0,sizeof(m_WSABuffer));
	memset(&m_OverLapped,0,sizeof(m_OverLapped));
}

//��������
COverLapped::~COverLapped()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLappedSend::COverLappedSend() : COverLapped(OperationType_Send)
{
	m_WSABuffer.len=0;
	m_WSABuffer.buf=(char *)m_cbBuffer;
}

//��������
COverLappedSend::~COverLappedSend()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerSocketItem::CServerSocketItem(WORD wIndex, IServerSocketItemSink * pIServerSocketItemSink) 
	: m_wIndex(wIndex), m_pIServerSocketItemSink(pIServerSocketItemSink)
{
	m_wRountID=0;
	m_wRecvSize=0;
	m_cbSendRound=0;
	m_cbRecvRound=0;
	m_bNotify=true;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_bAllowBatch=false;
	m_dwSendXorKey=0;
	m_dwRecvXorKey=0;
	m_dwClientAddr=0;
	m_dwConnectTime=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;
	m_hSocket=INVALID_SOCKET;
}

//��������
CServerSocketItem::~CServerSocketItem(void)
{
	//ɾ�������ص� IO
	INT_PTR iCount=m_OverLappedSendFree.GetCount();
	for (INT_PTR i=0;i<iCount;i++) delete m_OverLappedSendFree[i];
	m_OverLappedSendFree.RemoveAll();

	//ɾ����ص� IO
	iCount=m_OverLappedSendActive.GetCount();
	for (INT_PTR i=0;i<iCount;i++) delete m_OverLappedSendActive[i];
	m_OverLappedSendActive.RemoveAll();

	return;
}

//���ӳ��
WORD CServerSocketItem::SeedRandMap(WORD wSeed)
{
	DWORD dwHold=wSeed;
	return (WORD)((dwHold=dwHold*241103L+2533101L)>>16);
}

//ӳ�䷢������
BYTE CServerSocketItem::MapSendByte(BYTE const cbData)
{
	BYTE cbMap=g_SendByteMap[(BYTE)(cbData+m_cbSendRound)];
	m_cbSendRound+=3;
	return cbMap;
}

//ӳ���������
BYTE CServerSocketItem::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap=g_RecvByteMap[cbData]-m_cbRecvRound;
	m_cbRecvRound+=3;
	return cbMap;
}

//��ȡ���ͽṹ
COverLappedSend * CServerSocketItem::GetSendOverLapped()
{
	//Ѱ�ҿ��нṹ
	COverLappedSend * pOverLappedSend=NULL;
	INT_PTR nFreeCount=m_OverLappedSendFree.GetCount();
	if (nFreeCount>0)
	{
		pOverLappedSend=m_OverLappedSendFree[nFreeCount-1];
		ASSERT(pOverLappedSend!=NULL);
		m_OverLappedSendFree.RemoveAt(nFreeCount-1);
		m_OverLappedSendActive.Add(pOverLappedSend);
		return pOverLappedSend;
	}

	//�½����ͽṹ
	try
	{
		pOverLappedSend=new COverLappedSend;
		ASSERT(pOverLappedSend!=NULL);
		m_OverLappedSendActive.Add(pOverLappedSend);
		return pOverLappedSend;
	}
	catch (...) { }
	return NULL;
}

//�󶨶���
DWORD CServerSocketItem::Attach(SOCKET hSocket, DWORD dwClientAddr)
{
	//Ч������
	ASSERT(dwClientAddr!=0);
	ASSERT(m_bRecvIng==false);
	ASSERT(IsValidSocket()==false);
	ASSERT(hSocket!=INVALID_SOCKET);

	//���ñ���
	m_bNotify=false;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_hSocket=hSocket;
	m_dwClientAddr=dwClientAddr;
	m_dwRecvTickCount=GetTickCount();
	m_dwConnectTime=(DWORD)time(NULL);

	//����֪ͨ
	m_pIServerSocketItemSink->OnSocketAcceptEvent(this);

	return GetIdentifierID();
}

//���ͺ���
bool CServerSocketItem::SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//Ч��״̬
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;

	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//��������
	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	WORD wSendSize=EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head),sizeof(pOverLappedSend->m_cbBuffer));
	pOverLappedSend->m_WSABuffer.len=wSendSize;

	//��������
	if (m_OverLappedSendActive.GetCount()==1)
	{
		DWORD dwThancferred=0;
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			OnSendCompleted(pOverLappedSend,0L);
			return false;
		}
	}

	return true;
}

//���ͺ���
bool CServerSocketItem::SendData(void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//Ч�����
	ASSERT(wDataSize<=SOCKET_PACKET);

	//Ч��״̬
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;
	if (wDataSize>SOCKET_PACKET) return false;

	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//��������
	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		memcpy(pHead+1,pData,wDataSize);
	}
	WORD wSendSize=EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head)+wDataSize,sizeof(pOverLappedSend->m_cbBuffer));
	pOverLappedSend->m_WSABuffer.len=wSendSize;

	//��������
	if (m_OverLappedSendActive.GetCount()==1)
	{
		DWORD dwThancferred=0;
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			OnSendCompleted(pOverLappedSend,0L);
			return false;
		}
	}

	return true;
}

//Ͷ�ݽ���
bool CServerSocketItem::RecvData()
{
	//Ч�����
	ASSERT(m_bRecvIng==false);
	ASSERT(m_hSocket!=INVALID_SOCKET);

	//�жϹر�
	if (m_bCloseIng==true)
	{
		if (m_OverLappedSendActive.GetCount()==0) CloseSocket(m_wRountID);
		return false;
	}

	//��������
	m_bRecvIng=true;
	DWORD dwThancferred=0,dwFlags=0;
	int iRetCode=WSARecv(m_hSocket,&m_OverLappedRecv.m_WSABuffer,1,&dwThancferred,&dwFlags,&m_OverLappedRecv.m_OverLapped,NULL);
	if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
	{
		m_bRecvIng=false;
		CloseSocket(m_wRountID);
		return false;
	}

	return true;
}

//�ر�����
bool CServerSocketItem::CloseSocket(WORD wRountID)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;

	//�ر�����
	if (m_hSocket!=INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
	}

	//�жϹر�
	if ((m_bRecvIng==false)&&(m_OverLappedSendActive.GetCount()==0)) OnCloseCompleted();

	return true;
}

//���ùر�
bool CServerSocketItem::ShutDownSocket(WORD wRountID)
{
	return true;

	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;

	//���ñ���
	if (m_bCloseIng==false)
	{
		m_bCloseIng=true;
		if (m_OverLappedSendActive.GetCount()==0) CloseSocket(wRountID);
	}

	return true;
}

//����Ⱥ��
bool CServerSocketItem::AllowBatchSend(WORD wRountID, bool bAllowBatch)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;

	//���ñ���
	m_bAllowBatch=bAllowBatch;

	return true;
}

//���ñ���
void CServerSocketItem::ResetSocketData()
{
	//Ч��״̬
	ASSERT(m_hSocket==INVALID_SOCKET);

	//��������
	m_wRountID++;
	m_wRecvSize=0;
	m_cbSendRound=0;
	m_cbRecvRound=0;
	m_dwSendXorKey=0;
	m_dwRecvXorKey=0;
	m_dwClientAddr=0;
	m_dwConnectTime=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;

	//״̬����
	m_bNotify=true;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_bAllowBatch=false;
	m_OverLappedSendFree.Append(m_OverLappedSendActive);
	m_OverLappedSendActive.RemoveAll();

	return;
}

//������ɺ���
bool CServerSocketItem::OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred)
{
	//Ч�����
	ASSERT(pOverLappedSend!=NULL);
	ASSERT(m_OverLappedSendActive.GetCount()>0);
	ASSERT(pOverLappedSend==m_OverLappedSendActive[0]);

	//�ͷŷ��ͽṹ
	m_OverLappedSendActive.RemoveAt(0);
	m_OverLappedSendFree.Add(pOverLappedSend);

	//���ñ���
	if (dwThancferred!=0) m_dwSendTickCount=GetTickCount();

	//�жϹر�
	if (m_hSocket==INVALID_SOCKET)
	{
		m_OverLappedSendFree.Append(m_OverLappedSendActive);
		m_OverLappedSendActive.RemoveAll();
		CloseSocket(m_wRountID);
		return true;
	}

	//������������
	if (m_OverLappedSendActive.GetCount()>0)
	{
		DWORD dwThancferred=0;
		pOverLappedSend=m_OverLappedSendActive[0];
		ASSERT(pOverLappedSend!=NULL);
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			m_OverLappedSendFree.Append(m_OverLappedSendActive);
			m_OverLappedSendActive.RemoveAll();
			CloseSocket(m_wRountID);
			return false;
		}
		return true;
	}

	//�жϹر�
	if (m_bCloseIng==true) 
		CloseSocket(m_wRountID);

	return true;
}

//������ɺ���
bool CServerSocketItem::OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred)
{
	//Ч������
	ASSERT(m_bRecvIng==true);

	//���ñ���
	m_bRecvIng=false;
	m_dwRecvTickCount=GetTickCount();

	//�жϹر�
	if (m_hSocket==INVALID_SOCKET)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//��������
	int iRetCode=recv(m_hSocket,(char *)m_cbRecvBuf+m_wRecvSize,sizeof(m_cbRecvBuf)-m_wRecvSize,0);
	if (iRetCode<=0)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//�������
	m_wRecvSize+=iRetCode;
	BYTE cbBuffer[SOCKET_BUFFER];
	CMD_Head * pHead=(CMD_Head *)m_cbRecvBuf;

	//��������
	try
	{
		while (m_wRecvSize>=sizeof(CMD_Head))
		{
			//Ч������
			WORD wPacketSize=pHead->CmdInfo.wPacketSize;
			if (wPacketSize>SOCKET_BUFFER) throw TEXT("���ݰ�����");
			if (wPacketSize<sizeof(CMD_Head)) throw TEXT("���ݰ��Ƿ�");
			if (pHead->CmdInfo.cbVersion!=SOCKET_VER) throw TEXT("���ݰ��汾����");
			if (m_wRecvSize<wPacketSize) break;

			//��ȡ����
			CopyMemory(cbBuffer,m_cbRecvBuf,wPacketSize);
			WORD wRealySize=CrevasseBuffer(cbBuffer,wPacketSize);
			ASSERT(wRealySize>=sizeof(CMD_Head));
			m_dwRecvPacketCount++;

			//��������
			WORD wDataSize=wRealySize-sizeof(CMD_Head);
			void * pDataBuffer=cbBuffer+sizeof(CMD_Head);
			CMD_Command Command=((CMD_Head *)cbBuffer)->CommandInfo;

			//�ں�����
			if (Command.wMainCmdID==MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
				case SUB_KN_DETECT_SOCKET:	//������
					{
						break;
					}
				default: throw TEXT("�Ƿ�������");
				}
			}
			else 
			{
				//��Ϣ����
				m_pIServerSocketItemSink->OnSocketReadEvent(Command,pDataBuffer,wDataSize,this);			
			}

			//ɾ����������
			m_wRecvSize-=wPacketSize;
			MoveMemory(m_cbRecvBuf,m_cbRecvBuf+wPacketSize,m_wRecvSize);
		}
	}
	catch (...)
	{ 
		CloseSocket(m_wRountID);
		return false;
	}

	return RecvData();
}

//�ر����֪ͨ
bool CServerSocketItem::OnCloseCompleted()
{
	//Ч��״̬
	ASSERT(m_hSocket==INVALID_SOCKET);
	ASSERT(m_OverLappedSendActive.GetCount()==0);

	//�ر��¼�
	ASSERT(m_bNotify==false);
	if (m_bNotify==false)
	{
		m_bNotify=true;
		m_pIServerSocketItemSink->OnSocketCloseEvent(this);
	}

	//״̬����
	ResetSocketData();

	return true;
}

//��������
WORD CServerSocketItem::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	WORD i = 0;
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_Head));
	ASSERT(wDataSize<=(sizeof(CMD_Head)+SOCKET_PACKET));
	ASSERT(wBufferSize>=(wDataSize+2*sizeof(DWORD)));

	//��������
	WORD wEncryptSize=wDataSize-sizeof(CMD_Info),wSnapCount=0;
	if ((wEncryptSize%sizeof(DWORD))!=0)
	{
		wSnapCount=sizeof(DWORD)-wEncryptSize%sizeof(DWORD);
		memset(pcbDataBuffer+sizeof(CMD_Info)+wEncryptSize,0,wSnapCount);
	}

	//Ч�������ֽ�ӳ��
	BYTE cbCheckCode=0;
	for (i=sizeof(CMD_Info);i<wDataSize;i++) 
	{
		cbCheckCode+=pcbDataBuffer[i];
		pcbDataBuffer[i]=MapSendByte(pcbDataBuffer[i]);
	}

	//��д��Ϣͷ
	CMD_Head * pHead=(CMD_Head *)pcbDataBuffer;
	pHead->CmdInfo.cbCheckCode=~cbCheckCode+1;
	pHead->CmdInfo.wPacketSize=wDataSize;
	pHead->CmdInfo.cbVersion=SOCKET_VER;

	//��������
	DWORD dwXorKey=m_dwSendXorKey;
	WORD * pwSeed=(WORD *)(pcbDataBuffer+sizeof(CMD_Info));
	DWORD * pdwXor=(DWORD *)(pcbDataBuffer+sizeof(CMD_Info));
	WORD wEncrypCount=(wEncryptSize+wSnapCount)/sizeof(DWORD);
	for (i=0;i<wEncrypCount;i++)
	{
		*pdwXor++^=dwXorKey;
		dwXorKey=SeedRandMap(*pwSeed++);
		dwXorKey|=((DWORD)SeedRandMap(*pwSeed++))<<16; 
		dwXorKey^=g_dwPacketKey;
	}

	//���ñ���
	m_dwSendPacketCount++;
	m_dwSendXorKey=dwXorKey;

	return wDataSize;
}

//��������
WORD CServerSocketItem::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	WORD i = 0;
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_Head));
	ASSERT(((CMD_Head *)pcbDataBuffer)->CmdInfo.wPacketSize==wDataSize);

	//��������
	WORD wSnapCount=0;
	if ((wDataSize%sizeof(DWORD))!=0)
	{
		wSnapCount=sizeof(DWORD)-wDataSize%sizeof(DWORD);
		memset(pcbDataBuffer+wDataSize,0,wSnapCount);
	}

	//��ȡ��Կ
	if (m_dwRecvPacketCount==0)
	{
		ASSERT(wDataSize>=(sizeof(CMD_Head)+sizeof(DWORD)));
		if (wDataSize<(sizeof(CMD_Head)+sizeof(DWORD))) throw TEXT("���ݰ����ܳ��ȴ���");
		m_dwRecvXorKey=*(DWORD *)(pcbDataBuffer+sizeof(CMD_Head));
		m_dwSendXorKey=m_dwRecvXorKey;
		MoveMemory(pcbDataBuffer+sizeof(CMD_Head),pcbDataBuffer+sizeof(CMD_Head)+sizeof(DWORD),
			wDataSize-sizeof(CMD_Head)-sizeof(DWORD));
		wDataSize-=sizeof(DWORD);
		((CMD_Head *)pcbDataBuffer)->CmdInfo.wPacketSize-=sizeof(DWORD);
	}

	//��������
	DWORD dwXorKey=m_dwRecvXorKey;
	DWORD * pdwXor=(DWORD *)(pcbDataBuffer+sizeof(CMD_Info));
	WORD  * pwSeed=(WORD *)(pcbDataBuffer+sizeof(CMD_Info));
	WORD wEncrypCount=(wDataSize+wSnapCount-sizeof(CMD_Info))/4;
	for (i=0;i<wEncrypCount;i++)
	{
		if ((i==(wEncrypCount-1))&&(wSnapCount>0))
		{
			BYTE * pcbKey=((BYTE *)&m_dwRecvXorKey)+sizeof(DWORD)-wSnapCount;
			CopyMemory(pcbDataBuffer+wDataSize,pcbKey,wSnapCount);
		}
		dwXorKey=SeedRandMap(*pwSeed++);
		dwXorKey|=((DWORD)SeedRandMap(*pwSeed++))<<16;
		dwXorKey^=g_dwPacketKey;
		*pdwXor++^=m_dwRecvXorKey;
		m_dwRecvXorKey=dwXorKey;
	}

	//Ч�������ֽ�ӳ��
	CMD_Head * pHead=(CMD_Head *)pcbDataBuffer;
	BYTE cbCheckCode=pHead->CmdInfo.cbCheckCode;;
	for (i=sizeof(CMD_Info);i<wDataSize;i++)
	{
		pcbDataBuffer[i]=MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode+=pcbDataBuffer[i];
	}
	if (cbCheckCode!=0) throw TEXT("���ݰ�Ч�������");

	return wDataSize;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerSocketRSThread::CServerSocketRSThread(void)
{
	m_hCompletionPort=NULL;
}

//��������
CServerSocketRSThread::~CServerSocketRSThread(void)
{
}

//���ú���
bool CServerSocketRSThread::InitThread(HANDLE hCompletionPort)
{
	ASSERT(hCompletionPort!=NULL);
	m_hCompletionPort=hCompletionPort;
	return true;
}

//���к���
bool CServerSocketRSThread::RepetitionRun()
{
	//Ч�����
	ASSERT(m_hCompletionPort!=NULL);

	//��������
	DWORD dwThancferred=0;					
	OVERLAPPED * pOverLapped=NULL;
	COverLapped * pSocketLapped=NULL;
	CServerSocketItem * pServerSocketItem=NULL;

	//�ȴ���ɶ˿�
	BOOL bSuccess=GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pServerSocketItem,&pOverLapped,INFINITE);
	if ((bSuccess==FALSE)&&(GetLastError()!=ERROR_NETNAME_DELETED)) return false;
	if ((pServerSocketItem==NULL)&&(pOverLapped==NULL)) return false;

	//�������
	ASSERT(pOverLapped!=NULL);
	ASSERT(pServerSocketItem!=NULL);
	pSocketLapped=CONTAINING_RECORD(pOverLapped,COverLapped,m_OverLapped);
	switch (pSocketLapped->GetOperationType())
	{
	case OperationType_Recv:	//SOCKET ���ݶ�ȡ
		{
			COverLappedRecv * pOverLappedRecv=(COverLappedRecv *)pSocketLapped;
			CThreadLock SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->OnRecvCompleted(pOverLappedRecv,dwThancferred);
			break;
		}
	case OperationType_Send:	//SOCKET ���ݷ���
		{
			COverLappedSend * pOverLappedSend=(COverLappedSend *)pSocketLapped;
			CThreadLock SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->OnSendCompleted(pOverLappedSend,dwThancferred);
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketAcceptThread::CSocketAcceptThread(void)
{
	m_hCompletionPort=NULL;
	m_pTCPSocketManager=NULL;
	m_hListenSocket=INVALID_SOCKET;
}

//��������
CSocketAcceptThread::~CSocketAcceptThread(void)
{
}

//���ú���
bool CSocketAcceptThread::InitThread(HANDLE hCompletionPort, SOCKET hListenSocket, CTCPSocketService * pTCPSocketManager)
{
	ASSERT(hCompletionPort!=NULL);
	ASSERT(pTCPSocketManager!=NULL);
	ASSERT(hListenSocket!=INVALID_SOCKET);
	m_hListenSocket=hListenSocket;
	m_hCompletionPort=hCompletionPort;
	m_pTCPSocketManager=pTCPSocketManager;
	return true;
}

//���к���
bool CSocketAcceptThread::RepetitionRun()
{
	//Ч�����
	ASSERT(m_hCompletionPort!=NULL);
	ASSERT(m_pTCPSocketManager!=NULL);

	//���ñ���
	SOCKADDR_IN	SocketAddr;
	CServerSocketItem * pServerSocketItem=NULL;
	SOCKET hConnectSocket=INVALID_SOCKET;
	int nBufferSize=sizeof(SocketAddr);

	try
	{
		//��������
		hConnectSocket=WSAAccept(m_hListenSocket,(SOCKADDR *)&SocketAddr,&nBufferSize,NULL,NULL);
		if (hConnectSocket==INVALID_SOCKET) return false;

		//��ȡ����
		pServerSocketItem=m_pTCPSocketManager->ActiveSocketItem();
		if (pServerSocketItem==NULL) throw TEXT("�������Ӷ���ʧ��");

		//�������
		CThreadLock SocketLockHandle(pServerSocketItem->GetSignedLock());
		pServerSocketItem->Attach(hConnectSocket,SocketAddr.sin_addr.S_un.S_addr);
		CreateIoCompletionPort((HANDLE)hConnectSocket,m_hCompletionPort,(ULONG_PTR)pServerSocketItem,0);
		pServerSocketItem->RecvData();
	}
	catch (...)
	{
		//�������
		ASSERT(pServerSocketItem==NULL);
		if (hConnectSocket!=INVALID_SOCKET)	closesocket(hConnectSocket);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketDetectThread::CSocketDetectThread(void)
{
	m_dwTickCount=0;;
	m_pTCPSocketManager=NULL;
}

//��������
CSocketDetectThread::~CSocketDetectThread(void)
{
}

//���ú���
bool CSocketDetectThread::InitThread(CTCPSocketService * pTCPSocketManager)
{
	//Ч�����
	ASSERT(pTCPSocketManager!=NULL);

	//���ñ���
	m_dwTickCount=0L;
	m_pTCPSocketManager=pTCPSocketManager;

	return true;
}

//���к���
bool CSocketDetectThread::RepetitionRun()
{
	//Ч�����
	ASSERT(m_pTCPSocketManager!=NULL);

	//���ü��
	Sleep(500);
	m_dwTickCount+=500L;

	//�������
	if (m_dwTickCount>20000L)
	{
		m_dwTickCount=0L;
		m_pTCPSocketManager->DetectSocket();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////