#include "StdAfx.h"
#include "Information.h"
#include "FlashControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

//�궨��
#define WND_FLASH_CLASS				TEXT("WHFalshClass")							//��������

////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CFlashActiveX::CFlashActiveX()
{
	//�������
	m_hWnd=NULL;
	m_lTotalFrames=0L;

	//���ýӿ�
	m_pIOleObject=NULL;
	m_pIViewObject=NULL;
	m_pIViewObjectEx=NULL;
	m_pIFalshControl=NULL;

	m_nRef=0;

	//���ñ���
	m_pIFlashControlSink=NULL;

	return;
}

//��������
CFlashActiveX::~CFlashActiveX()
{
	//�ͷŽӿ�
	SafeRelease(m_pIOleObject);
	SafeRelease(m_pIViewObject);
	SafeRelease(m_pIViewObjectEx);
	SafeRelease(m_pIFalshControl);

	//�رմ���
	if (m_hWnd!=NULL) DestroyWindow(m_hWnd);

	return;
}

//�滭����
VOID CFlashActiveX::DrawControl(HDC hdcDraw, LPCRECT rcDraw, BOOL bErase)
{
	//Ч��״̬
	if (m_hWnd==NULL) return;
	if (m_lTotalFrames==0L) return;
	if (m_pIFalshControl==NULL) return;

	//��ȡλ��
	CRect rcRect;
	GetClientRect(m_hWnd,&rcRect);

	//�滭�ж� 
	if (rcRect.Width()==0) return;
	if (rcRect.Height()==0) return;

	//�ӿڱ���
	IOleObject * pIOleObject=m_pIOleObject;
	IViewObject * pIViewObject=(m_pIViewObjectEx!=NULL)?m_pIViewObjectEx:m_pIViewObject;

	//�滭����
	if ((pIOleObject!=NULL)&&(pIViewObject!=NULL))
	{
		//��������
		CImage ImageBuffer;
		ImageBuffer.Create(rcRect.Width(),rcRect.Height(),32);

		//�滭����
		CImageDC ImageDC(ImageBuffer);
		OleDraw(pIViewObject,DVASPECT_TRANSPARENT,ImageDC,&rcRect);

		//��������
		BLENDFUNCTION BlendFunction;
		ZeroMemory(&BlendFunction,sizeof(BlendFunction));

		//���ñ���
		BlendFunction.BlendOp=AC_SRC_OVER;
		BlendFunction.AlphaFormat=AC_SRC_ALPHA;
		BlendFunction.SourceConstantAlpha=0xFF;

		//����ж�
		if ((m_pIFalshControl->CurrentFrame()+1L)>=m_lTotalFrames)
		{
			//ֹͣ����
			m_pIFalshControl->StopPlay();

			//���ñ���
			BlendFunction.SourceConstantAlpha=0x00;

			//�¼�֪ͨ
			if (m_pIFlashControlSink!=NULL)
			{
				m_pIFlashControlSink->OnEventFlashPlayFinish();
			}
		}

		//��������
		CPoint ImagePoint(0,0);
		CSize SizeImage(ImageBuffer.GetWidth(),ImageBuffer.GetHeight());

		//���ô���
		UpdateLayeredWindow(m_hWnd,NULL,NULL,&SizeImage,ImageDC,&ImagePoint,0L,&BlendFunction,ULW_ALPHA);
	}

	return;
}

//����λ��
VOID CFlashActiveX::SetPosition(INT nXPos, INT nYPos, INT nWidth, INT nHeight, UINT nFlags)
{
	//Ч��״̬
	ASSERT(m_hWnd!=NULL);
	if (m_hWnd==NULL) return;

	//����λ��
	SetWindowPos(m_hWnd,NULL,nXPos,nYPos,nWidth,nHeight,nFlags|SWP_NOZORDER);

	return;
}

//��������
BOOL CFlashActiveX::CreateControl(DWORD dwStyle, CWnd * pParentWnd, CRect rcCreate, HINSTANCE hInstance, IUnknownEx * pIUnknownEx)
{
	//״̬Ч��
	ASSERT(m_hWnd==NULL);
	if (m_hWnd!=NULL) return FALSE;

	//��������
	WNDCLASSEX WndClassEx;
	ZeroMemory(&WndClassEx,sizeof(WndClassEx));

	//���ñ���
	WndClassEx.cbSize=sizeof(WNDCLASSEX);
	WndClassEx.hInstance=hInstance;
	WndClassEx.lpszClassName=WND_FLASH_CLASS;
	WndClassEx.lpfnWndProc=::DefWindowProc;

	//ע�ᴰ��
	RegisterClassEx(&WndClassEx);

	//��ѯ�ӿ�
	m_pIFlashControlSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IFlashControlSink);

	//��������
	m_hWnd=CreateWindowEx(WS_EX_LAYERED,WND_FLASH_CLASS,NULL,dwStyle,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),
		pParentWnd->m_hWnd,NULL,hInstance,(VOID *)this);

	//�����ؼ�
	GUID ClsID=ShockwaveFlashObjects::CLSID_ShockwaveFlash;
	HRESULT hResult=OleCreate(ClsID,IID_IOleObject,OLERENDER_DRAW,0,(IOleClientSite *)this,(IStorage *)this,(VOID **)&m_pIOleObject);

	//����ж�
	if (FAILED(hResult))
	{
		static bool bShow=true;
		if ((hResult==REGDB_E_CLASSNOTREG)&&(bShow==false))
		{
			bShow=false;
			//��ʾ��Ϣ
			CInformation Information;
			if (IDOK==Information.ShowMessageBox(TEXT("ϵͳ��ʾ"),TEXT("����FLASH�������汾���ͣ�Ϊ������ʹ�ù��ܣ������������µ�Flash��������ȷ�����µ�Flash��� "),MB_OKCANCEL,30))
			{
				//��ҳ��
				ShellExecute(NULL,TEXT("OPEN"),TEXT("http://www.adobe.com/go/getflashplayer"),NULL,NULL,SW_NORMAL);
			}
		}

		return FALSE;
	}

	//��ѯ�ӿ�
	if (FAILED(m_pIOleObject->QueryInterface(__uuidof(IFalshControl),(VOID * *)&m_pIFalshControl)))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//��ѯ�ӿ�
	if (FAILED(m_pIOleObject->QueryInterface(IID_IViewObjectEx,(VOID * *)&m_pIViewObjectEx)))
	{
		if (FAILED(m_pIOleObject->QueryInterface(IID_IViewObject,(VOID **)&m_pIViewObject)))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	//����ģʽ
	OleSetContainedObject(m_pIOleObject,TRUE);

	//�������
	m_pIFalshControl->PutWMode(L"Transparent");
	m_pIFalshControl->PutBackgroundColor(0x00000000);

	//��ʾ�ؼ�
	if (FAILED(m_pIOleObject->DoVerb(OLEIVERB_SHOW,NULL,(IOleClientSite *)this,0L,NULL,NULL)))
	{
		ASSERT(FALSE);
		return false;
	}

	return TRUE;
}

//��ʼ����
VOID CFlashActiveX::PlayMovie()
{
	//Ч�����
	ASSERT(m_pIFalshControl!=NULL);
	if (m_pIFalshControl==NULL) return;
		
	//���Ŷ���
	m_pIFalshControl->Rewind();
	m_pIFalshControl->Play();

	return;
}

//ֹͣ����
VOID CFlashActiveX::StopMovie()
{
	//Ч�����
	ASSERT(m_pIFalshControl!=NULL);
	if (m_pIFalshControl==NULL) return;

	//ֹͣ����
	m_pIFalshControl->StopPlay();

	return;
}

//���²���
VOID CFlashActiveX::ReplayMovie()
{
	//Ч�����
	ASSERT(m_pIFalshControl!=NULL);
	if (m_pIFalshControl==NULL) return;

	//���Ŷ���
	m_pIFalshControl->Rewind();
	m_pIFalshControl->Play();

	return;
}

//�����ļ�
bool CFlashActiveX::LoadMovieFile(LPCTSTR pszFileName)
{
	//Ч�����
	ASSERT((m_pIFalshControl!=NULL)&&(m_pIOleObject!=NULL)&&(pszFileName!=NULL));
	if ((m_pIFalshControl==NULL)||(m_pIOleObject==NULL)||(pszFileName==NULL)) return false;

	//ֹͣ����
	if (m_pIFalshControl->IsPlaying())
	{
		StopMovie();
	}

	//�����ļ�
	if (FAILED(m_pIFalshControl->LoadMovie(0,pszFileName)))
	{
		ASSERT(FALSE);
		return false;
	}

	//��ȡ����
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

//��ѯ�ӿ�
HRESULT STDMETHODCALLTYPE CFlashActiveX::QueryInterface(REFIID IID, VOID ** ppvObject)
{
	//��ѯ�ӿ�
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

	//����ж�
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

//���캯��
CFlashControl::CFlashControl()
{
}

//��������
CFlashControl::~CFlashControl()
{
}

//��ʼ����
VOID CFlashControl::PlayMovie()
{
	//��ʼ����
	m_FlashContainer.PlayMovie();

	return;
}

//ֹͣ����
VOID CFlashControl::StopMovie()
{
	//ֹͣ����
	m_FlashContainer.StopMovie();

	return;
}

//���²���
VOID CFlashControl::ReplayMovie()
{
	//���²���
	m_FlashContainer.ReplayMovie();

	return;
}

//���ض���
bool CFlashControl::LoadMovieFile(LPCTSTR pszFileName)
{
	//���ض���
	bool bSuccess=m_FlashContainer.LoadMovieFile(pszFileName);

	return bSuccess;
}

//����λ��
VOID CFlashControl::SetPosition(INT nXPos, INT nYPos, INT nWidth, INT nHeight, UINT nFlags)
{
	//����λ��
	m_FlashContainer.SetPosition(nXPos,nYPos,nWidth,nHeight, nFlags);

	return;
}

//�������
VOID CFlashControl::CreateControl(CWnd * pParentWnd, CRect rcCreate, IUnknownEx * pIUnknownEx)
{
	//��������
	HINSTANCE hInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
	m_FlashContainer.CreateControl(WS_POPUP|WS_VISIBLE|WS_CLIPSIBLINGS,pParentWnd,rcCreate,hInstance,pIUnknownEx);

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////

