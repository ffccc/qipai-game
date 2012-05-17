#include "StdAfx.h"
#include "WebMessage.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWebMessage, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWebMessage::CWebMessage()
{
	//���ݱ���
	m_dwDataSize=0L;
	m_dwBufferSize=0L;
	m_pcbDataBuffer=NULL;

	return;
}

//��������
CWebMessage::~CWebMessage()
{
	//ɾ���ڴ�
	SafeDeleteArray(m_pcbDataBuffer);

	return;
}

//�ӿڲ�ѯ
VOID * CWebMessage::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);
	return NULL;
}

//�����쳣
bool CWebMessage::OnDownLoadError(enDownLoadError DownLoadError)
{
	return true;
}

//����״̬
bool CWebMessage::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	return true;
}

//����״̬
bool CWebMessage::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//Ч��״̬
	ASSERT((m_pcbDataBuffer!=NULL)&&((m_dwDataSize+wStreamSize)<=m_dwBufferSize));
	if ((m_pcbDataBuffer==NULL)||((m_dwDataSize+wStreamSize)>m_dwBufferSize)) return false;

	//��������
	m_dwDataSize+=wStreamSize;
	CopyMemory(m_pcbDataBuffer+m_dwDataSize-wStreamSize,pcbMailData,wStreamSize);

	return true;
}

//���ؿ�ʼ
bool CWebMessage::OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	//ɾ���ڴ�
	m_dwDataSize=0L;
	m_dwBufferSize=0L;
	SafeDeleteArray(m_pcbDataBuffer);

	//�����ڴ�
	try
	{
		//�����ڴ�
		m_dwDataSize=0L;
		m_dwBufferSize=dwTotalFileSize;
		m_pcbDataBuffer=new BYTE [dwTotalFileSize];

		return true;
	}
	catch (...)
	{
		//�������
		ASSERT(FALSE);

		//ɾ���ڴ�
		m_dwDataSize=0L;
		m_dwBufferSize=0L;
		SafeDeleteArray(m_pcbDataBuffer);

		return false;
	}

	return true;
}

//�滭��Ϣ
VOID CWebMessage::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC BufferDC;
	CImage ImageBuffer;
	BufferDC.CreateCompatibleDC(NULL);
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),16);

	//���û���
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(ImageBuffer);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	BufferDC.FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),m_crBackGroup);

	//�滭��Ϣ
	LPCTSTR pszText=TEXT("ϵͳ��Ϣ��������Ϸ��԰�°汾�����Ƴ�");
	BufferDC.SetTextColor(RGB(255,255,0));
	rcClient.left+=5;
	BufferDC.DrawText(pszText,lstrlen(pszText),&rcClient,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	rcClient.left-=5;

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	ImageBuffer.Destroy();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
