#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameApp.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameApp::CGameFrameApp()
{
	//设置变量
	m_pGameFrameWnd=NULL;
	m_pGameFrameEngine=NULL;

	return;
}

//析构函数
CGameFrameApp::~CGameFrameApp()
{
}

//配置函数
BOOL CGameFrameApp::InitInstance()
{
	__super::InitInstance();

	//内部初始化
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	try
	{
		//变量定义
		ASSERT(CGlobalUnits::GetInstance()!=NULL);
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

		//全局单元
		bool bSuccess=pGlobalUnits->InitGlobalUnits();
		if (bSuccess==false) throw TEXT("游戏环境初始化失败");

		//创建窗口
		m_pGameFrameWnd=GetGameFrameWnd();
		if (m_pGameFrameWnd==NULL) throw TEXT("游戏框架窗口创建失败");

		//创建引擎
		m_pGameFrameEngine=GetGameFrameEngine(VERSION_FRAME_SDK);
		if (m_pGameFrameEngine==NULL) throw TEXT("游戏引擎对象创建失败");

		//创建框架
		m_pMainWnd=m_pGameFrameWnd;
		m_pGameFrameWnd->Create(NULL,NULL,WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));

		//声音引擎
		m_D3DSound.CreateD3DSound(m_pMainWnd->GetSafeHwnd());

		//创建引擎
		if (m_GameFrameService.CreateGameFrameService()==false)
		{
			throw TEXT("内核引擎创建失败");
		}

		//连接广场
		IClientKernel * pIClientKernel=pGlobalUnits->m_ClientKernelModule.GetInterface();
		if ((pIClientKernel!=NULL)&&(pIClientKernel->IsSingleMode()==false)) pIClientKernel->CreateConnect();

		return TRUE;
	}
	catch (LPCTSTR pszMesssage)
	{
		//构造消息
		CString strBuffer;
		strBuffer.Format(TEXT("由于 [ %s ] 游戏程序启动失败。"),pszMesssage);

		//提示消息
		CInformation Information;
		Information.ShowMessageBox(TEXT("游戏框架"),strBuffer,MB_ICONSTOP,30L);

		//删除对象
		SafeRelease(m_pGameFrameWnd);
		SafeRelease(m_pGameFrameEngine);

		return FALSE;
	}

	return TRUE;
}

//退出函数
BOOL CGameFrameApp::ExitInstance()
{
	//删除对象
	SafeRelease(m_pGameFrameEngine);

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//关闭连接
	IClientKernel * pIClientKernel=pGlobalUnits->m_ClientKernelModule.GetInterface();
	if ((pIClientKernel!=NULL)&&(pIClientKernel->IsSingleMode()==false)) pIClientKernel->IntermitConnect();

	//注销组件
	pGlobalUnits->UnInitGlobalUnits();

	return __super::ExitInstance();
}

//创建窗口
CGameFrameWnd * CGameFrameApp::GetGameFrameWnd()
{
	return new CGameFrameWnd;
}

//////////////////////////////////////////////////////////////////////////////////
