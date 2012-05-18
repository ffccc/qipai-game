#ifndef CUSTSITE_HEAD_FILE
#define CUSTSITE_HEAD_FILE

#include "Afxocc.h"
#include <MSHtmhst.h>

//////////////////////////////////////////////////////////////////////////////////

class CCustomControlSite : public COleControlSite
{
public:
	CCustomControlSite(COleControlContainer * pCnt) : COleControlSite(pCnt){}

protected:
	DECLARE_INTERFACE_MAP();
	BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
		STDMETHOD(ShowContextMenu)(/* [in] */ DWORD dwID,
		/* [in] */ POINT __RPC_FAR *ppt,
		/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
		/* [in] */ IDispatch __RPC_FAR *pdispReserved);
		STDMETHOD(GetHostInfo)( 
		/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
		STDMETHOD(ShowUI)( 
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
		/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
		/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc);
		STDMETHOD(HideUI)(void);
		STDMETHOD(UpdateUI)(void);
		STDMETHOD(EnableModeless)(/* [in] */ BOOL fEnable);
		STDMETHOD(OnDocWindowActivate)(/* [in] */ BOOL fEnable);
		STDMETHOD(OnFrameWindowActivate)(/* [in] */ BOOL fEnable);
		STDMETHOD(ResizeBorder)( 
			/* [in] */ LPCRECT prcBorder,
			/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
			/* [in] */ BOOL fRameWindow);
			STDMETHOD(TranslateAccelerator)( 
			/* [in] */ LPMSG lpMsg,
			/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
			/* [in] */ DWORD nCmdID);
			STDMETHOD(GetOptionKeyPath)( 
			/* [out] */ LPOLESTR __RPC_FAR *pchKey,
			/* [in] */ DWORD dw);
			STDMETHOD(GetDropTarget)(
			/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
			/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
			STDMETHOD(GetExternal)( 
			/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
			STDMETHOD(TranslateUrl)( 
			/* [in] */ DWORD dwTranslate,
			/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
			/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
			STDMETHOD(FilterDataObject)( 
			/* [in] */ IDataObject __RPC_FAR *pDO,
			/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
	END_INTERFACE_PART(DocHostUIHandler)
};

//////////////////////////////////////////////////////////////////////////////////

class CCustomOccManager :public COccManager
{
public:
	CCustomOccManager(){}
	COleControlSite * CreateSite(COleControlContainer * pCtrlCont)
	{
		CCustomControlSite * pOleControlSite=new CCustomControlSite(pCtrlCont);
		return pOleControlSite;
	}
};

//////////////////////////////////////////////////////////////////////////////////

#endif
