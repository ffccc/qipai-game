#ifndef DLG_DOWN_LOAD_HEAD_FILE
#define DLG_DOWN_LOAD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//����״̬
#define DOWN_LOAD_IDLE					0								//����״̬
#define DOWN_LOAD_LINK					1								//����״̬
#define DOWN_LOAD_READ					2								//��ȡ״̬
#define DOWN_LOAD_SETUP					3								//��װ״̬
#define DOWN_LOAD_ERROR					4								//����״̬
#define DOWN_LOAD_FINISH				5								//��װ���

//��Ϣ����
#define WM_ITEM_STATUS_UPDATE			(WM_USER+100)					//״̬����

//////////////////////////////////////////////////////////////////////////////////

//���ػص�
class CDownLoadSink : public CWnd, public IDownLoadSink
{
	//��Ա����
	friend class CDlgDownLoad;

	//״̬����
protected:
	BYTE							m_cbDownLoadStatus;					//����״̬

	//�����ٶ�
protected:
	DWORD							m_dwDownSpeed;						//�����ٶ�
	DWORD							m_dwLastCalcSize;					//�ϴδ�С
	DWORD							m_dwLastCalcTime;					//�ϴ�ʱ��

	//�ļ���Ϣ
protected:
	DWORD							m_dwDownLoadSize;					//���ش�С
	DWORD							m_dwTotalFileSize;					//�ļ���С

	//��������
protected:
	WORD							m_wKindID;							//��Ϸ��ʶ
	WORD							m_wServerID;						//�����ʶ
	TCHAR							m_szClientName[LEN_KIND];			//��������

	//�ں���Ϣ
protected:
	CFile							m_DownFile;							//�����ļ�
	CDownLoad						m_DownLoad;							//�������
	CDlgDownLoad *					m_pDlgDownLoad;						//���ش���
	PROCESS_INFORMATION				m_ProcessInfoMation;				//������Ϣ

	//��������
public:
	//���캯��
	CDownLoadSink();
	//��������
	virtual ~CDownLoadSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܺ���
public:
	//�ر�����
	bool CloseDownLoad();
	//ִ������
	bool PerformDownLoad(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//״̬�ӿ�
public:
	//�����쳣
	virtual bool OnDownLoadError(enDownLoadError DownLoadError);
	//����״̬
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus);

	//���нӿ�
public:
	//��������
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize);
	//������Ϣ
	virtual bool OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation);

	//�ڲ�����
private:
	//ִ�а�װ
	bool PerformInstall();
	//�����ļ�
	bool CreateDownFile(LPCTSTR pszLocation);

	//��Ϣ����
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//���ؿؼ�
class CDlgDownLoad : public CSkinDialog
{
	//״̬����
protected:
	bool							m_bCreate;							//������־
	WORD							m_wCurrentPage;						//��ǰҳ��
	WORD							m_wViewPageCount;					//ҳ����Ŀ
	WORD							m_wViewLineCount;					//ҳ������

	//����״̬
protected:
	WORD							m_wFocusItem;						//��������
	WORD							m_wHoverItem;						//��������

	//��˸����
protected:
	WORD							m_wFlashItem;						//��˸����
	WORD							m_wFlashCount;						//��˸����

	//�ؼ�����
protected:
	CSkinButton						m_btLast;							//ת��һҳ
	CSkinButton						m_btNext;							//ת��һҳ
	CSkinButton						m_btCancelOne;						//ȡ������
	CSkinButton						m_btCancelAll;						//ȫ��ȡ��
	CSkinButton						m_btRetryAgain;						//�ٴ�����

	//��Դ����
protected:
	CSize							m_SizeItemImage;					//�����С
	CSize							m_SizeScaleImage;					//���ȴ�С

	//���ݱ���
protected:
	CWHArray<CDownLoadSink *>		m_DownLoadSinkArray;				//��������

	//��������
public:
	//���캯��
	CDlgDownLoad();
	//��������
	virtual ~CDlgDownLoad();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���غ���
protected:
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//���ܺ���
public:
	//������Ϸ
	bool DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//��������
private:
	//�����ؼ�
	VOID RectifyControl();
	//��˸����
	VOID FlashDownLoadItem(WORD wFlashItem);

	//��������
private:
	//ת������
	WORD SwitchItemIndex(CPoint MousePoint);

	//״̬����
private:
	//ȡ���ж�
	bool VerdictCancelOne(WORD wFocuxIndex);
	//�����ж�
	bool VerdictRetryAgain(WORD wFocuxIndex);
	//��ȡ״̬
	LPCTSTR GetStatusString(BYTE cbDownLoadStatus);

	//��ť��Ϣ
protected:
	//ת��һҳ
	VOID OnBnClickedLast();
	//ת��һҳ
	VOID OnBnClickedNext();
	//ȡ������
	VOID OnBnClickedCancelOne();
	//�ٴ�����
	VOID OnBnClickedRetryAgain();

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnDestroy();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpwndpos);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//�Զ���Ϣ
protected:
	//״̬��Ϣ
	LRESULT OnItemStatusUpdateMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif