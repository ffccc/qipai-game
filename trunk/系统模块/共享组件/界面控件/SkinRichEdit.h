#ifndef SKIN_RICH_EDIT_HEAD_FILE
#define SKIN_RICH_EDIT_HEAD_FILE

#pragma once

#include "SkinControls.h"


//////////////////////////////////////////////////////////////////////////

//ͼ�� OLE ��
class SKIN_CONTROL_CLASS CImageDataObject : IDataObject
{
	//�ں˱���
protected:
	ULONG							m_ulRefCnt;							//���ü���
	STGMEDIUM 						m_StgMedium;						//������Ϣ
	FORMATETC 						m_FormatEtc;						//������Ϣ

	//�ӿڱ���
protected:
	IStorage *						m_pIStorage;						//����ӿ�
	IOleObject *					m_pIOleObject;						//����ӿ�

	//��������
public:
	//���캯��
	CImageDataObject();
	//��������
	virtual ~CImageDataObject();

	//�����ӿ�
public:
	//��������
	virtual ULONG STDMETHODCALLTYPE AddRef();
	//�ͷŶ���
	virtual ULONG STDMETHODCALLTYPE Release();
	//��ѯ�ӿ�
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, VOID * * ppvObject);

	//����ӿ�
public:
	//��ȡ����
	virtual HRESULT STDMETHODCALLTYPE GetData(FORMATETC * pformatetcIn, STGMEDIUM * pStgMedium);
	//��������
	virtual HRESULT STDMETHODCALLTYPE SetData(FORMATETC * pFormatEtc, STGMEDIUM * pStgMedium, BOOL fRelease);
	
	//�����ӿ�
public:
	virtual HRESULT STDMETHODCALLTYPE DUnadvise(DWORD dwConnection) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC * pFormatEtc) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA * * ppenumAdvise) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC * pFormatEtc, STGMEDIUM *  pStgMedium ) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC * * ppenumFormatEtc ) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC * pFormatEtcIn, FORMATETC * pFormatEtcOut) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC * pFormatEtc, DWORD advf, IAdviseSink * pAdvSink, DWORD * pdwConnection) { return E_NOTIMPL; }

	//���ú���
public:
	//����ͼƬ
	bool SetImage(HBITMAP hBitmap);
	//����ͼƬ
	bool LoadImage(LPCTSTR pszFileName);
	//����ͼƬ
	bool LoadImage(HINSTANCE hInstance, LPCTSTR lpBitmapName);

	//ͼƬ����
public:
	//��ȡͼƬ
	HBITMAP GetBitmap() { return m_StgMedium.hBitmap; }
	//��ȡ��Ϣ
	bool IntercalateReObject(REOBJECT & ReObject, IOleClientSite * pIOleClientSite);
};

//////////////////////////////////////////////////////////////////////////

using namespace ImageOleLib;
class _AFX_RICHEDITEX_STATE
{
public:
    _AFX_RICHEDITEX_STATE();
    virtual ~_AFX_RICHEDITEX_STATE();
    HINSTANCE m_hInstRichEdit20 ;
};


//��Ϣ����
class SKIN_CONTROL_CLASS CSkinRichEdit : public CRichEditCtrl
{
	//�ӿڱ���
protected:
	IRichEditOle *					m_pIRichEditOle;					//OLE ָ��

	//��������
public:
	//���캯��
	CSkinRichEdit();
	//��������
	virtual ~CSkinRichEdit();

	//���غ���
protected:
	//�ؼ���
	virtual VOID PreSubclassWindow();

	//���ܺ���
public:
	//�����Ļ
	bool CleanScreen();
	//������Ϣ
	bool LoadMessage(LPCTSTR pszFileName);
	//������Ϣ
	bool SaveMessage(LPCTSTR pszFileName);

	//ͼƬ����
public:
	//����ͼƬ
	bool InsertImage(CBitmap * pBitmap);
	//����ͼƬ
	void InsertImage( CString strPicPath );
	//�������
	bool InsertDataObject(CImageDataObject * pImageDataObject);

	//�ִ�����
public:
	//�����ִ�
	bool InsertString(LPCTSTR pszString, COLORREF crTextColor);
	//�����ִ�
	bool InsertString(LPCTSTR pszString, COLORREF crTextColor, COLORREF crBackColor);

	//��������
public:
	//�����ִ�
	bool InsertHyperLink(LPCTSTR pszString);
	//�����ִ�
	bool InsertHyperLink(LPCTSTR pszString, COLORREF crBackColor);

	//��������
private:
	//�����ִ�
	bool InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat);
	//���ػص�
	static DWORD CALLBACK LoadCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb);
	//����ص�
	static DWORD CALLBACK SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb);

	//�ڲ�����
private:
	//�ַ��ж�
	bool EfficacyUrlChar(TCHAR chChar);
	//��ַ�ж�
	bool EfficacyUrlString(LPCTSTR pszUrl);

	//�˵�����
protected:
	//ɾ����Ϣ
	VOID OnClearAll();
	//ȫ��ѡ��
	VOID OnSelectAll();
	//�����ַ�
	VOID OnCopyString();
	//������Ϣ
	VOID OnSaveString();
	//ɾ���ַ�
	VOID OnDeleteString();

	//��Ϣӳ��
protected:
	//������Ϣ
	VOID OnDestroy();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�Ҽ���Ϣ
	VOID OnRButtonDown(UINT nFlags, CPoint point);
	//���ù��
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif