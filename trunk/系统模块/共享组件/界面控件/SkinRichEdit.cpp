#include "Stdafx.h"
#include "SkinRichEdit.h"
#include "richole.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//�˵�����
#define IDM_CLEAR_ALL				(WM_USER+201)						//ɾ����Ϣ
#define IDM_SELECT_ALL				(WM_USER+202)						//ȫ��ѡ��
#define IDM_COPY_STRING				(WM_USER+203)						//�����ַ�
#define IDM_SAVE_STRING				(WM_USER+205)						//������Ϣ
#define IDM_DELETE_STRING			(WM_USER+204)						//ɾ���ַ�

//////////////////////////////////////////////////////////////////////////

//���캯��
CImageDataObject::CImageDataObject()
{
	//�ں˱���
	m_ulRefCnt=0;
	ZeroMemory(&m_StgMedium,sizeof(m_StgMedium));
	ZeroMemory(&m_FormatEtc,sizeof(m_FormatEtc));

	//���ýӿ�
	m_pIStorage=NULL;
	m_pIOleObject=NULL;

	return;
}

//��������
CImageDataObject::~CImageDataObject()
{
	//�ͷŶ���
	::ReleaseStgMedium(&m_StgMedium);

	//�ͷŶ���
	if (m_pIOleObject!=NULL) m_pIOleObject->Release();
	if (m_pIStorage!=NULL) m_pIStorage->Release();

	return;
}

//��������
ULONG STDMETHODCALLTYPE CImageDataObject::AddRef()
{
	m_ulRefCnt++;
	return m_ulRefCnt;
}

//�ͷŶ���
ULONG STDMETHODCALLTYPE CImageDataObject::Release()
{
	if (--m_ulRefCnt==0) delete this;
	return m_ulRefCnt;
}

//��ѯ�ӿ�
HRESULT STDMETHODCALLTYPE CImageDataObject::QueryInterface(REFIID iid, VOID * * ppvObject)
{
	//��ѯ�ӿ�
	if (iid==IID_IUnknown||iid==IID_IDataObject)
	{
		*ppvObject=this;
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

//��ȡ����
HRESULT STDMETHODCALLTYPE CImageDataObject::GetData(FORMATETC * pFormatEtcIn, STGMEDIUM * pStgMedium) 
{
	//״̬�ж�
	ASSERT(m_StgMedium.hBitmap!=NULL);
	if (m_StgMedium.hBitmap==NULL) return E_HANDLE;

	//��ȡ����
	HANDLE hHandle=OleDuplicateData(m_StgMedium.hBitmap,CF_BITMAP,0);
	if (hHandle==NULL) return E_HANDLE;

	//���ñ���
	pStgMedium->tymed=TYMED_GDI;
	pStgMedium->pUnkForRelease=NULL;
	pStgMedium->hBitmap=(HBITMAP)hHandle;

	return S_OK;
}

//��������
HRESULT STDMETHODCALLTYPE CImageDataObject::SetData(FORMATETC * pFormatEtc, STGMEDIUM * pStgMedium, BOOL fRelease) 
{
	//���ñ���
	m_FormatEtc=*pFormatEtc;
	m_StgMedium=*pStgMedium;

	return S_OK;
}

//����ͼƬ
bool CImageDataObject::SetImage(HBITMAP hBitmap)
{
	//������Ϣ
	m_StgMedium.tymed=TYMED_GDI;
	m_StgMedium.hBitmap=hBitmap;
	m_StgMedium.pUnkForRelease=NULL;

	//������Ϣ
	m_FormatEtc.ptd=NULL;
	m_FormatEtc.lindex=-1;
	m_FormatEtc.tymed=TYMED_GDI;
	m_FormatEtc.cfFormat=CF_BITMAP;
	m_FormatEtc.dwAspect=DVASPECT_CONTENT;

	return true;
}

//����ͼƬ
bool CImageDataObject::LoadImage(LPCTSTR pszFileName)
{
	//����ͼƬ
	CImage Image;
	Image.Load(pszFileName);

	//����ж�
	if (Image.IsNull()==true)
	{
		ASSERT(FALSE);
		return false;
	}

	//����ͼƬ
	SetImage(Image.Detach());

	return true;
}

//����ͼƬ
bool CImageDataObject::LoadImage(HINSTANCE hInstance, LPCTSTR lpBitmapName)
{
	//����ͼƬ
	HBITMAP hBitmap=NULL;
	hBitmap=LoadBitmap(hInstance,lpBitmapName);

	//����ͼƬ
	SetImage(hBitmap);

	return true;
}

//��ȡ��Ϣ
bool CImageDataObject::IntercalateReObject(REOBJECT & ReObject, IOleClientSite * pIOleClientSite)
{
	//״̬�ж�
	ASSERT(m_StgMedium.hBitmap!=NULL);
	if (m_StgMedium.hBitmap==NULL) return false;

	//��������
	if ((m_pIOleObject==NULL)||(m_pIStorage==NULL))
	{
		try
		{
			//�����洢
			LPLOCKBYTES lpLockBytes=NULL;
			CreateILockBytesOnHGlobal(NULL,TRUE,&lpLockBytes);
			StgCreateDocfileOnILockBytes(lpLockBytes,STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE,0,&m_pIStorage);

			//��������
			OleCreateStaticFromData(this,IID_IOleObject,OLERENDER_FORMAT,&m_FormatEtc,NULL,m_pIStorage,(VOID * *)&m_pIOleObject);
			OleSetContainedObject(m_pIOleObject,TRUE);

			//����Ч��
			ASSERT((m_pIOleObject!=NULL)&&(m_pIStorage!=NULL));
			if ((m_pIOleObject==NULL)||(m_pIStorage==NULL)) throw 0;
		}
		catch (...)
		{
			//�ͷŶ���
			if (m_pIOleObject!=NULL)
			{
				m_pIOleObject->Release();
				m_pIOleObject=NULL;
			}

			//�ͷŶ���
			if (m_pIStorage!=NULL)
			{
				m_pIStorage->Release();
				m_pIStorage=NULL;
			}

			return false;
		}
	}

	//���ñ���
	ZeroMemory(&ReObject,sizeof(ReObject));

	//������Ϣ
	ReObject.cbStruct=sizeof(ReObject);
	ReObject.cp=REO_CP_SELECTION;
	ReObject.dvaspect=DVASPECT_CONTENT;
	m_pIOleObject->GetUserClassID(&ReObject.clsid);

	//�ӿ���Ϣ
	ReObject.pstg=m_pIStorage;
	ReObject.poleobj=m_pIOleObject;
	ReObject.polesite=pIOleClientSite;

	return true;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinRichEdit, CRichEditCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_CLEAR_ALL, OnClearAll)
	ON_COMMAND(IDM_SELECT_ALL, OnSelectAll)
	ON_COMMAND(IDM_COPY_STRING, OnCopyString)
	ON_COMMAND(IDM_SAVE_STRING, OnSaveString)
	ON_COMMAND(IDM_DELETE_STRING, OnDeleteString)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinRichEdit::CSkinRichEdit()
{
	//���ñ���
	m_pIRichEditOle=NULL;

	return;
}

//��������
CSkinRichEdit::~CSkinRichEdit()
{
	//�ͷŶ���
	if (m_pIRichEditOle!=NULL) 
	{
		m_pIRichEditOle->Release();
		m_pIRichEditOle=NULL;
	}

	return;
}

//�ؼ���
VOID CSkinRichEdit::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//��ʼ���ؼ�
	SetEventMask(ENM_LINK);
	SetOptions(ECOOP_OR,ECO_NOHIDESEL);

	//��ȡ�ӿ�
	m_pIRichEditOle=GetIRichEditOle();

	return;
}

//������Ϣ
INT CSkinRichEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ʼ���ؼ�
	SetEventMask(ENM_LINK);
	SetOptions(ECOOP_OR,ECO_NOHIDESEL);

	//��ȡ�ӿ�
	m_pIRichEditOle=GetIRichEditOle();

	return 0;
}

//�����Ļ
bool CSkinRichEdit::CleanScreen()
{
	//�����Ļ
	SetSel(0,GetWindowTextLength());
	ReplaceSel(TEXT(""));

	return true;
}

//������Ϣ
bool CSkinRichEdit::LoadMessage(LPCTSTR pszFileName)
{
	//Ч�����
	ASSERT(pszFileName!=NULL);
	if (pszFileName==NULL) return false;

	//���ļ�
	CFile File;
	if (File.Open(pszFileName,CFile::modeRead,NULL)==FALSE) return false;

	//��������
	EDITSTREAM EditStream;
	EditStream.pfnCallback=LoadCallBack;
	EditStream.dwCookie=(DWORD)(LONGLONG)(&File);

	//��ȡ�ļ�
    StreamIn(SF_RTF,EditStream);

	return true;
}

//������Ϣ
bool CSkinRichEdit::SaveMessage(LPCTSTR pszFileName)
{
	//��������
	CString strFileName=pszFileName;

	//���ļ�
	if (strFileName.IsEmpty()==true)
	{
		CFileDialog FileDlg(FALSE,TEXT("RTF"),0,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,TEXT("��Ϣ�ļ�(*.RTF)|*.RTF||"),this);
		if (FileDlg.DoModal()==IDOK) strFileName=FileDlg.GetPathName();
		else return true;
	}

	//���ļ�
	CFile File;
	if (File.Open(strFileName,CFile::modeWrite|CFile::modeCreate,NULL)==FALSE) return false;

	//��������
	EDITSTREAM EditStream;
	EditStream.pfnCallback=SaveCallBack;
	EditStream.dwCookie=(DWORD)(LONGLONG)(&File);

	//д���ļ�
    StreamOut(SF_RTF,EditStream);

	return true;
}

//����ͼƬ
bool CSkinRichEdit::InsertImage(CBitmap * pBitmap)
{
	//�������
	CImageDataObject ImageDataObject;
	ImageDataObject.SetImage((HBITMAP)pBitmap->GetSafeHandle());

	//����ͼƬ
	InsertDataObject(&ImageDataObject);

	return false;
}

//����ͼƬ
void CSkinRichEdit::InsertImage( CString strPicPath )
{
	//��������
	LPLOCKBYTES lpLockBytes = NULL;
	SCODE sc;
	HRESULT hr;
	LPOLECLIENTSITE lpClientSite;
	IGifAnimatorPtr	lpAnimator;
	LPSTORAGE lpStorage;
	LPOLEOBJECT	lpObject;

	//������Դ
	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);
	
	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);
	
	//��ȡ�ӿ�
	GetIRichEditOle()->GetClientSite(&lpClientSite);
	ASSERT(lpClientSite != NULL);

	try
	{
		//I��ʼ�ӿ�
		hr = ::CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );
		if( FAILED(hr) )
			_com_issue_error(hr);
		
		//��ȡ����
		hr = lpAnimator.CreateInstance(CLSID_GifAnimator);	
		if( FAILED(hr) )
				_com_issue_error(hr);

		//ת������
		BSTR path = strPicPath.AllocSysString();

		//����ͼƬ
		hr = lpAnimator->LoadFromFile(path);
		if( FAILED(hr) )
			_com_issue_error(hr);
			
		TRACE0( lpAnimator->GetFilePath() );
		
		//��ȡ����
		hr = lpAnimator.QueryInterface(IID_IOleObject, (void**)&lpObject);
		if( FAILED(hr) )
			_com_issue_error(hr);
		
		OleSetContainedObject(lpObject, TRUE);
		
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));

		reobject.cbStruct = sizeof(REOBJECT);	
		CLSID clsid;
		sc = lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);

		//���ñ���
		reobject.clsid = clsid;
		reobject.cp = REO_CP_SELECTION;
		reobject.dvaspect = DVASPECT_CONTENT;
		reobject.dwFlags = REO_BELOWBASELINE;
		reobject.dwUser = 0;
		reobject.poleobj = lpObject;
		reobject.polesite = lpClientSite;
		reobject.pstg = lpStorage;
		
		SIZEL sizel;
		sizel.cx = sizel.cy = 0;
		reobject.sizel = sizel;
		HWND hWndRT = this->m_hWnd;

		GetIRichEditOle()->InsertObject(&reobject);
		::SendMessage(hWndRT, EM_SCROLLCARET, (WPARAM)0, (LPARAM)0);
		VARIANT_BOOL ret;

		ret = lpAnimator->TriggerFrameChange();

		//��ʾͼƬ
		lpObject->DoVerb(OLEIVERB_UIACTIVATE, NULL, lpClientSite, 0, m_hWnd, NULL);
		lpObject->DoVerb(OLEIVERB_SHOW, NULL, lpClientSite, 0, m_hWnd, NULL);
		
		//���´���
		RedrawWindow();

		//�ͷŽӿ�
		if (lpClientSite)
		{
			lpClientSite->Release();
			lpClientSite = NULL;
		}
		if (lpObject)
		{
			lpObject->Release();
			lpObject = NULL;
		}
		if (lpStorage)
		{
			lpStorage->Release();
			lpStorage = NULL;
		}
		
		SysFreeString(path);
	}
	catch( _com_error e )
	{
		::CoUninitialize();	

		//Bmp��ʾ
		try{
		CImage Image;
		if ( SUCCEEDED(Image.Load(strPicPath)) )
		{
			CBitmap Bitmap;
			Bitmap.Attach(HBITMAP(Image));
			InsertImage(&Bitmap);
			Bitmap.Detach();
		}
		}
		catch(...){}
	}
}

//�������
bool CSkinRichEdit::InsertDataObject(CImageDataObject * pImageDataObject)
{
	//Ч�����
	ASSERT((m_pIRichEditOle!=NULL)&&(pImageDataObject!=NULL));
	if ((m_pIRichEditOle==NULL)||(pImageDataObject==NULL)) return false;

	//������Ϣ
	CHARRANGE CharRange;
	GetSel(CharRange);

	//��������
	IOleClientSite * pIOleClientSite=NULL;
	bool bResumeSelect=(CharRange.cpMax!=CharRange.cpMin);
	
	//�����Ϣ
	REOBJECT ReObject;
	ZeroMemory(&ReObject,sizeof(ReObject));
	m_pIRichEditOle->GetClientSite(&pIOleClientSite);

	//����ͼƬ
	if (pImageDataObject->IntercalateReObject(ReObject,pIOleClientSite)==true)
	{
		//����ͼƬ
		SetSel(-1L,-1L);
		m_pIRichEditOle->InsertObject(&ReObject);

		//�ָ���Ϣ
		if (bResumeSelect==true) SetSel(CharRange);
		else PostMessage(WM_VSCROLL,SB_BOTTOM,0);
	}

	//�ͷŶ���
	if (pIOleClientSite!=NULL) pIOleClientSite->Release();

	return true;
}

//�����ִ�
bool CSkinRichEdit::InsertString(LPCTSTR pszString, COLORREF crTextColor)
{
	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR;
	CharFormat.crTextColor=crTextColor;

	//�����ִ�
	InsertString(pszString,CharFormat);

	return true;
}

//�����ִ�
bool CSkinRichEdit::InsertString(LPCTSTR pszString, COLORREF crTextColor, COLORREF crBackColor)
{
	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_BACKCOLOR;
	CharFormat.crTextColor=crTextColor;
	CharFormat.crBackColor=crBackColor;

	//�����ִ�
	InsertString(pszString,CharFormat);

	return true;
}

//�����ִ�
bool CSkinRichEdit::InsertHyperLink(LPCTSTR pszString)
{
	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_LINK;
	CharFormat.dwEffects=CFE_LINK;

	//�����ִ�
	InsertString(pszString,CharFormat);

	return true;
}

//�����ִ�
bool CSkinRichEdit::InsertHyperLink(LPCTSTR pszString, COLORREF crBackColor)
{
	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_BACKCOLOR|CFM_LINK;
	CharFormat.dwEffects=CFE_LINK;
	CharFormat.crBackColor=crBackColor;

	//�����ִ�
	InsertString(pszString,CharFormat);

	return true;
}

//���ù��
BOOL CSkinRichEdit::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//���ù��
	if (nHitTest==HTCAPTION)
	{
		SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�����ִ�
bool CSkinRichEdit::InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat)
{
	//��������
	bool bResumeSelect;
	CHARRANGE CharRange;

	//����״̬
	GetSel(CharRange.cpMin,CharRange.cpMax);
	bResumeSelect=(CharRange.cpMax!=CharRange.cpMin);

	//��������
	LPCTSTR pszHttp=TEXT("http://");
	const INT nHttpLength=lstrlen(pszHttp);
	const INT nStringLength=lstrlen(pszString);

	//��������
	INT nStringStart=0;
	INT nStringPause=0;

	//�ַ�����
	for (INT i=0;i<nStringLength;i++)
	{
		//��������
		INT nUrlPause=i;

		//��ַ�ж�
		if (((i+nHttpLength)<nStringLength)&&(memcmp(&pszString[i],pszHttp,nHttpLength*sizeof(TCHAR))==0))
		{
			//��������
			nUrlPause=i+nHttpLength;

			//��ַ����
			while (nUrlPause<nStringLength)
			{
				//�ַ��ж�
				if (EfficacyUrlChar(pszString[nUrlPause])==true)
				{
					nUrlPause++;
					continue;
				}

				break;
			}
		}

		//��ֹ�ַ�
		if (nUrlPause<=(i+nHttpLength)) nStringPause=(i+1);

		//�����ַ�
		if ((i==(nStringLength-1))||(nUrlPause>(i+nHttpLength)))
		{
			//��ͨ�ַ�
			if (nStringPause>nStringStart)
			{
				//��ȡ����
				CString strNormalString;
				LPTSTR pszNormalString=strNormalString.GetBuffer(nStringPause-nStringStart+1);

				//�����ַ�
				pszNormalString[nStringPause-nStringStart]=0;
				CopyMemory(pszNormalString,&pszString[nStringStart],(nStringPause-nStringStart)*sizeof(TCHAR));

				//�ͷŻ���
				strNormalString.ReleaseBuffer();

				//������Ϣ
				SetSel(-1L,-1L);
				SetWordCharFormat(CharFormat);
				ReplaceSel((LPCTSTR)strNormalString);
			}

			//���ӵ�ַ
			if (nUrlPause>(i+nHttpLength))
			{
				//��ȡ����
				CString strUrlString;
				LPTSTR pszUrlString=strUrlString.GetBuffer((nUrlPause-i)+1);

				//�����ַ�
				pszUrlString[nUrlPause-i]=0;
				CopyMemory(pszUrlString,&pszString[i],(nUrlPause-i)*sizeof(TCHAR));

				//�ͷŻ���
				strUrlString.ReleaseBuffer();

				//�����ʽ
				CHARFORMAT2 CharFormatHyper;
				CharFormatHyper=CharFormat;

				//Ч���ַ
				if (EfficacyUrlString(strUrlString)==true)
				{
					CharFormatHyper.dwMask|=CFM_LINK;
					CharFormatHyper.dwEffects|=CFE_LINK;
				}

				//������Ϣ
				SetSel(-1L,-1L);
				SetWordCharFormat(CharFormatHyper);
				ReplaceSel((LPCTSTR)strUrlString);
			}

			//��������
			nStringStart=__max(i,nUrlPause);
		}

		//��������
		i+=(nUrlPause-i);
	}

	//״̬����
	if (bResumeSelect==true) SetSel(CharRange);
	else PostMessage(WM_VSCROLL,SB_BOTTOM,0);

	return true;
}

//���ػص�
DWORD CALLBACK CSkinRichEdit::LoadCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb)
{
	//��ȡ�ļ�
	*pcb=((CFile *)(LONGLONG)(dwCookie))->Read(pbBuff,cb);

	return 0;
}

//����ص�
DWORD CALLBACK CSkinRichEdit::SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb)
{
	//д���ļ�
	((CFile *)(LONGLONG)(dwCookie))->Write(pbBuff,cb);

	return 0;
}

//�ַ��ж�
bool CSkinRichEdit::EfficacyUrlChar(TCHAR chChar)
{
	//�����ַ�
	if (chChar==TEXT('.')) return true;
	if (chChar==TEXT('=')) return true;
	if (chChar==TEXT('+')) return true;
	if (chChar==TEXT('?')) return true;
	if (chChar==TEXT('#')) return true;
	if (chChar==TEXT('%')) return true;
	if (chChar==TEXT('/')) return true;
	if (chChar==TEXT(':')) return true;
	if (chChar==TEXT('&')) return true;

	//�ַ���Χ
	if ((chChar>=TEXT('a'))&&(chChar<=TEXT('z'))) return true;
	if ((chChar>=TEXT('A'))&&(chChar<=TEXT('Z'))) return true;
	if ((chChar>=TEXT('0'))&&(chChar<=TEXT('9'))) return true;

	return false;
}

//��ַ�ж�
bool CSkinRichEdit::EfficacyUrlString(LPCTSTR pszUrl)
{
	return true;
}

//������Ϣ
VOID CSkinRichEdit::OnDestroy()
{
	__super::OnDestroy();

	//�ͷŽӿ�
	if (m_pIRichEditOle!=NULL)
	{
		m_pIRichEditOle->Release();
		m_pIRichEditOle=NULL;
	}

	return;
}

//�Ҽ���Ϣ
VOID CSkinRichEdit::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ý���
	SetFocus();
	CMenu PopMenu;

	//�����˵�
	PopMenu.CreatePopupMenu();
	PopMenu.AppendMenu(0,IDM_COPY_STRING,TEXT("����(&C) \tCtrl+C"));
	PopMenu.AppendMenu(0,IDM_SELECT_ALL,TEXT("ȫѡ(&A) \tCtrl+A"));
	PopMenu.AppendMenu(0,IDM_DELETE_STRING,TEXT("ɾ��(&D)"));
	PopMenu.AppendMenu(0,IDM_CLEAR_ALL,TEXT("�����Ļ"));
	PopMenu.AppendMenu(0,MF_SEPARATOR);
	PopMenu.AppendMenu(0,IDM_SAVE_STRING,TEXT("������Ϣ..."));

	//���ò˵�
	UINT nSel=((GetSelectionType()!=SEL_EMPTY)?0:MF_GRAYED);
	PopMenu.EnableMenuItem(IDM_COPY_STRING,MF_BYCOMMAND|nSel);
	PopMenu.EnableMenuItem(IDM_DELETE_STRING,MF_BYCOMMAND|nSel);
	PopMenu.EnableMenuItem(IDM_SELECT_ALL,MF_BYCOMMAND);

	//��ʾ�˵�
	ClientToScreen(&Point);
	PopMenu.TrackPopupMenu(TPM_RIGHTBUTTON,Point.x,Point.y,this);
	PopMenu.DestroyMenu();

	return;
}

//ɾ����Ϣ
VOID CSkinRichEdit::OnClearAll()
{
	//ɾ����Ϣ
	SetSel(0L,-1L);
	ReplaceSel(NULL);

	return;
}

//ȫ��ѡ��
VOID CSkinRichEdit::OnSelectAll()
{
	//ȫ��ѡ��
	SetSel(0L,-1L);

	return;
}

//�����ַ�
VOID CSkinRichEdit::OnCopyString()
{
	//�����ַ�
	Copy();

	return;
}

//������Ϣ
VOID CSkinRichEdit::OnSaveString()
{
	//������Ϣ
	SaveMessage(NULL);

	return;
}

//ɾ���ַ�
VOID CSkinRichEdit::OnDeleteString()
{
	//ɾ���ַ�
	ReplaceSel(TEXT(""));

	return;
}

//////////////////////////////////////////////////////////////////////////

