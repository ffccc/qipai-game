#include "StdAfx.h"
#include "Information.h"
#include "FlashControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

//宏定义
#define WND_FLASH_CLASS				TEXT("WHFalshClass")							//窗口类名

////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CFlashActiveX::CFlashActiveX()
{
	//组件变量
	m_hWnd=NULL;
	m_lTotalFrames=0L;

	//设置接口
	m_pIOleObject=NULL;
	m_pIViewObject=NULL;
	m_pIViewObjectEx=NULL;
	m_pIFalshControl=NULL;

	m_nRef=0;

	//设置变量
	m_pIFlashControlSink=NULL;

	return;
}

//析构函数
CFlashActiveX::~CFlashActiveX()
{
	//释放接口
	SafeRelease(m_pIOleObject);
	SafeRelease(m_pIViewObject);
	SafeRelease(m_pIViewObjectEx);
	SafeRelease(m_pIFalshControl);

	//关闭窗口
	if (m_hWnd!=NULL) DestroyWindow(m_hWnd);

	return;
}

//绘画函数
VOID CFlashActiveX::DrawControl(HDC hdcDraw, LPCRECT rcDraw, BOOL bErase)
{
	//效验状态
	if (m_hWnd==NULL) return;
	if (m_lTotalFrames==0L) return;
	if (m_pIFalshControl==NULL) return;

	//获取位置
	CRect rcRect;
	GetClientRect(m_hWnd,&rcRect);

	//绘画判断 
	if (rcRect.Width()==0) return;
	if (rcRect.Height()==0) return;

	//接口变量
	IOleObject * pIOleObject=m_pIOleObject;
	IViewObject * pIViewObject=(m_pIViewObjectEx!=NULL)?m_pIViewObjectEx:m_pIViewObject;

	//绘画对象
	if ((pIOleObject!=NULL)&&(pIViewObject!=NULL))
	{
		//创建缓冲
		CImage ImageBuffer;
		ImageBuffer.Create(rcRect.Width(),rcRect.Height(),32);

		//绘画对象
		CImageDC ImageDC(ImageBuffer);
		OleDraw(pIViewObject,DVASPECT_TRANSPARENT,ImageDC,&rcRect);

		//变量定义
		BLENDFUNCTION BlendFunction;
		ZeroMemory(&BlendFunction,sizeof(BlendFunction));

		//设置变量
		BlendFunction.BlendOp=AC_SRC_OVER;
		BlendFunction.AlphaFormat=AC_SRC_ALPHA;
		BlendFunction.SourceConstantAlpha=0xFF;

		//完成判断
		if ((m_pIFalshControl->CurrentFrame()+1L)>=m_lTotalFrames)
		{
			//停止播放
			m_pIFalshControl->StopPlay();

			//设置变量
			BlendFunction.SourceConstantAlpha=0x00;

			//事件通知
			if (m_pIFlashControlSink!=NULL)
			{
				m_pIFlashControlSink->OnEventFlashPlayFinish();
			}
		}

		//变量定义
		CPoint ImagePoint(0,0);
		CSize SizeImage(ImageBuffer.GetWidth(),ImageBuffer.GetHeight());

		//设置窗口
		UpdateLayeredWindow(m_hWnd,NULL,NULL,&SizeImage,ImageDC,&ImagePoint,0L,&BlendFunction,ULW_ALPHA);
	}

	return;
}

//设置位置
VOID CFlashActiveX::SetPosition(INT nXPos, INT nYPos, INT nWidth, INT nHeight, UINT nFlags)
{
	//效验状态
	ASSERT(m_hWnd!=NULL);
	if (m_hWnd==NULL) return;

	//设置位置
	SetWindowPos(m_hWnd,NULL,nXPos,nYPos,nWidth,nHeight,nFlags|SWP_NOZORDER);

	return;
}

//创建函数
BOOL CFlashActiveX::CreateControl(DWORD dwStyle, CWnd * pParentWnd, CRect rcCreate, HINSTANCE hInstance, IUnknownEx * pIUnknownEx)
{
	//状态效验
	ASSERT(m_hWnd==NULL);
	if (m_hWnd!=NULL) return FALSE;

	//变量定义
	WNDCLASSEX WndClassEx;
	ZeroMemory(&WndClassEx,sizeof(WndClassEx));

	//设置变量
	WndClassEx.cbSize=sizeof(WNDCLASSEX);
	WndClassEx.hInstance=hInstance;
	WndClassEx.lpszClassName=WND_FLASH_CLASS;
	WndClassEx.lpfnWndProc=::DefWindowProc;

	//注册窗口
	RegisterClassEx(&WndClassEx);

	//查询接口
	m_pIFlashControlSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IFlashControlSink);

	//创建窗口
	m_hWnd=CreateWindowEx(WS_EX_LAYERED,WND_FLASH_CLASS,NULL,dwStyle,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),
		pParentWnd->m_hWnd,NULL,hInstance,(VOID *)this);

	//创建控件
	GUID ClsID=ShockwaveFlashObjects::CLSID_ShockwaveFlash;
	HRESULT hResult=OleCreate(ClsID,IID_IOleObject,OLERENDER_DRAW,0,(IOleClientSite *)this,(IStorage *)this,(VOID **)&m_pIOleObject);

	//结果判断
	if (FAILED(hResult))
	{
		static bool bShow=true;
		if ((hResult==REGDB_E_CLASSNOTREG)&&(bShow==false))
		{
			bShow=false;
			//提示消息
			CInformation Information;
			if (IDOK==Information.ShowMessageBox(TEXT("系统提示"),TEXT("您的FLASH播放器版本过低，为了正常使用功能，请您下载最新的Flash插件。点击确定最新的Flash插件 "),MB_OKCANCEL,30))
			{
				//打开页面
				ShellExecute(NULL,TEXT("OPEN"),TEXT("http://www.adobe.com/go/getflashplayer"),NULL,NULL,SW_NORMAL);
			}
		}

		return FALSE;
	}

	//查询接口
	if (FAILED(m_pIOleObject->QueryInterface(__uuidof(IFalshControl),(VOID * *)&m_pIFalshControl)))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//查询接口
	if (FAILED(m_pIOleObject->QueryInterface(IID_IViewObjectEx,(VOID * *)&m_pIViewObjectEx)))
	{
		if (FAILED(m_pIOleObject->QueryInterface(IID_IViewObject,(VOID **)&m_pIViewObject)))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	//容器模式
	OleSetContainedObject(m_pIOleObject,TRUE);

	//设置组件
	m_pIFalshControl->PutWMode(L"Transparent");
	m_pIFalshControl->PutBackgroundColor(0x00000000);

	//显示控件
	if (FAILED(m_pIOleObject->DoVerb(OLEIVERB_SHOW,NULL,(IOleClientSite *)this,0L,NULL,NULL)))
	{
		ASSERT(FALSE);
		return false;
	}

	return TRUE;
}

//开始播放
VOID CFlashActiveX::PlayMovie()
{
	//效验参数
	ASSERT(m_pIFalshControl!=NULL);
	if (m_pIFalshControl==NULL) return;
		
	//播放动画
	m_pIFalshControl->Rewind();
	m_pIFalshControl->Play();

	return;
}

//停止播放
VOID CFlashActiveX::StopMovie()
{
	//效验参数
	ASSERT(m_pIFalshControl!=NULL);
	if (m_pIFalshControl==NULL) return;

	//停止动画
	m_pIFalshControl->StopPlay();

	return;
}

//重新播放
VOID CFlashActiveX::ReplayMovie()
{
	//效验参数
	ASSERT(m_pIFalshControl!=NULL);
	if (m_pIFalshControl==NULL) return;

	//播放动画
	m_pIFalshControl->Rewind();
	m_pIFalshControl->Play();

	return;
}

//设置文件
bool CFlashActiveX::LoadMovieFile(LPCTSTR pszFileName)
{
	//效验参数
	ASSERT((m_pIFalshControl!=NULL)&&(m_pIOleObject!=NULL)&&(pszFileName!=NULL));
	if ((m_pIFalshControl==NULL)||(m_pIOleObject==NULL)||(pszFileName==NULL)) return false;

	//停止播放
	if (m_pIFalshControl->IsPlaying())
	{
		StopMovie();
	}

	//加载文件
	if (FAILED(m_pIFalshControl->LoadMovie(0,pszFileName)))
	{
		ASSERT(FALSE);
		return false;
	}

	//获取参数
	m_lTotalFrames=m_pIFalshControl->GetTotalFrames();

	return true;
}

//IUnknown
ULONG STDMETHODCALLTYPE CFlashActiveX::AddRef()
{
	m_nRef++;
	return m_nRef;
}

ULONG STDMETHODCALLTYPE CFlashActiveX::Release()
{
	m_nRef--;
	return m_nRef;
}

//查询接口
HRESULT STDMETHODCALLTYPE CFlashActiveX::QueryInterface(REFIID IID, VOID ** ppvObject)
{
	//查询接口
	if (IsEqualGUID(IID, IID_IUnknown))
	{
		*ppvObject=(VOID *)(this);
	}
	else if (IsEqualGUID(IID, IID_IOleInPlaceSite))
	{
		*ppvObject=(VOID *)dynamic_cast<IOleInPlaceSite *>(this);
	}
	else if (IsEqualGUID(IID, IID_IOleInPlaceSiteEx))
	{
		*ppvObject=(VOID *)dynamic_cast<IOleInPlaceSiteEx *>(this);
	}
	else if (IsEqualGUID(IID, IID_IOleInPlaceSiteWindowless))
	{
		*ppvObject=(VOID *)dynamic_cast<IOleInPlaceSiteWindowless *>(this);
	}
	else if (IsEqualGUID(IID, ShockwaveFlashObjects::DIID__IShockwaveFlashEvents))
	{
		*ppvObject=(VOID *)dynamic_cast<ShockwaveFlashObjects::_IShockwaveFlashEvents *>(this);
	}
	else
	{
		*ppvObject=NULL;
	}

	//结果判断
	if ((*ppvObject)==NULL)
	{
		return E_NOINTERFACE;
	}
	
	m_nRef++;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFlashActiveX::GetWindowContext(LPOLEINPLACEFRAME FAR * lplpFrame, LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	*lplpDoc=NULL;
	*lplpFrame=NULL;

	lpFrameInfo->haccel=0;
	lpFrameInfo->fMDIApp=FALSE;
	lpFrameInfo->hwndFrame=m_hWnd;
	lpFrameInfo->cAccelEntries=0;

	CRect rcRect;
	::GetClientRect(m_hWnd,&rcRect);
	*lprcPosRect=rcRect;
	*lprcClipRect=rcRect;

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CFlashControl::CFlashControl()
{
}

//析构函数
CFlashControl::~CFlashControl()
{
}

//开始动画
VOID CFlashControl::PlayMovie()
{
	//开始动画
	m_FlashContainer.PlayMovie();

	return;
}

//停止动画
VOID CFlashControl::StopMovie()
{
	//停止动画
	m_FlashContainer.StopMovie();

	return;
}

//重新播放
VOID CFlashControl::ReplayMovie()
{
	//重新播放
	m_FlashContainer.ReplayMovie();

	return;
}

//加载动画
bool CFlashControl::LoadMovieFile(LPCTSTR pszFileName)
{
	//加载动画
	bool bSuccess=m_FlashContainer.LoadMovieFile(pszFileName);

	return bSuccess;
}

//设置位置
VOID CFlashControl::SetPosition(INT nXPos, INT nYPos, INT nWidth, INT nHeight, UINT nFlags)
{
	//设置位置
	m_FlashContainer.SetPosition(nXPos,nYPos,nWidth,nHeight, nFlags);

	return;
}

//配置组件
VOID CFlashControl::CreateControl(CWnd * pParentWnd, CRect rcCreate, IUnknownEx * pIUnknownEx)
{
	//创建窗口
	HINSTANCE hInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
	m_FlashContainer.CreateControl(WS_POPUP|WS_VISIBLE|WS_CLIPSIBLINGS,pParentWnd,rcCreate,hInstance,pIUnknownEx);

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////

