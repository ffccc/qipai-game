#include "StdAfx.h"
#include "DlgDownLoad.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////

//位置定义
#define CX_BORAD					5									//边框宽度
#define CY_BORAD					5									//边框高度

//数目定义
#define MIN_LINE					3									//可视数目
#define MAX_LINE					6									//可视数目
#define FLASH_ITEM_TIMES			8									//闪烁次数

//时间标识
#define IDI_FLASH_ITEM				100									//进程检测
#define IDI_UPDATE_STATUS			101									//更新状态

//时间标识
#define IDI_CHECK_PROCESS			200									//进程检测

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDownLoadSink, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgDownLoad, CSkinDialog)
	
	//系统消息
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_WINDOWPOSCHANGED()

	//按钮消息
	ON_BN_CLICKED(IDC_LAST, OnBnClickedLast)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_CANCEL_ONE, OnBnClickedCancelOne)
	ON_BN_CLICKED(IDC_RETRY_AGAIN, OnBnClickedRetryAgain)

	//自定消息
	ON_MESSAGE(WM_ITEM_STATUS_UPDATE,OnItemStatusUpdateMessage)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDownLoadSink::CDownLoadSink()
{
	//下载速度
	m_dwDownSpeed=0L;
	m_dwLastCalcSize=0L;
	m_dwLastCalcTime=0L;

	//文件信息
	m_dwDownLoadSize=0L;
	m_dwTotalFileSize=0L;

	//状态变量
	m_cbDownLoadStatus=DOWN_LOAD_IDLE;

	//任务属性
	m_wKindID=0;
	m_wServerID=0;
	ZeroMemory(m_szClientName,sizeof(m_szClientName));

	//内核信息
	m_pDlgDownLoad=NULL;
	ZeroMemory(&m_ProcessInfoMation,sizeof(m_ProcessInfoMation));

	return;
}

//析构函数
CDownLoadSink::~CDownLoadSink()
{
}

//接口查询
VOID * CDownLoadSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadSink,Guid,dwQueryVer);
	return NULL;
}

//关闭下载
bool CDownLoadSink::CloseDownLoad()
{
	//关闭窗口
	DestroyWindow();

	//关闭下载
	m_DownLoad.CloseDownLoad();

	//设置状态
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//关闭文件
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) m_DownFile.Close();

	//发送状态
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	return true;
}

//执行下载
bool CDownLoadSink::PerformDownLoad(LPCTSTR pszClientName, WORD wKindID, WORD wServerID)
{
	//效验状态
	ASSERT((m_cbDownLoadStatus==DOWN_LOAD_IDLE)||(m_cbDownLoadStatus==DOWN_LOAD_ERROR));
	if ((m_cbDownLoadStatus!=DOWN_LOAD_IDLE)&&(m_cbDownLoadStatus!=DOWN_LOAD_ERROR)) return false;

	//下载速度
	m_dwDownSpeed=0L;
	m_dwLastCalcSize=0L;
	m_dwLastCalcTime=GetTickCount();

	//设置变量
	m_wKindID=wKindID;
	m_wServerID=wServerID;
	m_cbDownLoadStatus=DOWN_LOAD_LINK;
	lstrcpyn(m_szClientName,pszClientName,CountArray(m_szClientName));

	//构造地址
	TCHAR szDownLoadUrl[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadUrl,CountArray(szDownLoadUrl),TEXT("%s/DownLoad.asp?KindID=%ld"),szPlatformLink,wKindID);

	//发送状态
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	//执行下载
	m_DownLoad.PerformDownLoad(szDownLoadUrl,QUERY_ME_INTERFACE(IUnknownEx));

	return true;
}

//下载异常
bool CDownLoadSink::OnDownLoadError(enDownLoadError DownLoadError)
{
	//设置变量
	m_cbDownLoadStatus=DOWN_LOAD_ERROR;

	//发送状态
	m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

	return true;
}

//下载状态
bool CDownLoadSink::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	//设置状态
	switch (DownLoadStatus)
	{
	case DownLoadStatus_Service:		//服务状态
		{
			//设置状态
			m_cbDownLoadStatus=DOWN_LOAD_READ;

			//发送状态
			m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

			break;
		}
	case DownLoadStatus_Conclude:		//完成状态
		{
			//设置状态
			m_cbDownLoadStatus=DOWN_LOAD_SETUP;

			//安装文件
			if (PerformInstall()==false)
			{
				//关闭下载
				CloseDownLoad();

				return false;
			}

			//发送状态
			m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);

			break;
		}
	}

	return true;
}

//下载数据
bool CDownLoadSink::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//写入文件
	m_dwDownLoadSize+=wStreamSize;
	m_DownFile.Write(pcbMailData,wStreamSize);

	//计算速度
	if (GetTickCount()>=(m_dwLastCalcTime+2000L))
	{
		//计算速度
		m_dwDownSpeed=(m_dwDownLoadSize-m_dwLastCalcSize)/(GetTickCount()-m_dwLastCalcTime);

		//设置变量
		m_dwLastCalcTime=GetTickCount();
		m_dwLastCalcSize=m_dwDownLoadSize;
	}
	
	return true;
}

//下载信息
bool CDownLoadSink::OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	//设置变量
	m_dwTotalFileSize+=dwTotalFileSize;

	//创建文件
	if (CreateDownFile(pszLocation)==false)
	{
		//关闭下载
		CloseDownLoad();

		return false;
	}

	return true;
}

//执行安装
bool CDownLoadSink::PerformInstall()
{
	//下载文件
	TCHAR szTargetName[MAX_PATH]=TEXT("");
	CString strSourceName=m_DownFile.GetFilePath();

	//变量定义
	INT nTailCount=lstrlen(TEXT(".WHD"));
	INT nSourceLength=lstrlen(strSourceName);

	//构造名字
	szTargetName[nSourceLength-nTailCount]=0;
	CopyMemory(szTargetName,(LPCTSTR)strSourceName,sizeof(TCHAR)*(nSourceLength-nTailCount));

	//关闭文件
	m_DownFile.Close();

	//删除文件
	DeleteFile(szTargetName);

	//移动文件
	if (MoveFileWithProgress(strSourceName,szTargetName,NULL,NULL,MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//构造命令
	TCHAR szCommandLine[MAX_PATH];
	_sntprintf(szCommandLine,CountArray(szCommandLine),TEXT("%s /VERYSILENT"),szTargetName);

	//变量定义
	STARTUPINFO StartInfo;
	ZeroMemory(&StartInfo,sizeof(StartInfo));

	//启动进程
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWDEFAULT;
	if (CreateProcess(NULL,szCommandLine,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_ProcessInfoMation)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建窗口
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,m_pDlgDownLoad,100);
	}

	//设置时间
	SetTimer(IDI_CHECK_PROCESS,100,NULL);

	return true;
}

//创建文件
bool CDownLoadSink::CreateDownFile(LPCTSTR pszLocation)
{
	//效验状态
	ASSERT(m_DownFile.m_hFile==INVALID_HANDLE_VALUE);
	if (m_DownFile.m_hFile!=INVALID_HANDLE_VALUE) return false;

	//工作目录
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//目标目录
	TCHAR szDownLoadPath[MAX_PATH]=TEXT("");
	_sntprintf(szDownLoadPath,CountArray(szDownLoadPath),TEXT("%s\\DownLoad"),szWorkDirectory);

	//文件名字
	DWORD dwIndex=lstrlen(pszLocation);
	while ((dwIndex>0)&&(pszLocation[dwIndex]!=TEXT('/'))) dwIndex--;

	//文件路径
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	LPCTSTR pszFileName=&pszLocation[dwIndex+1];
	_sntprintf(szFilePath,CountArray(szFilePath),TEXT("%s\\%s.WHD"),szDownLoadPath,pszFileName);

	//创建目录
	CreateDirectory(szDownLoadPath,NULL);

	//创建文件
	if (m_DownFile.Open(szFilePath,CFile::modeCreate|CFile::modeWrite)==FALSE) return false;

	return true;
}

//时间消息
VOID CDownLoadSink::OnTimer(UINT nIDEvent)
{
	//进程检测
	if (nIDEvent==IDI_CHECK_PROCESS)
	{
		//状态效验
		if (m_ProcessInfoMation.hProcess==NULL)
		{
			//错误断言
			ASSERT(FALSE);

			//关闭下载
			CloseDownLoad();

			return;
		}

		//进程检测
		if (WaitForSingleObject(m_ProcessInfoMation.hProcess,0L)==WAIT_OBJECT_0)
		{
			//关闭窗口
			DestroyWindow();

			//设置状态
			m_cbDownLoadStatus=DOWN_LOAD_FINISH;

			//发送状态
			m_pDlgDownLoad->SendMessage(WM_ITEM_STATUS_UPDATE,(WPARAM)this,0);
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgDownLoad::CDlgDownLoad() : CSkinDialog(IDD_DLG_DOWNLOAD)
{
	//状态变量
	m_bCreate=false;
	m_wCurrentPage=0;
	m_wViewPageCount=0;
	m_wViewLineCount=0;

	//子项状态
	m_wFocusItem=INVALID_WORD;
	m_wHoverItem=INVALID_WORD;

	//闪烁变量
	m_wFlashItem=INVALID_WORD;
	m_wFlashCount=INVALID_WORD;

	//获取大小
	CImage ImageDownLoadItem;
	ImageDownLoadItem.LoadFromResource(AfxGetInstanceHandle(),IDB_DOWN_LOAD_ITEM);
	m_SizeItemImage.SetSize(ImageDownLoadItem.GetWidth(),ImageDownLoadItem.GetHeight()/5L);

	//获取大小
	CPngImage ImageDownLoadScale;
	ImageDownLoadScale.LoadImage(AfxGetInstanceHandle(),TEXT("DOWN_LOAD_SCALE"));
	m_SizeScaleImage.SetSize(ImageDownLoadScale.GetWidth(),ImageDownLoadScale.GetHeight()/2);

	return;
}

//析构函数
CDlgDownLoad::~CDlgDownLoad()
{
}

//控件绑定
VOID CDlgDownLoad::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAST, m_btLast);
	DDX_Control(pDX, IDC_NEXT, m_btNext);
	DDX_Control(pDX, IDCANCEL, m_btCancelAll);
	DDX_Control(pDX, IDC_CANCEL_ONE, m_btCancelOne);
	DDX_Control(pDX, IDC_RETRY_AGAIN, m_btRetryAgain);
}

//消息解释
BOOL CDlgDownLoad::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CDlgDownLoad::OnInitDialog()
{
	__super::OnInitDialog();

	//状态变量
	m_bCreate=true;
	m_wCurrentPage=0;
	m_wViewPageCount=0;
	m_wViewLineCount=0;

	//子项状态
	m_wFocusItem=INVALID_WORD;
	m_wHoverItem=INVALID_WORD;

	//闪烁变量
	m_wFlashItem=INVALID_WORD;
	m_wFlashCount=INVALID_WORD;

	//设置时间
	SetTimer(IDI_UPDATE_STATUS,200,NULL);

	//设置按钮
	m_btLast.SetButtonImage(IDB_BT_DOWN_LOAD_LAST,AfxGetInstanceHandle(),true,false);
	m_btNext.SetButtonImage(IDB_BT_DOWN_LOAD_NEXT,AfxGetInstanceHandle(),true,false);

	//设置图标
	SetWindowText(TEXT("游戏下载："));
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_DOWNLOAD)),FALSE);

	return TRUE;
}

//取消消息
VOID CDlgDownLoad::OnCancel()
{
	//关闭提示
	CInformation Information(this);
	INT nResult=Information.ShowMessageBox(TEXT("您确定要取消全部的程序下载更新吗？"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);

	//关闭窗口
	if (nResult==IDYES)
	{
		//关闭窗口
		DestroyWindow();

		//关闭判断
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		if (pGlobalUserInfo->GetGlobalUserData()->dwUserID==0) AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	}

	return;
}

//转上一页
VOID CDlgDownLoad::OnBnClickedLast()
{
	//效验状态
	ASSERT(m_wCurrentPage>0);
	if (m_wCurrentPage==0) return;

	//设置变量
	m_wCurrentPage--;

	//闪烁变量
	m_wFlashCount=0;
	m_wFlashItem=INVALID_WORD;

	//状态子项
	m_wFocusItem=INVALID_WORD;
	m_wHoverItem=INVALID_WORD;

	//控制按钮
	m_btCancelOne.EnableWindow(FALSE);
	m_btRetryAgain.EnableWindow(FALSE);

	//页面控制
	m_btLast.EnableWindow((m_wCurrentPage>0)?TRUE:FALSE);
	m_btNext.EnableWindow(((m_wCurrentPage+1)<m_wViewPageCount)?TRUE:FALSE);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//转下一页
VOID CDlgDownLoad::OnBnClickedNext()
{
	//效验状态
	ASSERT((m_wCurrentPage+1)<m_wViewPageCount);
	if ((m_wCurrentPage+1)>=m_wViewPageCount) return;

	//设置变量
	m_wCurrentPage++;

	//闪烁变量
	m_wFlashCount=0;
	m_wFlashItem=INVALID_WORD;

	//状态子项
	m_wFocusItem=INVALID_WORD;
	m_wHoverItem=INVALID_WORD;

	//控制按钮
	m_btCancelOne.EnableWindow(FALSE);
	m_btRetryAgain.EnableWindow(FALSE);

	//页面控制
	m_btLast.EnableWindow((m_wCurrentPage>0)?TRUE:FALSE);
	m_btNext.EnableWindow(((m_wCurrentPage+1)<m_wViewPageCount)?TRUE:FALSE);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//取消更新
VOID CDlgDownLoad::OnBnClickedCancelOne()
{
	//效验状态
	ASSERT(m_wFocusItem!=INVALID_WORD);
	ASSERT(m_DownLoadSinkArray.GetCount()>m_wFocusItem);

	//获取子项
	CDownLoadSink * pDownLoadSink=m_DownLoadSinkArray[m_wFocusItem];

	//关闭下载
	pDownLoadSink->CloseDownLoad();

	//删除下载
	SafeDelete(pDownLoadSink);
	m_DownLoadSinkArray.RemoveAt(m_wFocusItem);

	//关闭判断
	if (m_DownLoadSinkArray.GetCount()>0L)
	{
		//调整窗口
		RectifyControl();

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}
	else
	{
		//关闭窗口
		DestroyWindow();

		//关闭判断
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		if (pGlobalUserInfo->GetGlobalUserData()->dwUserID==0) AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	}

	return;
}

//再次重试
VOID CDlgDownLoad::OnBnClickedRetryAgain()
{
	//效验状态
	ASSERT(m_wFocusItem!=INVALID_WORD);
	ASSERT(m_DownLoadSinkArray.GetCount()>m_wFocusItem);

	//获取子项
	CDownLoadSink * pDownLoadSink=m_DownLoadSinkArray[m_wFocusItem];
	pDownLoadSink->PerformDownLoad(pDownLoadSink->m_szClientName,pDownLoadSink->m_wKindID,pDownLoadSink->m_wServerID);

	return;
}

//销毁消息
VOID CDlgDownLoad::OnDestroy()
{
	__super::OnDestroy();

	//状态变量
	m_bCreate=false;
	m_wCurrentPage=0;
	m_wViewPageCount=0;
	m_wViewLineCount=0;

	//子项状态
	m_wFocusItem=INVALID_WORD;
	m_wHoverItem=INVALID_WORD;

	//闪烁变量
	m_wFlashItem=INVALID_WORD;
	m_wFlashCount=INVALID_WORD;

	//关闭下载
	for (INT_PTR i=0;i<m_DownLoadSinkArray.GetCount();i++)
	{
		//获取子项
		ASSERT(m_DownLoadSinkArray[i]!=NULL);
		CDownLoadSink * pDownLoadSink=m_DownLoadSinkArray[i];

		//删除子项
		SafeDelete(pDownLoadSink);
	}

	//删除数组
	m_DownLoadSinkArray.RemoveAll();

	return;
}

//时间消息
VOID CDlgDownLoad::OnTimer(UINT nIDEvent)
{
	//更新状态
	switch (nIDEvent)
	{
	case IDI_FLASH_ITEM:		//闪烁子项
		{
			//设置变量
			m_wFlashCount++;

			//终止判断
			if (m_wFlashCount>=FLASH_ITEM_TIMES)
			{
				//删除时间
				KillTimer(IDI_FLASH_ITEM);

				//设置变量
				m_wFlashItem=INVALID_WORD;
				m_wFlashCount=INVALID_WORD;
			}

			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

			return;
		}
	case IDI_UPDATE_STATUS:		//更新状态
		{
			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//鼠标消息
VOID CDlgDownLoad::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//点击判断
	if (m_wFocusItem!=m_wHoverItem)
	{
		//设置变量
		m_wFocusItem=m_wHoverItem;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

		//控制按钮
		m_btCancelOne.EnableWindow((VerdictCancelOne(m_wFocusItem)==true)?TRUE:FALSE);
		m_btRetryAgain.EnableWindow((VerdictRetryAgain(m_wFocusItem)==true)?TRUE:FALSE);
	}

	return;
}

//鼠标消息
VOID CDlgDownLoad::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//下载判断
	if ((m_wHoverItem!=INVALID_WORD)&&(VerdictRetryAgain(m_wHoverItem)==true))
	{
		OnBnClickedRetryAgain();
	}

	return;
}

//位置改变
VOID CDlgDownLoad::OnWindowPosChanged(WINDOWPOS * lpwndpos)
{
	__super::OnWindowPosChanged(lpwndpos);

	//移动按钮
	if (m_bCreate==true)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//前后页面
		CRect rcLast;
		m_btLast.GetWindowRect(&rcLast);

		//框架位置
		INT nBBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
		INT nRBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

		//移动按钮
		INT nYControlPos=rcClient.Height()-rcLast.Height()-nBBorder-6;
		m_btLast.SetWindowPos(NULL,10,nYControlPos,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_btNext.SetWindowPos(NULL,15+rcLast.Width(),nYControlPos,0,0,SWP_NOSIZE|SWP_NOZORDER);

		//按钮区域
		CRect rcButton;
		m_btCancelAll.GetWindowRect(&rcButton);

		//移动按钮
		INT nYButtonPos=rcClient.Height()-rcButton.Height()-nBBorder-4;
		m_btCancelAll.SetWindowPos(NULL,rcClient.Width()-rcButton.Width()-8-nRBorder,nYButtonPos,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_btCancelOne.SetWindowPos(NULL,rcClient.Width()-rcButton.Width()*2-18-nRBorder,nYButtonPos,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_btRetryAgain.SetWindowPos(NULL,rcClient.Width()-rcButton.Width()*3-28-nRBorder,nYButtonPos,0,0,SWP_NOSIZE|SWP_NOZORDER);
	}

	return;
}

//光标消息
BOOL CDlgDownLoad::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//设置选择
	WORD wHoverItem=m_wHoverItem;
	m_wHoverItem=SwitchItemIndex(MousePoint);

	//更新界面
	if (m_wHoverItem!=wHoverItem)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	//设置光标
	if (m_wHoverItem!=INVALID_WORD)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//绘画消息
VOID CDlgDownLoad::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0));

	//加载资源
	CPngImage ImageDownLoadScale;
	ImageDownLoadScale.LoadImage(AfxGetInstanceHandle(),TEXT("DOWN_LOAD_SCALE"));

	//加载资源
	CImage ImageDownLoadItem;
	ImageDownLoadItem.LoadFromResource(AfxGetInstanceHandle(),IDB_DOWN_LOAD_ITEM);

	//渲染资源
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if (pSkinRenderManager!=NULL) pSkinRenderManager->RenderImage(ImageDownLoadItem);

	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;

	//绘画标题
	ImageDownLoadItem.BitBlt(pDC->m_hDC,CX_BORAD+nLBorder,CY_BORAD+nTBorder,m_SizeItemImage.cx,m_SizeItemImage.cy,0,0);

	//绘画子项
	for (INT i=0;i<m_wViewLineCount;i++)
	{
		//计算位置
		INT nYImagePos=0;
		INT nXScreenPos=CX_BORAD+nLBorder;
		INT nYScreenPos=CY_BORAD+nTBorder+(i+1)*m_SizeItemImage.cy;

		//索引变量
		WORD wDrawIndex=m_wCurrentPage*m_wViewLineCount+i;

		//闪烁判断
		if (m_wFlashItem!=wDrawIndex)
		{
			if (m_wFocusItem==wDrawIndex) nYImagePos=4*m_SizeItemImage.cy;
			else if (m_wHoverItem==wDrawIndex) nYImagePos=3*m_SizeItemImage.cy;
			else nYImagePos=((i%2)!=0)?m_SizeItemImage.cy:m_SizeItemImage.cy*2;
		}
		else
		{
			if ((i%2)==0) nYImagePos=4*m_SizeItemImage.cy;
			if ((i%2)==1) nYImagePos=((i%2)!=0)?m_SizeItemImage.cy:m_SizeItemImage.cy*2;
		}

		//绘画背景
		ImageDownLoadItem.BitBlt(pDC->m_hDC,nXScreenPos,nYScreenPos,m_SizeItemImage.cx,m_SizeItemImage.cy,0,nYImagePos);

		//绘画子项
		if (wDrawIndex<m_DownLoadSinkArray.GetCount())
		{
			//获取子项
			ASSERT(m_DownLoadSinkArray[wDrawIndex]!=NULL);
			CDownLoadSink * pDownLoadSink=m_DownLoadSinkArray[wDrawIndex];

			//变量定义
			UINT uDrawFormat=DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS;

			//任务名字
			CRect rcClientName;
			rcClientName.SetRect(nXScreenPos,nYScreenPos,nXScreenPos+84,nYScreenPos+30);
			pDC->DrawText(pDownLoadSink->m_szClientName,lstrlen(pDownLoadSink->m_szClientName),&rcClientName,uDrawFormat);

			//文件大小
			if ((pDownLoadSink->m_cbDownLoadStatus==DOWN_LOAD_READ)||(pDownLoadSink->m_cbDownLoadStatus==DOWN_LOAD_SETUP))
			{
				//构造字符
				TCHAR szTotalFileSize[64]=TEXT("");
				DWORD dwTotalFileSize=pDownLoadSink->m_dwTotalFileSize;
				_sntprintf(szTotalFileSize,CountArray(szTotalFileSize),TEXT("%ld KB"),dwTotalFileSize/1000L);

				//文件大小
				CRect rcTotalSize;
				rcTotalSize.SetRect(nXScreenPos+84,nYScreenPos,nXScreenPos+164,nYScreenPos+30);
				pDC->DrawText(szTotalFileSize,lstrlen(szTotalFileSize),&rcTotalSize,uDrawFormat);
			}

			//下载速度
			if ((pDownLoadSink->m_cbDownLoadStatus==DOWN_LOAD_READ)||(pDownLoadSink->m_cbDownLoadStatus==DOWN_LOAD_SETUP))
			{
				//构造字符
				TCHAR szDownSpeed[64]=TEXT("");
				_sntprintf(szDownSpeed,CountArray(szDownSpeed),TEXT("%ld KB/S"),pDownLoadSink->m_dwDownSpeed);

				//文件大小
				CRect rcDownSpeed;
				rcDownSpeed.SetRect(nXScreenPos+398,nYScreenPos,nXScreenPos+483,nYScreenPos+30);
				pDC->DrawText(szDownSpeed,lstrlen(szDownSpeed),&rcDownSpeed,uDrawFormat);
			}

			//下载进度
			if (pDownLoadSink->m_cbDownLoadStatus!=DOWN_LOAD_IDLE)
			{
				//计算比例
				DWORD dwDownScale=0L;
				if (pDownLoadSink->m_dwTotalFileSize>0L)
				{
					dwDownScale=pDownLoadSink->m_dwDownLoadSize*100L/pDownLoadSink->m_dwTotalFileSize;
				}

				//构造字符
				TCHAR szDownScale[64]=TEXT("");
				LPCTSTR pszStatusString=GetStatusString(pDownLoadSink->m_cbDownLoadStatus);
				_sntprintf(szDownScale,CountArray(szDownScale),TEXT("%ld%% %s"),dwDownScale,pszStatusString);

				//下载进度
				pDC->TextOut(nXScreenPos+315,nYScreenPos+8,szDownScale,lstrlen(szDownScale));

				//绘画进度
				INT nXDrawArea=m_SizeScaleImage.cx*dwDownScale/100L;
				ImageDownLoadScale.DrawImage(pDC,nXScreenPos+168,nYScreenPos+8,m_SizeScaleImage.cx,m_SizeScaleImage.cy,0,0);
				ImageDownLoadScale.DrawImage(pDC,nXScreenPos+168,nYScreenPos+8,nXDrawArea,m_SizeScaleImage.cy,0,m_SizeScaleImage.cy);
			}
		}
	}

	//绘画下线
	ImageDownLoadItem.BitBlt(pDC->m_hDC,CX_BORAD+nLBorder,CY_BORAD+nTBorder+(m_wViewLineCount+1)*m_SizeItemImage.cy,m_SizeItemImage.cx,1,0,0);

	return;
}

//下载游戏
bool CDlgDownLoad::DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID)
{
	//查找游戏
	for (INT_PTR i=0;i<m_DownLoadSinkArray.GetCount();i++)
	{
		//获取子项
		ASSERT(m_DownLoadSinkArray[i]!=NULL);
		CDownLoadSink * pDownLoadSink=m_DownLoadSinkArray[i];

		//子项判断
		if (pDownLoadSink->m_wKindID==wKindID)
		{
			//显示窗口
			ShowWindow(SW_SHOW);
			SetForegroundWindow();

			//闪动子项
			FlashDownLoadItem((WORD)(i));

			return true;
		}
	}

	//创建子项
	CDownLoadSink * pDownLoadSink=NULL;
	try
	{
		//创建子项
		pDownLoadSink=new CDownLoadSink;

		//错误判断
		if (pDownLoadSink==NULL)
		{
			ASSERT(FALSE);
			return false;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		return false;
	}

	//插入数组
	m_DownLoadSinkArray.Add(pDownLoadSink);

	//设置子项
	pDownLoadSink->m_pDlgDownLoad=this;
	pDownLoadSink->PerformDownLoad(pszClientName,wKindID,wServerID);

	//调整窗口
	RectifyControl();

	//显示窗口
	ShowWindow(SW_SHOW);
	SetForegroundWindow();

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	return true;
}

//调整控件
VOID CDlgDownLoad::RectifyControl()
{
	//页面设置
	INT nItemCount=(INT)m_DownLoadSinkArray.GetCount();
	m_wViewLineCount=__min(__max(MIN_LINE,nItemCount),MAX_LINE);
	m_wViewPageCount=(nItemCount+m_wViewLineCount-1)/m_wViewLineCount;

	//页面调整
	if (m_wCurrentPage>=m_wViewPageCount) m_wCurrentPage=(m_wViewPageCount-1);

	//计算范围
	WORD wMinIndex=m_wCurrentPage*m_wViewLineCount;
	WORD wMaxtIndex=__min((m_wCurrentPage+1)*m_wViewLineCount-1,(nItemCount-1));

	//子项状态
	m_wHoverItem=((m_wHoverItem<wMinIndex)||(m_wHoverItem>wMaxtIndex))?INVALID_WORD:m_wHoverItem;
	m_wFocusItem=(m_wFocusItem!=INVALID_WORD)?__max(__min(m_wFocusItem,wMaxtIndex),wMinIndex):INVALID_WORD;

	//页面控制
	m_btLast.EnableWindow((m_wCurrentPage>0)?TRUE:FALSE);
	m_btNext.EnableWindow(((m_wCurrentPage+1)<m_wViewPageCount)?TRUE:FALSE);

	//控制按钮
	m_btCancelOne.EnableWindow((VerdictCancelOne(m_wFocusItem)==true)?TRUE:FALSE);
	m_btRetryAgain.EnableWindow((VerdictRetryAgain(m_wFocusItem)==true)?TRUE:FALSE);

	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

	//客户区域
	CRect rcWindow;
	rcWindow.SetRect(0,0,m_SizeItemImage.cx+CX_BORAD*2+nLBorder+nRBorder,m_SizeItemImage.cy*(m_wViewLineCount+1)+nTBorder+nBBorder+44);

	//移动窗口
	CalcWindowRect(&rcWindow,CWnd::adjustBorder);
	SetWindowPos(NULL,0,0,rcWindow.Width(),rcWindow.Height(),SWP_NOZORDER|SWP_NOMOVE|SWP_NOCOPYBITS);

	return;
}

//闪烁子项
VOID CDlgDownLoad::FlashDownLoadItem(WORD wFlashItem)
{
	//效验状态
	ASSERT((wFlashItem==INVALID_WORD)||(wFlashItem<m_DownLoadSinkArray.GetCount()));
	if ((wFlashItem!=INVALID_WORD)&&(wFlashItem>=m_DownLoadSinkArray.GetCount())) return;

	//设置变量
	m_wFlashCount=0;
	m_wFlashItem=wFlashItem;

	//设置时间
	SetTimer(IDI_FLASH_ITEM,150,NULL);

	return;
}

//转换子项
WORD CDlgDownLoad::SwitchItemIndex(CPoint MousePoint)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//变量定义
	INT_PTR nItemCount=m_DownLoadSinkArray.GetCount();
	INT_PTR nHideCount=m_wCurrentPage*m_wViewLineCount;
	INT_PTR nCurrentCount=__min(nItemCount-nHideCount,MAX_LINE);

	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

	//横向判断
	if (MousePoint.x<=(CX_BORAD+nLBorder)) return INVALID_WORD;
	if (MousePoint.x>=(rcClient.Width()-CX_BORAD-nRBorder)) return INVALID_WORD;

	//竖向判断
	if (MousePoint.y<=(CY_BORAD+nTBorder+m_SizeItemImage.cy)) return INVALID_WORD;
	if (MousePoint.y>=(CY_BORAD+nTBorder+m_SizeItemImage.cy*(nCurrentCount+1))) return INVALID_WORD;

	//子项计算
	return (WORD)((MousePoint.y-CY_BORAD-nTBorder-m_SizeItemImage.cy)/m_SizeItemImage.cy+nHideCount);
}

//取消判断
bool CDlgDownLoad::VerdictCancelOne(WORD wFocuxIndex)
{
	//获取子项
	if (wFocuxIndex!=INVALID_WORD)
	{
		//获取子项
		ASSERT(m_DownLoadSinkArray[m_wFocusItem]!=NULL);
		CDownLoadSink * pDownLoadSink=m_DownLoadSinkArray[m_wFocusItem];

		//取消判断
		BYTE cbDownLoadStatus=pDownLoadSink->m_cbDownLoadStatus;
		if ((cbDownLoadStatus!=DOWN_LOAD_SETUP)&&(cbDownLoadStatus!=DOWN_LOAD_SETUP)) return true;
	}

	return false;
}

//重试判断
bool CDlgDownLoad::VerdictRetryAgain(WORD wFocuxIndex)
{
	//获取子项
	if (wFocuxIndex!=INVALID_WORD)
	{
		//获取子项
		ASSERT(m_DownLoadSinkArray[m_wFocusItem]!=NULL);
		CDownLoadSink * pDownLoadSink=m_DownLoadSinkArray[m_wFocusItem];

		//取消判断
		if (pDownLoadSink->m_cbDownLoadStatus==DOWN_LOAD_ERROR) return true;
	}

	return false;
}

//获取状态
LPCTSTR CDlgDownLoad::GetStatusString(BYTE cbDownLoadStatus)
{
	switch (cbDownLoadStatus)
	{
	case DOWN_LOAD_IDLE:
	case DOWN_LOAD_LINK: { return TEXT("正在连接"); }
	case DOWN_LOAD_READ: { return TEXT("正在下载"); }
	case DOWN_LOAD_SETUP: { return TEXT("正在安装"); }
	case DOWN_LOAD_ERROR: { return TEXT("下载失败"); }
	case DOWN_LOAD_FINISH: { return TEXT("安装成功"); }
	}
	
	return NULL;
}

//状态消息
LRESULT CDlgDownLoad::OnItemStatusUpdateMessage(WPARAM wParam, LPARAM lParam)
{
	//获取子项
	ASSERT((CDownLoadSink *)wParam!=NULL);
	CDownLoadSink * pDownLoadSink=(CDownLoadSink *)wParam;

	//完成判断
	if (pDownLoadSink->m_cbDownLoadStatus==DOWN_LOAD_FINISH)
	{
		//变量定义
		WORD wKindID=pDownLoadSink->m_wKindID;
		WORD wServerID=pDownLoadSink->m_wServerID;

		//删除下载
		for (INT_PTR i=0;i<m_DownLoadSinkArray.GetCount();i++)
		{
			if (m_DownLoadSinkArray[i]==pDownLoadSink)
			{
				m_DownLoadSinkArray.RemoveAt(i);
				break;
			}
		}

		//删除子项
		SafeDelete(pDownLoadSink);

		//关闭判断
		if (m_DownLoadSinkArray.GetCount()>0L)
		{
			//调整窗口
			RectifyControl();
		
			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
		}
		else
		{
			//关闭窗口
			DestroyWindow();
		}

		//大厅通知
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_DOWN_LOAD_FINISH,MAKELONG(wKindID,wServerID));
	}
	else
	{
		//控制按钮
		if ((m_wFocusItem!=INVALID_WORD)&&(m_DownLoadSinkArray[m_wFocusItem]==pDownLoadSink))
		{
			m_btCancelOne.EnableWindow((VerdictCancelOne(m_wFocusItem)==true)?TRUE:FALSE);
			m_btRetryAgain.EnableWindow((VerdictRetryAgain(m_wFocusItem)==true)?TRUE:FALSE);
		}
	}

	//更新界面
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
