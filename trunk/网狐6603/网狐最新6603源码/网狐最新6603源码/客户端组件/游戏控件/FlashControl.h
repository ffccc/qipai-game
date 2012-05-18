#ifndef FLASH_CONTROL_HEAD_FILE
#define FLASH_CONTROL_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//���Ͷ���
typedef ShockwaveFlashObjects::IShockwaveFlash IFalshControl;

//////////////////////////////////////////////////////////////////////////////////

//��������
class SHARE_CONTROL_CLASS CFlashActiveX : public ShockwaveFlashObjects::_IShockwaveFlashEvents,
	public IOleClientSite,	public IOleInPlaceSiteWindowless, public IStorage
{
	//�������
protected:
	HWND							m_hWnd;								//���ھ��
	LONG							m_lTotalFrames;						//֡������

	//�ӿڱ���
protected:						 
	IOleObject *					m_pIOleObject;						//�ӿ�ָ��
	IViewObjectEx *					m_pIViewObject;						//�ӿ�ָ��
	IViewObjectEx *					m_pIViewObjectEx;					//�ӿ�ָ��
	IFalshControl *					m_pIFalshControl;					//�ӿ�ָ��

	//���ñ���
protected:
	IFlashControlSink *				m_pIFlashControlSink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CFlashActiveX();
	//��������
	virtual ~CFlashActiveX();

	//���ú���
public:
	//�滭����
	VOID DrawControl(HDC hdcDraw, LPCRECT rcDraw, BOOL bErase);
	//����λ��
	VOID SetPosition(INT nXPos, INT nYPos, INT nWidth, INT nHeight, UINT nFlags);
	//��������
	BOOL CreateControl(DWORD dwStyle, CWnd * pParentWnd, CRect rcCreate, HINSTANCE hInstance, IUnknownEx * pIUnknownEx);

	//���ܺ���
public:
	//��ʼ����
	VOID PlayMovie();
	//ֹͣ����
	VOID StopMovie();
	//���²���
	VOID ReplayMovie();
	//���ض���
	bool LoadMovieFile(LPCTSTR pszFileName);

	//IUnknown
public:
	INT m_nRef;
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID IID, VOID ** ppvObject);

	//IDispatch
public:
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( UINT __RPC_FAR *pctinfo){ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( UINT iTInfo,LCID lcid,ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo){ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE QueryService( REFGUID guidService, REFIID IID,VOID __RPC_FAR *__RPC_FAR *ppvObject) { return 0; }
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( REFIID IID, LPOLESTR __RPC_FAR *rgszNames,UINT cNames, LCID lcid,DISPID __RPC_FAR *rgDispId){ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE Invoke( DISPID dispIdMember,REFIID IID,LCID lcid,WORD wFlags,DISPPARAMS __RPC_FAR *pDispParams,VARIANT __RPC_FAR *pVarResult,EXCEPINFO __RPC_FAR *pExcepInfo,UINT __RPC_FAR *puArgErr){ return S_OK; }

	//IOleClientSite
public:
	virtual HRESULT STDMETHODCALLTYPE ShowObject()																	{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE SaveObject()																	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow)														{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout()														{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk)			{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetContainer(LPOLECONTAINER FAR * ppContainer)								{ *ppContainer=0; return E_NOINTERFACE; }

	//IOleInPlaceSite
public:
	virtual HRESULT STDMETHODCALLTYPE OnUIActivate()																{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate()															{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate()															{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate()															{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable)												{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect)											{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState()															{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo()															{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtent)														{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode)											{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetWindow(HWND FAR * lphwnd)													{ *lphwnd=m_hWnd; return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame,LPOLEINPLACEUIWINDOW FAR* lplpDoc,LPRECT lprcPosRect,LPRECT lprcClipRect,LPOLEINPLACEFRAMEINFO lpFrameInfo);

	//IOleInPlaceSiteEx
public:
	virtual HRESULT STDMETHODCALLTYPE RequestUIActivate(VOID)														{ return S_FALSE; }
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivateEx(BOOL fNoRedraw)											{ return S_FALSE; }
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivateEx(BOOL __RPC_FAR *pfNoRedraw, DWORD dwFlags)				{if (pfNoRedraw)	*pfNoRedraw=FALSE;return S_OK; }

	//IOleInPlaceSiteWindowless
public:
	virtual HRESULT STDMETHODCALLTYPE GetFocus( VOID)																{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE SetFocus( BOOL fFocus)														{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE CanWindowlessActivate( VOID)													{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE GetDC( LPCRECT pRect,DWORD grfFlags,HDC __RPC_FAR *phDC)						{ return S_FALSE; }
	virtual HRESULT STDMETHODCALLTYPE ReleaseDC(HDC hDC)															{ return S_FALSE; }
	virtual HRESULT STDMETHODCALLTYPE GetCapture( VOID)																{ return S_FALSE; }
	virtual HRESULT STDMETHODCALLTYPE SetCapture( BOOL fCapture)													{ return S_FALSE; }
	virtual HRESULT STDMETHODCALLTYPE InvalidateRgn(HRGN hRGN,BOOL fErase)											{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE InvalidateRect(LPCRECT pRect, BOOL fErase)									{ DrawControl(NULL,pRect,fErase); return S_OK; } 
	virtual HRESULT STDMETHODCALLTYPE AdjustRect(LPRECT prc)														{ return S_FALSE; }
	virtual HRESULT STDMETHODCALLTYPE ScrollRect( INT dx,INT dy,LPCRECT pRectScroll,LPCRECT pRectClip)				{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE OnDefWindowMessage(UINT msg,WPARAM wParam,LPARAM lParam,LRESULT __RPC_FAR *plResult){ return S_FALSE; }

	//IStorage
public:
	virtual HRESULT STDMETHODCALLTYPE CreateStream(const WCHAR * pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm)			{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE OpenStream(const WCHAR * pwcsName, VOID * reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm)				{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE CreateStorage(const WCHAR * pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg)			{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE OpenStorage(const WCHAR * pwcsName, IStorage * pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg){ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE CopyTo(DWORD ciidExclude, IID const *rgiidExclude, SNB snbExclude,IStorage *pstgDest)								{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE MoveElementTo(const OLECHAR *pwcsName,IStorage * pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags)			{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags)																						{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE Revert()																											{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE EnumElements(DWORD reserved1, VOID * reserved2, DWORD reserved3, IEnumSTATSTG ** ppenum)							{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE DestroyElement(const OLECHAR *pwcsName)																			{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE RenameElement(const WCHAR *pwcsOldName, const WCHAR *pwcsNewName)													{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE SetElementTimes(const WCHAR *pwcsName, FILETIME const *pctime, FILETIME const *patime, FILETIME const *pmtime)	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE SetClass(REFCLSID clsid)																							{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE SetStateBits(DWORD grfStateBits, DWORD grfMask)																	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG * pstatstg, DWORD grfStatFlag)																		{ return E_NOTIMPL; }
};

//////////////////////////////////////////////////////////////////////////////////

//�����ؼ�
class SHARE_CONTROL_CLASS CFlashControl
{
	//��������
protected:
	CFlashActiveX					m_FlashContainer;					//�ؼ�����
		
	//��������
public:
	//���캯��
	CFlashControl();
	//��������
	virtual ~CFlashControl();

	//���ܺ���
public:
	//��ʼ����
	VOID PlayMovie();
	//ֹͣ����
	VOID StopMovie();
	//���²���
	VOID ReplayMovie();
	//���ض���
	bool LoadMovieFile(LPCTSTR pszFileName);

	//���ܺ���
public:
	//����λ��
	VOID SetPosition(INT nXPos, INT nYPos, INT nWidth, INT nHeight, UINT nFlags);
	//�������
	VOID CreateControl(CWnd * pParentWnd, CRect rcCreate, IUnknownEx * pIUnknownEx);
};

//////////////////////////////////////////////////////////////////////////////////

#endif