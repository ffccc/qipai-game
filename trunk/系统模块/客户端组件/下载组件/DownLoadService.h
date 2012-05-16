#ifndef DOWN_LOAD_SERVICE_HEAD_FILE
#define DOWN_LOAD_SERVICE_HEAD_FILE

#pragma once

#include "AfxInet.h"
#include "Resource.h"
#include "DownLoad.h"

//��˵��
class CDownLoadMission;
class CDownLoadService;
typedef CArrayTemplate<CDownLoadMission *> CDownLoadMissionArray;

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����״̬
enum enDownLoadStatus
{
	enDownLoadStatus_Unknow,			//δ֪״̬
	enDownLoadStatus_Ready,				//׼��״̬
	enDownLoadStatus_DownLoadIng,		//����״̬
	enDownLoadStatus_Finish,			//���״̬
	enDownLoadStatus_Fails,				//����ʧ��
};

//����ö��
enum enDownLoadResult
{
	enDownLoadResult_Noknow,			//û�д���
	enDownLoadResult_Exception,			//�쳣����
	enDownLoadResult_CreateFileFails,	//����ʧ��
	enDownLoadResult_InternetReadError,	//��ȡ����
};

//����״̬
struct tagDownLoadStatus
{
	WORD								wProgress;						//���ؽ���
	TCHAR								szStatus[128];					//״̬����
	enDownLoadStatus					DownLoadStatus;					//����״̬
};

//////////////////////////////////////////////////////////////////////////

//�����߳�
class CDownLoadThread : public CServiceThread
{
	//��������
protected:
	bool							m_bPreparative;						//��ʼ��־
	tagDownLoadRequest				* m_pDownLoadRequest;				//��������

	//��Ϣ����
protected:
	TCHAR							m_szTempFile[MAX_PATH];				//��ʱ�ļ�
	TCHAR							m_szLocalFile[MAX_PATH];			//�����ļ�

	//״̬����
protected:
	CCriticalSection				m_CriticalSection;					//�߳�����
	enDownLoadStatus				m_DownLoadStatus;					//����״̬
	enDownLoadResult				m_DownLoadResult;					//���״̬
	DWORD							m_dwDownLoadSize;					//���ش�С
	DWORD							m_dwTotalFileSize;					//�ļ���С

	//�̱߳���
protected:
	CFile							m_LocalFile;						//�����ļ�
	CHttpFile						* m_pHttpFile;						//�����ļ�
	CHttpConnection					* m_pHttpConnection;				//��������
	CInternetSession				m_InternetSession;					//����Ự

	//��������
public:
	//���캯��
	CDownLoadThread();
	//��������
	virtual ~CDownLoadThread();

	//���ܺ���
public:
	//��ʼ���߳�
	bool InitThread(tagDownLoadRequest * pDownLoadRequest);
	//����״̬
	void GetDownLoadStatus(tagDownLoadStatus & DownLoadStatus);
	//Ŀ���ļ�
	LPCTSTR GetDownLoadFileName();

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
	//�ر��¼�
	virtual bool OnEventThreadConclude();

	//�̺߳���
private:
	//����׼��
	void DownLoadPreparative();
	//��������
	void DownLoadCleanUp();
};

//////////////////////////////////////////////////////////////////////////

//����ص��ӿ�
interface IDownLoadMissionSink
{
	//����֪ͨ
	virtual void OnMissionFinish(enDownLoadStatus DownLoadStatus, CDownLoadMission * pDownLoadMission)=NULL;
};

//��������
class CDownLoadMission : public CSkinDialogEx
{
	//��������
protected:
	DWORD								m_dwMissionID;					//�����ʶ
	DWORD								m_dwDownLoadType;				//��������
	tagDownLoadRequest					m_DownLoadRequest;				//��������
	IDownLoadMissionSink				* m_pIDownLoadMissionSink;		//�ص��ӿ�

	//�ؼ�����
protected:
	CSkinButton							m_btReTry;						//�򿪰�ť
	CSkinButton							m_btCancel;						//�򿪰�ť
	CProgressCtrl						m_ProgressCtrl;					//���ȿؼ�
	CDownLoadThread						m_DownLoadThread;				//�����߳�
	CSkinHyperLink						m_DownLoadUrl;					//���ص�ַ

	//��������
public:
	//���캯��
	CDownLoadMission(IDownLoadMissionSink * pIDownLoadMissionSink);
	//��������
	virtual ~CDownLoadMission();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel();

	//���ܺ���
public:
	//�����ʶ
	DWORD GetMissionID() { return m_dwMissionID; }
	//��������
	DWORD GetDownLoadType() { return m_dwDownLoadType; }
	//��ʼ����
	bool StartDownLoad(DWORD dwMissionID, DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest);
	//ֹͣ����
	bool StopDownLoad();
	//�Ա�����
	bool CompareRequest(tagDownLoadRequest * pDownLoadRequest);
	//��ʾ����
	bool DisplayDownLoadFace(bool bDisplay);

	//��Ϣ����
protected:
	//���԰�ť
	afx_msg void OnBnClickedReTry();
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//���ط���
class CDownLoadService : public IDownLoadService, public IDownLoadMissionSink
{
	//��������
protected:
	DWORD								m_dwMissionID;					//�����ʶ
	IDownLoadServiceSink				* m_pIDownLoadServiceSink;		//�ص��ӿ�
	CDownLoadMissionArray				m_DownLoadMissionActive;		//��������
	CDownLoadMissionArray				m_DownLoadMissionRelease;		//��������

	//��������
public:
	//���캯��
	CDownLoadService();
	//��������
	virtual ~CDownLoadService();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��ȡ��Ŀ
	virtual INT_PTR __cdecl GetDownLoadMissionCount();
	//���ýӿ�
	virtual bool __cdecl SetDownLoadServiceSink(IUnknownEx * pIUnknownEx);
	//��������
	virtual DWORD __cdecl AddDownLoadRequest(DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest);

	//�¼��ӿ�
public:
	//����֪ͨ
	virtual void OnMissionFinish(enDownLoadStatus DownLoadStatus, CDownLoadMission * pDownLoadMission);

	//�ڲ�����
private:
	//��������
	CDownLoadMission * SearchMission(tagDownLoadRequest * pDownLoadRequest);
};

//////////////////////////////////////////////////////////////////////////

#endif