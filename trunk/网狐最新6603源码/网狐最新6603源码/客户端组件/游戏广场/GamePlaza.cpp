#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePlazaApp::CGamePlazaApp()
{
	//设置变量
	m_pIDispatch=NULL;
	m_pCustomOccManager=NULL;

	return;
}

//初始函数
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();

#ifndef _DEBUG
#ifndef RELEASE_MANAGER

	//存在判断
	bool bPlazaExist=false;
	CMutex Mutex(FALSE,szPlazaClass,NULL);
	if (Mutex.Lock(0)==FALSE) bPlazaExist=true;

	//搜索窗口
	CWnd * pWndGamePlaza=CWnd::FindWindow(szPlazaClass,NULL);
	if (pWndGamePlaza!=NULL) 
	{
		//设置变量
		bPlazaExist=true;

		//还原窗口
		if (pWndGamePlaza->IsIconic()) 
		{
			pWndGamePlaza->ShowWindow(SW_RESTORE);
		}

		//激活窗口
		pWndGamePlaza->SetActiveWindow();
		pWndGamePlaza->BringWindowToTop();
		pWndGamePlaza->SetForegroundWindow();
	}

	//结果处理
	if (bPlazaExist==true) return FALSE;

#endif
#endif

	//环境配置
	AfxOleInit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//随机种子
	srand((DWORD)time(NULL));

	//设置表名
	SetRegistryKey(szProductKey);

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//安装目录
	CWHRegKey RegInstallPath;
	RegInstallPath.OpenRegKey(TEXT(""),true);
	RegInstallPath.WriteString(TEXT("InstallPath"),szDirectory);

	//加载配置
	m_ParameterGlobal.LoadParameter();
	m_UserInformation.LoadInformation();
	m_ExpressionManager.LoadExpression();

	//设置目录
	SetCurrentDirectory(szDirectory);
	m_SkinResourceManager.SetSkinResource(m_ParameterGlobal.m_SkinRenderInfo);

	//创建对象
	m_pIDispatch=new CImpIDispatch;
	m_pCustomOccManager=new CCustomOccManager;
	AfxEnableControlContainer(m_pCustomOccManager); 

	//创建组件
	if (m_FaceItemControlModule.CreateInstance()==false) return false;
	if (m_UserOrderParserModule.CreateInstance()==false) return false;

	//用户元素
	m_UserItemElement.SetUserOrderParser(m_UserOrderParserModule.GetInterface());
	m_UserItemElement.SetFaceItemControl(m_FaceItemControlModule.GetInterface());

	//变量定义
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//注册窗口
	WndClasss.style=CS_DBLCLKS;
	WndClasss.hIcon=LoadIcon(IDR_MAINFRAME);
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=szPlazaClass;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	//建立窗口
	CPlatformFrame * pPlatformFrame=new CPlatformFrame();
	pPlatformFrame->Create(szPlazaClass,szProduct,WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));

	//设置变量
	m_pMainWnd=pPlatformFrame;

	return TRUE;
}

//退出函数
INT CGamePlazaApp::ExitInstance()
{
	//保存配置
	m_ParameterGlobal.SaveParameter();

	//删除对象
	SafeDelete(m_pIDispatch);
	SafeDelete(m_pCustomOccManager);

	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////////////

//应用程序对象
CGamePlazaApp theApp;

//////////////////////////////////////////////////////////////////////////////////
