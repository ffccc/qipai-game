#include "StdAfx.h"
#include "Information.h"
#include "FlashControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

//�궨��
#define WND_FLASH_CLASS				TEXT("WHFalshClass")							//��������

////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFlashControl, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
OLECONTAINER_DEF::COleContainerWnd()
{
	//���ýӿ�
	m_pIControl=NULL;
	m_pIOleObject=NULL;
	m_pIViewObject=NULL;
	m_pIViewObjectEx=NULL;
	m_pIConnectionPoint=NULL;
	m_pIOleInPlaceObject=NULL;
	m_pIConnectionPointContainer=NULL;
	m_pIOleInPlaceObjectWindowless=NULL;

	m_iBPP=0;
	m_lpBitsOnly=NULL;
	m_hdcBack=NULL;
	m_bmpBack=NULL;
	m_hWnd=NULL;
	
	m_iRef=0;
	m_dwConPointID=0;
	m_bTransparent=FALSE;
	m_bFixTransparency=FALSE;

	m_lTotalFrames=0;
	m_lCurrentFrame=-1;
	m_bStop=false;

	return;
} 

//��������
OLECONTAINER_DEF::~COleContainerWnd()
{
	if (m_pIControl)
	{
		if (m_pIControl->IsPlaying())
		{
			m_pIControl->Stop();
		}
		if (m_pIConnectionPoint)
		{
			if (m_dwConPointID)	m_pIConnectionPoint->Unadvise(m_dwConPointID);
			SafeRelease(m_pIConnectionPoint);
		}
		SafeRelease(m_pIConnectionPointContainer);
		m_pIOleObject->Close(OLECLOSE_NOSAVE);

		SafeRelease(m_pIViewObjectEx);
		SafeRelease(m_pIViewObject);
		SafeRelease(m_pIOleInPlaceObjectWindowless);
		SafeRelease(m_pIOleInPlaceObject);
		SafeRelease(m_pIOleObject);
		SafeRelease(m_pIControl);
	}
	if (m_hdcBack)		::DeleteDC(m_hdcBack);
	if (m_bmpBack)		::DeleteObject(m_bmpBack);
	if (m_hWnd)			DestroyWindow(m_hWnd);
}

//��������
OLECONTAINER_FUNCTION(BOOL)::Create(GUID clsid, DWORD dwExStyle, DWORD dwStyle, HWND hWndParent, HINSTANCE hInstance)
{

	m_bTransparent=dwExStyle&WS_EX_LAYERED;

	//��������
	WNDCLASSEX WndClassEx;
	ZeroMemory(&WndClassEx,sizeof(WndClassEx));

	//���ñ���
	WndClassEx.cbSize=sizeof(WNDCLASSEX);
	WndClassEx.hInstance=hInstance;
	WndClassEx.lpszClassName=WND_FLASH_CLASS;
	WndClassEx.lpfnWndProc=COleContainerWnd<TControl>::WndProcStatic;

	//ע�ᴰ��
	RegisterClassEx(&WndClassEx);

	CRect rcRect;
	if (dwStyle&WS_CHILD)
		::GetClientRect(hWndParent, &rcRect);
	else
		::GetWindowRect(hWndParent, &rcRect);

	m_hWnd=CreateWindowEx(dwExStyle,WND_FLASH_CLASS,NULL,dwStyle,rcRect.left,rcRect.top,rcRect.Width(),rcRect.Height(),hWndParent,NULL,hInstance,(VOID *)this);
	::SetWindowPos(m_hWnd,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	HRESULT hResult;
	hResult=OleCreate(clsid, IID_IOleObject, OLERENDER_DRAW,0,(IOleClientSite *)this,(IStorage *)this,(VOID **)&m_pIOleObject);
	if (FAILED(hResult))
	{
		static bool bShow=true;
		if (hResult==REGDB_E_CLASSNOTREG && bShow)
		{
			bShow=false;
			//��ʾ��Ϣ
			CInformation Information;
			if (IDOK==Information.ShowMessageBox(TEXT("ϵͳ��ʾ"),TEXT("����FLASH�������汾���ͣ�Ϊ������ʹ�ù��ܣ������������µ�Flash��������ȷ�����µ�Flash��� "),MB_OKCANCEL,30))
			{
				//��ҳ��
				ShellExecute(NULL,TEXT("OPEN"),TEXT("http://www.adobe.com/go/getflashplayer"),NULL,NULL,SW_NORMAL);
			}
			
			OnErrorClassNotReg();
		}
		return FALSE;
	}

	if (FAILED(OleSetContainedObject(m_pIOleObject, TRUE)))		return FALSE;
	if (FAILED(m_pIOleObject->QueryInterface(__uuidof(TControl), (VOID **)&m_pIControl)))	return FALSE;
	if (FAILED(m_pIOleObject->QueryInterface(IID_IViewObjectEx, (VOID **)&m_pIViewObjectEx)))
	{
		m_pIViewObjectEx=NULL;
		if (FAILED(m_pIOleObject->QueryInterface(IID_IViewObject, (VOID **)&m_pIViewObject)))	return FALSE;
	}
	if (m_bTransparent)
	{
		if (FAILED(m_pIOleObject->QueryInterface(IID_IOleInPlaceObjectWindowless, (VOID **)&m_pIOleInPlaceObjectWindowless)))
		{
			if (FAILED(m_pIOleObject->QueryInterface(IID_IOleInPlaceObject, (VOID **)&m_pIOleInPlaceObject)))		return FALSE;
			m_bTransparent=FALSE;
		}
	}
	else
	{
		if (FAILED(m_pIOleObject->QueryInterface(IID_IOleInPlaceObject, (VOID **)&m_pIOleInPlaceObject)))	return FALSE;
	}

	if (!OnBeforeShowingContent())		return FALSE;
	if (FAILED(m_pIOleObject->DoVerb(OLEIVERB_SHOW, NULL, (IOleClientSite *)this, 0, NULL, NULL)))		return FALSE;
	if (!OnAfterShowingContent())		return FALSE;

	INT nScreenW=GetSystemMetrics(SM_CXSCREEN); 
	INT nScreenH=GetSystemMetrics(SM_CYSCREEN);
	::SetWindowPos(m_hWnd,HWND_TOPMOST,(nScreenW-rcRect.Width())/2, (nScreenH-rcRect.Height())/2, 0, 0, SWP_NOSIZE);

	return TRUE;
}

//IUnknown
OLECONTAINER_FUNCTION(ULONG STDMETHODCALLTYPE)::AddRef()
{
	m_iRef++;
	return m_iRef;
}

OLECONTAINER_FUNCTION(ULONG STDMETHODCALLTYPE)::Release()
{
	m_iRef--;
	return m_iRef;
}

OLECONTAINER_FUNCTION(HRESULT STDMETHODCALLTYPE)::QueryInterface(REFIID riid, VOID ** ppvObject)
{
	if (IsEqualGUID(riid, IID_IUnknown))
		*ppvObject=(VOID*)(this);
	else if (IsEqualGUID(riid, IID_IOleInPlaceSite))
		*ppvObject=(VOID*)dynamic_cast<IOleInPlaceSite *>(this);
	else if (IsEqualGUID(riid, IID_IOleInPlaceSiteEx))
		*ppvObject=(VOID*)dynamic_cast<IOleInPlaceSiteEx *>(this);
	else if (IsEqualGUID(riid, IID_IOleInPlaceSiteWindowless))
		*ppvObject=(VOID*)dynamic_cast<IOleInPlaceSiteWindowless *>(this);
	else if (IsEqualGUID(riid, IID_IOleInPlaceFrame))
		*ppvObject=(VOID*)dynamic_cast<IOleInPlaceFrame *>(this);
	else if (IsEqualGUID(riid, IID_IStorage))
		*ppvObject=(VOID*)dynamic_cast<IStorage *>(this);
	else
	{
		*ppvObject=0;
		return E_NOINTERFACE;
	}
	if (!(*ppvObject))	return E_NOINTERFACE; //if dynamic_cast returned 0
	m_iRef++;
	return S_OK;
}

OLECONTAINER_FUNCTION(HRESULT STDMETHODCALLTYPE)::GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame, LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	*lplpFrame=(LPOLEINPLACEFRAME)this;
	*lplpDoc=0;

	lpFrameInfo->fMDIApp=FALSE;
	lpFrameInfo->hwndFrame=m_hWnd;
	lpFrameInfo->haccel=0;
	lpFrameInfo->cAccelEntries=0;

	CRect rcRect;
	::GetClientRect(m_hWnd, &rcRect);
	*lprcPosRect=rcRect;
	*lprcClipRect=rcRect;

	return(S_OK);
}

//�ص�����
OLECONTAINER_FUNCTION(LRESULT CALLBACK)::WndProcStatic(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//������Ϣ
	if (uMessage==WM_CREATE)
	{
		LPCREATESTRUCT lpCreateStruct=(LPCREATESTRUCT)lParam;
		SetWindowLong(hWnd,GWL_USERDATA,(LONG)(LONGLONG)(lpCreateStruct->lpCreateParams));
		return 0L;
	}

	//��ȡ����
	COleContainerWnd<TControl> * pWnd=(COleContainerWnd<TControl> *)((LONGLONG)(LONG)(GetWindowLong(hWnd,GWL_USERDATA)));

	//��������
	if (pWnd!=NULL)
	{
		return pWnd->WndProc(hWnd,uMessage,wParam,lParam);
	}
	else
	{
		return DefWindowProc(hWnd,uMessage,wParam,lParam);
	}

	return 0L;
}

//�ص�����
OLECONTAINER_FUNCTION(LRESULT)::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT lpcs=(LPCREATESTRUCT)lParam;
			SetWindowLong(hWnd, GWL_USERDATA,(LONG)(LONGLONG)(lpcs->lpCreateParams));
			return 0;
		}
		break;
	case WM_PAINT:
		{
			if (!m_bTransparent)
			{
				PAINTSTRUCT ps;
				HDC hdc=::BeginPaint(m_hWnd, &ps);
				Draw(hdc, &ps.rcPaint, ps.fErase);
				::EndPaint(m_hWnd, &ps);
				return 0;
			}
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		//SendMessage(m_hWnd, WM_NCLBUTTONUP, HTCAPTION, lParam);
		return 1;
		break;
	case WM_NCHITTEST:
		{
			INT x=LOWORD(lParam), y=HIWORD(lParam);
			if (m_pIOleObject && m_pIViewObjectEx)
			{
				IViewObjectEx *pIViewObject=m_pIViewObjectEx;
				POINT Point={x, y};
				DWORD dwRes;
				RECT rcTotal;
				GetWindowRect(m_hWnd, &rcTotal);
				HRESULT hResult=pIViewObject->QueryHitPoint(DVASPECT_CONTENT, &rcTotal, Point, 1, &dwRes);
				if (hResult==S_OK)
				{
					if (dwRes==HITRESULT_OUTSIDE)
						return HTTRANSPARENT;
					else
						return HTCLIENT;
				}
			}
		}
		break;
	case WM_SIZE:
		{
			HRESULT hResult;
			RECT rPos;
			GetClientRect(m_hWnd, &rPos);
			RECT rClip=rPos;
			if (m_pIOleInPlaceObjectWindowless)
				hResult=m_pIOleInPlaceObjectWindowless->SetObjectRects(&rPos, &rClip);
			else if (m_pIOleInPlaceObject)
				hResult=m_pIOleInPlaceObject->SetObjectRects(&rPos, &rClip);
			return 0;
		}
		break;
	case WM_RBUTTONDOWN:
		return 1;
	case WM_COMMAND:
		{
			INT wmId, wmEvent;
			wmId   =LOWORD(wParam);
			wmEvent=HIWORD(wParam);

			break;
		}
		break;
	}
	if (m_pIOleInPlaceObjectWindowless!=NULL)
	{
		if (uMessage==WM_MOUSEMOVE||uMessage==WM_LBUTTONDOWN||uMessage==WM_LBUTTONUP||uMessage==WM_LBUTTONDBLCLK
			|| uMessage==WM_RBUTTONDOWN||uMessage==WM_RBUTTONUP||uMessage==WM_RBUTTONDBLCLK
			|| uMessage==WM_MBUTTONDOWN||uMessage==WM_MBUTTONUP||uMessage==WM_MBUTTONDBLCLK
			|| uMessage==WM_MOUSEWHEEL 
			|| uMessage==WM_KEYDOWN||uMessage==WM_KEYUP||uMessage==WM_CHAR
			|| uMessage==WM_SETCURSOR
			)
		{
			HRESULT hResult;
			LRESULT res;
			hResult=m_pIOleInPlaceObjectWindowless->OnWindowMessage(uMessage, wParam, lParam, &res);
			if (hResult==S_OK)
				return res;
		}
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CFlashWnd::CFlashWnd()
{
	//���ñ���
	m_pIFlashControlSink=NULL;
	ZeroMemory(m_szFile,sizeof(m_szFile));

	return;
}

//��������
CFlashWnd::~CFlashWnd()
{
}

//�����ļ�
VOID CFlashWnd::SetFlashFile(LPCTSTR pszFileName)
{
	//Ч�����
	ASSERT(pszFileName!=NULL);
	if (pszFileName==NULL) return;

	//�����ļ�
	lstrcpyn(m_szFile,pszFileName,CountArray(m_szFile));

	return;
}

//����λ��
VOID CFlashWnd::SetFlashPosition(INT nXPos, INT nYPos)
{
	//Ч��״̬
	ASSERT(m_hWnd!=NULL);
	if (m_hWnd==NULL) return;

	//����λ��
	SetWindowPos(m_hWnd,HWND_TOPMOST,nXPos,nYPos,0,0,SWP_NOSIZE);

	return;
}

//���ýӿ�
VOID CFlashWnd::SetFlashControlSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IFlashControlSink)!=NULL);
	m_pIFlashControlSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IFlashControlSink);

	return;
}

//��ʼ����
VOID CFlashWnd::Play()
{
	OnAfterShowingContent();
	m_bStop=false;
}

//ֹͣ����
VOID CFlashWnd::Stop()
{
	m_pIControl->Stop();
	m_bStop=true;
	InvalidateRect(NULL,TRUE);
}

//���²���
VOID CFlashWnd::Replay()
{
	m_pIControl->Rewind();
	m_pIControl->Play();
	m_bStop=false;
}

//�滭����
VOID CFlashWnd::Draw(HDC hdcDraw, LPCRECT rcDraw, BOOL bErase)
{
	//Ч��״̬
	ASSERT(m_pIControl!=NULL);
	if (m_pIControl==NULL) return;

	//��ȡλ��
	CRect rcRect;
	GetClientRect(m_hWnd,&rcRect);

	//�滭�ж� 
	if (rcRect.Width()==0) return;
	if (rcRect.Height()==0) return;

	//�ӿڱ���
	IOleObject * pIOleObject=m_pIOleObject;
	IViewObject * pIViewObject=(m_pIViewObjectEx!=NULL)?m_pIViewObjectEx:m_pIViewObject;

	if (m_hdcBack)			::DeleteDC(m_hdcBack);
	if (m_bmpBack)			::DeleteObject(m_bmpBack);

	//�滭����
	/*if ((pIOleObject!=NULL)&&(pIViewObject!=NULL))
	{
		//��������
		CImage ImageBuffer;
		ImageBuffer.Create(rcRect.Width(),rcRect.Height(),32);

		//�滭����
		CImageDC ImageDC(ImageBuffer);
		OleDraw(pIViewObject,DVASPECT_TRANSPARENT,ImageDC,&rcRect);
	}*/

	//��������
	BITMAPINFOHEADER BitmapInfoHeader;
	ZeroMemory(&BitmapInfoHeader,sizeof(BitmapInfoHeader));

	//���ñ���
	BitmapInfoHeader.biSize=sizeof(BITMAPINFOHEADER);
	BitmapInfoHeader.biBitCount=32;
	BitmapInfoHeader.biCompression=BI_RGB;
	BitmapInfoHeader.biPlanes=1;
	BitmapInfoHeader.biWidth=rcRect.Width();
	BitmapInfoHeader.biHeight=-rcRect.Height();

	HDC hdc=::GetDC(m_hWnd);
	m_hdcBack=CreateCompatibleDC(hdc);
	m_bmpBack=CreateDIBSection(hdc, (BITMAPINFO *)&BitmapInfoHeader, DIB_RGB_COLORS, (VOID **)&m_lpBitsOnly, NULL, 0x0);
	SelectObject(m_hdcBack, m_bmpBack);
	::ReleaseDC(m_hWnd,hdc);

	if (m_iBPP==0) m_iBPP=GetDeviceCaps(m_hdcBack,BITSPIXEL);

	CPoint Point(rcRect.left, rcRect.top);
	CSize Size(rcRect.Width(),rcRect.Height());

	//�滭����
	if ((pIOleObject!=NULL)&&(pIViewObject!=NULL))
	{
		CRect rcTotal;
		::GetClientRect(m_hWnd,&rcTotal);
		if (m_iBPP==32)
		{
			ZeroMemory(m_lpBitsOnly,Size.cx*Size.cy*sizeof(DWORD));
			OleDraw(pIViewObject,DVASPECT_TRANSPARENT,m_hdcBack,&rcTotal);
		}
		else //in 8/16/24 bit screen depth UpdateLayeredWindow produces wrong results - we use underlaying DC to paint to
		{
			//HWND hwndParent=::GetParent(hwnd);
			HWND hwndParent=::GetDesktopWindow();
			HDC hdcParent=::GetWindowDC(hwndParent);

			CRect rcClient;
			GetClientRect(m_hWnd,rcClient);
			
			CPoint pt;
			pt.x=rcClient.left;
			pt.y=rcClient.top;
			ClientToScreen(m_hWnd,&pt);

			BitBlt(m_hdcBack, 0, 0, rcTotal.right, rcTotal.bottom, hdcParent, pt.x, pt.y, SRCCOPY);
			::ReleaseDC(hwndParent, hdcParent);
			OleDraw(pIViewObject, DVASPECT_TRANSPARENT, m_hdcBack, &rcTotal);
		}
	}

/*	CPoint Point(rcRect.left, rcRect.top);
	CSize Size(rcRect.Width(),rcRect.Height());*/

	//��������
	BLENDFUNCTION BlendFunction;
	ZeroMemory(&BlendFunction,sizeof(BlendFunction));

	//���ñ���
	BlendFunction.BlendOp=AC_SRC_OVER;
	BlendFunction.AlphaFormat=AC_SRC_ALPHA;
	BlendFunction.SourceConstantAlpha=255;

	return;
	//��ǰ֡��
	LONG lCurrentFrame=-1;
	if (m_pIControl!=NULL)
	{
		lCurrentFrame=m_pIControl->CurrentFrame();
	}

	//����ж�
	if ((m_pIControl!=NULL)&&(((m_lCurrentFrame+1)==m_lTotalFrames)||(m_lCurrentFrame>lCurrentFrame)))
	{
		//ֹͣ����
		m_pIControl->Stop();

		//�¼�֪ͨ
		if (m_pIFlashControlSink!=NULL)
		{
			m_pIFlashControlSink->OnEventFlashPlayFinish();
		}
	}

	//���ñ���
	if (m_bStop)
		BlendFunction.SourceConstantAlpha=0;

	m_lCurrentFrame=lCurrentFrame;

	CPoint ImagePoint(0,0);
	UpdateLayeredWindow(m_hWnd, NULL, &Point, &Size, m_hdcBack, &ImagePoint, 0, &BlendFunction, m_iBPP==32 ? ULW_ALPHA : ULW_OPAQUE);

	return;
}

//IUnknown
ULONG STDMETHODCALLTYPE CFlashWnd::AddRef()
{
	m_iRef++;
	return m_iRef;
}
ULONG STDMETHODCALLTYPE CFlashWnd::Release()
{
	m_iRef--;
	return m_iRef;
}
HRESULT STDMETHODCALLTYPE CFlashWnd::QueryInterface(REFIID riid, VOID ** ppvObject)
{
	HRESULT hResult=COleContainerWnd<ShockwaveFlashObjects::IShockwaveFlash>::QueryInterface(riid, ppvObject);
	if (hResult!=E_NOINTERFACE)	return hResult;

	if (IsEqualGUID(riid, ShockwaveFlashObjects::DIID__IShockwaveFlashEvents))
	{
		*ppvObject=(VOID*)dynamic_cast<ShockwaveFlashObjects::_IShockwaveFlashEvents *>(this);
	}
	else
	{
		*ppvObject=0;
		return E_NOINTERFACE;
	}

	if (!(*ppvObject)) return E_NOINTERFACE;

	m_iRef++;

	return S_OK;
}

VOID CFlashWnd::OnErrorClassNotReg()
{
	//some notification code here
}

BOOL CFlashWnd::OnBeforeShowingContent()
{
	LONG lVersion=m_pIControl->FlashVersion();
	if ((lVersion&0x00FF0000)==0x00080000) 
		m_bFixTransparency=TRUE;
	else
		m_bFixTransparency=FALSE;

	if (FAILED(m_pIControl->QueryInterface(IID_IConnectionPointContainer, (VOID**)&m_pIConnectionPointContainer)))						return FALSE;
	if (FAILED(m_pIConnectionPointContainer->FindConnectionPoint(ShockwaveFlashObjects::DIID__IShockwaveFlashEvents, &m_pIConnectionPoint)))	return FALSE;
	if (FAILED(m_pIConnectionPoint->Advise((ShockwaveFlashObjects::_IShockwaveFlashEvents *)this, &m_dwConPointID)))			return FALSE;

	if (m_bTransparent)	m_pIControl->PutWMode(L"transparent");
	m_pIControl->PutScale(L"showAll");
	m_pIControl->PutBackgroundColor(0x00000000);
	m_pIControl->PutEmbedMovie(FALSE);
	m_pIControl->PutLoop(FALSE);

	return TRUE;
}

BOOL CFlashWnd::OnAfterShowingContent()
{
	m_pIControl->PutEmbedMovie(TRUE);

	if (m_szFile[0]!=0) 
	{
		if (FAILED(m_pIControl->LoadMovie(0, m_szFile)))	return FALSE;
		if (FAILED(m_pIControl->Play()))					return FALSE;
		try
		{
			m_lTotalFrames=m_pIControl->GetTotalFrames();
		}
		catch(...)
		{
			m_lTotalFrames=0;
			return FALSE;
		}
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CFlashControl::CFlashControl()
{
}

//��������
CFlashControl::~CFlashControl()
{
}

//�������
VOID CFlashControl::CreateContrl(CWnd * pParentWnd, CRect rcCreate, INT nID)
{
	//Ч��״̬
	ASSERT(m_hWnd==NULL);
	if (m_hWnd!=NULL) return;

	//ע�ᴰ��
	HGDIOBJ hBursh=GetStockObject(NULL_BRUSH);
	HCURSOR hCursor=LoadCursor(NULL,IDC_ARROW);
	LPCTSTR pszWndClass=AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,hCursor,(HBRUSH)hBursh,NULL);

	//��������
	DWORD dwStyle=WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CHILD;
	//CreateEx(NULL,pszWndClass,NULL,dwStyle,rcCreate,pParentWnd,nID);

	//��������
	GUID FalshGuid=ShockwaveFlashObjects::CLSID_ShockwaveFlash;
	HINSTANCE hInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
	m_FlashWnd.Create(FalshGuid,WS_EX_LAYERED,WS_POPUP|WS_VISIBLE|WS_CLIPSIBLINGS,pParentWnd->m_hWnd,hInstance);

	return;
}

//�滭����
BOOL CFlashControl::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////