#include "StdAfx.h"
#include "WebMessage.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWebMessage, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWebMessage::CWebMessage()
{
	//数据变量
	m_dwDataSize=0L;
	m_dwBufferSize=0L;
	m_pcbDataBuffer=NULL;

	return;
}

//析构函数
CWebMessage::~CWebMessage()
{
	//删除内存
	SafeDeleteArray(m_pcbDataBuffer);

	return;
}

//接口查询
VOID * CWebMessage::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);
	return NULL;
}

//下载异常
bool CWebMessage::OnDownLoadError(enDownLoadError DownLoadError)
{
	return true;
}

//下载状态
bool CWebMessage::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	return true;
}

//下载状态
bool CWebMessage::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//效验状态
	ASSERT((m_pcbDataBuffer!=NULL)&&((m_dwDataSize+wStreamSize)<=m_dwBufferSize));
	if ((m_pcbDataBuffer==NULL)||((m_dwDataSize+wStreamSize)>m_dwBufferSize)) return false;

	//拷贝数据
	m_dwDataSize+=wStreamSize;
	CopyMemory(m_pcbDataBuffer+m_dwDataSize-wStreamSize,pcbMailData,wStreamSize);

	return true;
}

//下载开始
bool CWebMessage::OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	//删除内存
	m_dwDataSize=0L;
	m_dwBufferSize=0L;
	SafeDeleteArray(m_pcbDataBuffer);

	//申请内存
	try
	{
		//创建内存
		m_dwDataSize=0L;
		m_dwBufferSize=dwTotalFileSize;
		m_pcbDataBuffer=new BYTE [dwTotalFileSize];

		return true;
	}
	catch (...)
	{
		//错误断言
		ASSERT(FALSE);

		//删除内存
		m_dwDataSize=0L;
		m_dwBufferSize=0L;
		SafeDeleteArray(m_pcbDataBuffer);

		return false;
	}

	return true;
}

//绘画消息
VOID CWebMessage::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CDC BufferDC;
	CImage ImageBuffer;
	BufferDC.CreateCompatibleDC(NULL);
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),16);

	//设置缓冲
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(ImageBuffer);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画背景
	BufferDC.FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),m_crBackGroup);

	//绘画消息
	LPCTSTR pszText=TEXT("系统消息：网狐游戏家园新版本即将推出");
	BufferDC.SetTextColor(RGB(255,255,0));
	rcClient.left+=5;
	BufferDC.DrawText(pszText,lstrlen(pszText),&rcClient,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	rcClient.left-=5;

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	ImageBuffer.Destroy();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
