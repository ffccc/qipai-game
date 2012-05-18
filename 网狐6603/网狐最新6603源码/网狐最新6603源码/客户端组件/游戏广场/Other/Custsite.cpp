#include "Stdafx.h"

#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

#include "Ocdb.h" 
#include "..\GamePlaza.h"

#include <c:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\atlmfc\src\mfc\occimpl.h>

#undef AFX_DATA
#define AFX_DATA AFX_DATA_EXPORT

#include "Custsite.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CCustomControlSite, COleControlSite)
	INTERFACE_PART(CCustomControlSite, IID_IDocHostUIHandler, DocHostUIHandler)
END_INTERFACE_MAP()

//////////////////////////////////////////////////////////////////////////////////

ULONG FAR EXPORT  CCustomControlSite::XDocHostUIHandler::AddRef()
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	return pThis->ExternalAddRef();
}


ULONG FAR EXPORT  CCustomControlSite::XDocHostUIHandler::Release()
{                            
    METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	return pThis->ExternalRelease();
}

HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::QueryInterface(REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}

// * CImpIDocHostUIHandler::GetHostInfo
// *
// * Purpose: Called at initialization
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::GetHostInfo( DOCHOSTUIINFO* pInfo )
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	pInfo->dwFlags=DOCHOSTUIFLAG_NO3DBORDER;
	//pInfo->dwFlags|=DOCHOSTUIFLAG_SCROLL_NO;
	pInfo->dwDoubleClick=DOCHOSTUIDBLCLK_DEFAULT;

    return S_OK;
}

// * CImpIDocHostUIHandler::ShowUI
// *
// * Purpose: Called when MSHTML.DLL shows its UI
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::ShowUI(
				DWORD dwID, 
				IOleInPlaceActiveObject * /*pActiveObject*/,
				IOleCommandTarget * pCommandTarget,
				IOleInPlaceFrame * /*pFrame*/,
				IOleInPlaceUIWindow * /*pDoc*/)
{

	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	// We've already got our own UI in place so just return S_OK
    return S_OK;
}

// * CImpIDocHostUIHandler::HideUI
// *
// * Purpose: Called when MSHTML.DLL hides its UI
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::HideUI(void)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return S_OK;
}

// * CImpIDocHostUIHandler::UpdateUI
// *
// * Purpose: Called when MSHTML.DLL updates its UI
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::UpdateUI(void)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	// MFC is pretty good about updating it's UI in it's Idle loop so I don't do anything here
	return S_OK;
}

// * CImpIDocHostUIHandler::EnableModeless
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::EnableModeless
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::EnableModeless(BOOL /*fEnable*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnDocWindowActivate
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnDocWindowActivate
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::OnDocWindowActivate(BOOL /*fActivate*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnFrameWindowActivate
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnFrameWindowActivate
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::OnFrameWindowActivate(BOOL /*fActivate*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ResizeBorder
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::ResizeBorder
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::ResizeBorder(
				LPCRECT /*prcBorder*/, 
				IOleInPlaceUIWindow* /*pUIWindow*/,
				BOOL /*fRameWindow*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ShowContextMenu
// *
// * Purpose: Called when MSHTML.DLL would normally display its context menu
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::ShowContextMenu(
				DWORD /*dwID*/, 
				POINT* /*pptPosition*/,
				IUnknown* /*pCommandTarget*/,
				IDispatch* /*pDispatchObjectHit*/)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return S_OK; // We've shown our own context menu. MSHTML.DLL will no longer try to show its own.
}

// * CImpIDocHostUIHandler::TranslateAccelerator
// *
// * Purpose: Called from MSHTML.DLL's TranslateAccelerator routines
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg,
            /* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
            /* [in] */ DWORD nCmdID)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return S_FALSE;
}

// * CImpIDocHostUIHandler::GetOptionKeyPath
// *
// * Purpose: Called by MSHTML.DLL to find where the host wishes to store 
// *	its options in the registry
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler::GetOptionKeyPath(BSTR* pbstrKey, DWORD)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

STDMETHODIMP CCustomControlSite::XDocHostUIHandler::GetDropTarget( 
            /* [in] */ IDropTarget __RPC_FAR *pDropTarget,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

STDMETHODIMP CCustomControlSite::XDocHostUIHandler::GetExternal( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	IDispatch * pIDispatch=(IDispatch *)theApp.m_pIDispatch;
	pIDispatch->AddRef();
	*ppDispatch=pIDispatch;
    return S_OK;
}
        
STDMETHODIMP CCustomControlSite::XDocHostUIHandler::TranslateUrl( 
            /* [in] */ DWORD dwTranslate,
            /* [in] */ OLECHAR __RPC_FAR *pchURLIn,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}
        
STDMETHODIMP CCustomControlSite::XDocHostUIHandler::FilterDataObject( 
            /* [in] */ IDataObject __RPC_FAR *pDO,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler)
    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////////////
