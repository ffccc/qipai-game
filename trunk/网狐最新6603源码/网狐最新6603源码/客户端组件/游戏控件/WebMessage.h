#ifndef WEB_MESSAGE_HEAD_FILE
#define WEB_MESSAGE_HEAD_FILE

#pragma once

#include "AfxHtml.h"
#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��ҳ��Ϣ
class SHARE_CONTROL_CLASS CWebMessage : public CWnd, public IDownLoadSink
{
	//���ñ���
protected:
	COLORREF						m_crBackGroup;						//������ɫ

	//���ݱ���
protected:
	DWORD							m_dwDataSize;						//���ݴ�С
	DWORD							m_dwBufferSize;						//�����С
	LPBYTE							m_pcbDataBuffer;					//���ݻ���

	//�������
protected:
	CDownLoad						m_DownLoad;							//�������

	//��������
public:
	//���캯��
	CWebMessage();
	//��������
	virtual ~CWebMessage();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

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

	//��Ϣ����
protected:
	//�滭��Ϣ
	VOID OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif