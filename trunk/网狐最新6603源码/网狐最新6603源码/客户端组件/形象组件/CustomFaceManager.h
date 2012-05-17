#ifndef CUSTOM_FACE_MANAGER_HEAD_FILE
#define CUSTOM_FACE_MANAGER_HEAD_FILE

#pragma once

#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CWHArray<tagCustomFaceIndex> CCustomFaceIndexArray;

//////////////////////////////////////////////////////////////////////////////////

//ͷ�����
class AVATAR_CONTROL_CLASS CCustomFaceManager : public ICustomFaceManager, public IDownLoadSink
{
	//�������
protected:
	tagCustomFaceInfo				m_CustomFaceInfo;					//�Զ�ͷ��

	//��������
protected:
	CDownLoad						m_DownLoad;							//�������
	CCustomFaceIndexArray			m_CustomFaceIndexArray;				//�ȴ�����

	//�ӿڱ���
protected:
	ICustomFaceEvent *				m_pICustomFaceEvent;				//�ص��ӿ�

	//��̬����
protected:
	static CCustomFaceManager *		m_pCustomFaceManager;				//����ָ��

	//��������
public:
	//���캯��
	CCustomFaceManager();
	//��������
	virtual ~CCustomFaceManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//���ýӿ�
	virtual bool SetCustomFaceEvent(IUnknownEx * pIUnknownEx);
	//����ͷ��
	virtual bool LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID);
	//����ͷ��
	virtual bool LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);
	//����ͷ��
	virtual bool SaveUserCustomFace(DWORD dwUserID, DWORD dwCustomID, DWORD dwCustomFace[FACE_CX*FACE_CY]);

	//״̬�ӿ�
protected:
	//�����쳣
	virtual bool OnDownLoadError(enDownLoadError DownLoadError);
	//����״̬
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus);

	//���нӿ�
protected:
	//��������
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize);
	//������Ϣ
	virtual bool OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation);

	//�ڲ�����
private:
	//ִ������
	bool PerformDownLoad(DWORD dwUserID, DWORD dwCustomID);

	//���ܺ���
public:
	//��ȡʵ��
	static CCustomFaceManager * GetInstance() { return m_pCustomFaceManager; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif