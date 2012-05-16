#ifndef SERVICE_PARAMETER_HEAD_FILE
#define SERVICE_PARAMETER_HEAD_FILE
#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define VER_PARAMETER				2									//�ļ��汾

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagOptionParameter
{
	TCHAR							szDataBaseAddr[16];					//���ݿ��ַ
	TCHAR							szDataBaseName[32];					//���ݿ�����
	TCHAR							szModuleName[MODULE_LEN];			//ģ������
	tagGameServiceOption			GameServiceOption;					//��������
};

//��¼����
struct tagRecordParameter
{
	WORD							wVersion;							//���ð汾
	WORD							wFileSize;							//�ļ���С
	tagOptionParameter				OptionParameter;					//���ò���
};

//////////////////////////////////////////////////////////////////////////

//���������
class CServiceParameter : public CDialog
{
	//��������
protected:
	bool							m_bLoaded;							//���ر�־
	tagOptionParameter				m_OptionParameter;					//���ò���

	//�������
protected:
	HINSTANCE						m_hDllInstance;						//��Ϸʵ��
	tagGameServiceAttrib			m_GameServiceAttrib;				//��������
	IGameServiceManager				* m_pIGameServiceManager;			//����ӿ�

	//��������
public:
	//���캯��
	CServiceParameter(void);
	//��������
	virtual ~CServiceParameter(void);

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK();

	//���ܺ���
public:
	//�Ƿ����
	bool IsLoaded() { return m_bLoaded; }
	//��ȡ����
	const tagOptionParameter & GetOptionParameter() { return m_OptionParameter; }

	//������
public:
	//���ò���
	void ResetParameter();
	//�޸Ĳ���
	bool ConfigParameter();
	//��������
	bool LoadParameter(LPCTSTR pszFileName);
	//��������
	bool SaveParameter(LPCTSTR pszFileName);
	//��������
	bool CreateParameter(LPCTSTR pszModuleName);

	//�ڲ�����
private:
	//����ģ��
	bool LoadGameServiceModule(LPCTSTR pszModuleName);
	//�ͷ�ģ��
	void FreeGameServiceModule();
	//��������
	void UpdateOptionParameter();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif