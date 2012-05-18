#include "StdAfx.h"
#include "DownLoad.h"

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
#pragma comment (lib,"Wininet.lib")

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDownLoad::CDownLoad()
{
	//�ļ���Ϣ
	m_dwTotalSize=0L;
	m_dwDownLoadSize=0L;

	//״̬����
	m_DownLoadStatus=DownLoadStatus_Conclude;
	ZeroMemory(&m_DownLoadInfo,sizeof(m_DownLoadInfo));

	//�ӿڱ���
	m_pIDownLoadSink=NULL;

	//���ýӿ�
	m_WEBSocket.SetWEBSocketSink(QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//��������
CDownLoad::~CDownLoad()
{
	//�ر�����
	CloseDownLoad();

	return;
}

//�ӿڲ�ѯ
VOID * CDownLoad::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IWEBSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IWEBSocketSink,Guid,dwQueryVer);
	return NULL;
}

//�ر�����
bool CDownLoad::CloseDownLoad()
{
	//���ñ���
	m_dwTotalSize=0L;
	m_dwDownLoadSize=0L;
	m_DownLoadStatus=DownLoadStatus_Conclude;

	//״̬����
	m_HeadTranslate.CleanData();

	//�ر�����
	m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

	return true;
}

//ִ������
bool CDownLoad::PerformDownLoad(LPCTSTR pszUrl, IUnknownEx * pIUnknownEx)
{
	//״̬Ч��
	ASSERT(m_DownLoadStatus==DownLoadStatus_Conclude);
	if (m_DownLoadStatus!=DownLoadStatus_Conclude) return false;

	//��ѯ�ӿ�
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadSink)!=NULL);
	m_pIDownLoadSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadSink);

	//��������
	if (ConnectServer(pszUrl)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//������Ϣ
bool CDownLoad::OnSocketLink(INT nErrorCode)
{
	//������
	if (nErrorCode!=0)
	{
		//�ر�����
		CloseDownLoad();

		//״̬֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Connect);

		return false;
	}

	//��������
	SendRequestData(m_DownLoadInfo.szHostName,m_DownLoadInfo.szUrlPath);

	return true;
}

//�ر���Ϣ
bool CDownLoad::OnSocketShut(BYTE cbShutReason)
{
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//�ر�����
		CloseDownLoad();

		//״̬֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Server);
	}

	return true;
}

//���ݰ���
VOID CDownLoad::OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize)
{
	//Ч��״̬
	ASSERT(m_DownLoadStatus==DownLoadStatus_Service);
	if (m_DownLoadStatus!=DownLoadStatus_Service) return;

	//���ñ���
	m_dwDownLoadSize+=wStreamSize;

	//���ݴ���
	ASSERT(m_pIDownLoadSink!=NULL);
	if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDataStream(pcbMailData,wStreamSize);

	//����ж�
	if (m_dwDownLoadSize==m_dwTotalSize)
	{
		//�ر�����
		CloseDownLoad();

		//״̬֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadStatus(DownLoadStatus_Conclude);
	}

	return;
}

//���ݰ�ͷ
VOID CDownLoad::OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode)
{
	switch (nStatusCode)
	{
	case HTTP_STATUS_OK:				//��ȡ�ɹ�
		{
			//���ñ���
			m_DownLoadStatus=DownLoadStatus_Service;
			m_HeadTranslate.SetHeadData((BYTE *)pcbHeadData,wHeadSize);

			//�ļ���Ϣ
			m_dwDownLoadSize=0L;
			m_dwTotalSize=m_HeadTranslate.GetTotalFileSize();

			//״̬֪ͨ
			ASSERT(m_pIDownLoadSink!=NULL);
			if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadStatus(DownLoadStatus_Service);

			//�¼�֪ͨ
			LPCTSTR pszEntityTag=m_HeadTranslate.GetFileEntityTag();
			m_pIDownLoadSink->OnDataInformation(m_dwTotalSize,pszEntityTag,m_DownLoadInfo.szUrlPath);

			return;
		}
	case HTTP_STATUS_MOVED:				//Ŀ����ת
	case HTTP_STATUS_REDIRECT:			//Ŀ����ת
	case HTTP_STATUS_REDIRECT_METHOD:	//Ŀ����ת
		{
			//��ȡλ��
			LPCSTR pszLocation=strstr((CHAR *)pcbHeadData,"Location: ");
			if (pszLocation==NULL) m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

			//��������
			CHAR szLocation[MAX_PATH]="";
			WORD wStartPos=(WORD)strlen("Location: ");
			
			//�����ַ
			WORD wOff=0;
			while (pszLocation[wStartPos+wOff]!='\r')
			{
				//Ч������
				ASSERT(wOff<CountArray(szLocation));
				if (wOff>=CountArray(szLocation)) m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

				//�����ַ�
				WORD wIndex=wOff++;
				szLocation[wIndex]=pszLocation[wStartPos+wIndex];
			}
			szLocation[wOff]=0;

			//ת�����
			if (szLocation[0]=='/')
			{
				//��������
				CA2CT strLocation(szLocation);
				lstrcpyn(m_DownLoadInfo.szUrlPath,(LPCTSTR)strLocation,CountArray(m_DownLoadInfo.szUrlPath));

				//��������
				SendRequestData(m_DownLoadInfo.szHostName,m_DownLoadInfo.szUrlPath);
			}
			else
			{
				//�ر�����
				m_WEBSocket.CloseSocket(SHUT_REASON_INSIDE);

				//��������
				ConnectServer((LPCTSTR)(CA2CT(szLocation)));
			}

			return;
		}
	default:							//Ĭ�ϴ���
		{
			//�ر�����
			CloseDownLoad();

			//״̬֪ͨ
			ASSERT(m_pIDownLoadSink!=NULL);
			if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Server);

			return;
		}
	}

	return;
}

//��������
bool CDownLoad::ConnectServer(LPCTSTR pszUrl)
{
	//��������
	BYTE cbResult=CONNECT_EXCEPTION;
	INTERNET_SCHEME nScheme=GetDownLoadInfo(pszUrl,m_DownLoadInfo);

	//���ͷ���
	switch (nScheme)
	{
	case INTERNET_SERVICE_FTP:		//FTP ����
		{
			cbResult=m_WEBSocket.ConnectServer(m_DownLoadInfo.szHostName,m_DownLoadInfo.wHostPort);
			break;
		}
	case INTERNET_SERVICE_HTTP:		//HTTP ����
		{
			cbResult=m_WEBSocket.ConnectServer(m_DownLoadInfo.szHostName,m_DownLoadInfo.wHostPort);
			break;
		}
	default:
		{
			ASSERT(FALSE);
			break;
		}
	}

	//�������
	if (cbResult==CONNECT_SUCCESS)
	{
		//����״̬
		m_DownLoadStatus=DownLoadStatus_Connect;

		//״̬֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadStatus(DownLoadStatus_Connect);
	}
	else
	{
		//ֹͣ����
		CloseDownLoad();

		//�¼�֪ͨ
		ASSERT(m_pIDownLoadSink!=NULL);
		if (m_pIDownLoadSink!=NULL) m_pIDownLoadSink->OnDownLoadError(DownLoadError_Server);

		return false;
	}

	return true;
}

//��������
bool CDownLoad::SendRequestData(LPCTSTR pszHostName, LPCTSTR pszUrlPath)
{
	//��������
	CHAR szRequestData[1024]="";
	CT2CA strUrlPath(pszUrlPath);
	CT2CA strHostName(pszHostName);

	//��������
	_snprintf(szRequestData,CountArray(szRequestData),"GET /%s HTTP/1.1\r\nHost:%s\r\nAccept:*/*\r\n"
		"User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)\r\nConnection:Keep-Alive\r\n\r\n",
		(LPCSTR)strUrlPath,(LPCSTR)strHostName);

	//��������
	m_WEBSocket.SendRequestData(szRequestData,(WORD)strlen(szRequestData));

	return true;
}

//��ȡ��Ϣ
INTERNET_SCHEME CDownLoad::GetDownLoadInfo(LPCTSTR pszUrl, tagDownLoadInfo & DownLoadInfo)
{
	//��������
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	ZeroMemory(&DownLoadInfo,sizeof(DownLoadInfo));

	//�淶��ַ
	DWORD dwMaxLength=CountArray(szDownLoadUrl);
	DWORD dwFlags=ICU_BROWSER_MODE|ICU_NO_ENCODE|ICU_NO_META|ICU_ENCODE_SPACES_ONLY;
	if (InternetCanonicalizeUrl(pszUrl,szDownLoadUrl,&dwMaxLength,dwFlags)==FALSE) return INTERNET_SCHEME_UNKNOWN;

	//��ʽ��ַ
	URL_COMPONENTS Components;
	ZeroMemory(&Components,sizeof(Components));

	//���ñ���
	Components.dwStructSize=sizeof(Components);
	Components.lpszUrlPath=DownLoadInfo.szUrlPath;
	Components.lpszHostName=DownLoadInfo.szHostName;
	Components.dwUrlPathLength=CountArray(DownLoadInfo.szUrlPath);
	Components.dwHostNameLength=CountArray(DownLoadInfo.szHostName);
	if (InternetCrackUrl(szDownLoadUrl,0,0,&Components)==FALSE) return INTERNET_SCHEME_UNKNOWN;

	//���ñ���
	DownLoadInfo.wHostPort=Components.nPort;

	return Components.nScheme;
}

//////////////////////////////////////////////////////////////////////////////////
