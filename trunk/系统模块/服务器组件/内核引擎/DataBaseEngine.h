#ifndef DATA_BASE_ENGINE_HEAD_FILE
#define DATA_BASE_ENGINE_HEAD_FILE

#pragma once

//���ͷ�ļ�
#include "KernelEngineHead.h"
#include "KernelEngine.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//ADO ������
class CADOError : public IDataBaseException
{
	//��������
protected:
	enSQLException					m_enErrorType;						//�������
	CString							m_strErrorDescribe;					//������Ϣ

	//��������
public:
	//���캯��
	CADOError();
	//��������
	virtual ~CADOError();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { return ; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//�쳣����
	virtual HRESULT __cdecl GetExceptionResult(){return 0;};
	//�쳣����
	virtual enSQLException __cdecl GetExceptionType() { return m_enErrorType; }
	//�쳣����
	virtual LPCTSTR __cdecl GetExceptionDescribe(){ return m_strErrorDescribe; }

	//���ܺ���
public:
	//���ô���
	void SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe);
};

//////////////////////////////////////////////////////////////////////////

//���ݿ����
class KERNEL_ENGINE_CLASS CDataBase : public IDataBase
{
	//��Ϣ����
protected:
	CADOError						m_ADOError;							//�������
	CString							m_strConnect;						//�����ַ���
	CString							m_strErrorDescribe;					//������Ϣ

	//״̬����
protected:
	DWORD							m_dwConnectCount;					//���Դ���
	DWORD							m_dwConnectErrorTime;				//����ʱ��
	const DWORD						m_dwResumeConnectCount;				//�ָ�����
	const DWORD						m_dwResumeConnectTime;				//�ָ�ʱ��

	//�ں˱���
protected:
	_CommandPtr						m_DBCommand;						//�������
	_RecordsetPtr					m_DBRecordset;						//��¼������
	_ConnectionPtr					m_DBConnection;						//���ݿ����

	//��������
public:
	//���캯��
	CDataBase();
	//��������
	virtual ~CDataBase();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() {delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//������
	virtual void __cdecl OpenConnection();
	//�ر�����
	virtual void __cdecl CloseConnection();
	virtual bool __cdecl SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//������Ϣ
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);


	//�������
	virtual VOID __cdecl ClearParameters();
	//��ȡ����
	virtual VOID __cdecl GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue);
	//�������
	virtual VOID __cdecl AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue);
	//�л���¼
	virtual VOID __cdecl NextRecordset();
	//�رռ�¼
	virtual VOID __cdecl CloseRecordset();
	//�󶨶���
	virtual VOID __cdecl BindToRecordset(CADORecordBinding * pBind);


	//��¼�ӿ�
public:
	//�����ƶ�
	virtual VOID __cdecl MoveToNext();
	//�Ƶ���ͷ
	virtual VOID __cdecl MoveToFirst();
	//�Ƿ����
	virtual bool __cdecl IsRecordsetEnd();
	//��ȡ��Ŀ
	virtual LONG __cdecl GetRecordCount();
	//������ֵ
	virtual LONG __cdecl GetReturnValue();
	//��ȡ����
	virtual VOID __cdecl GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue);

	//���ƽӿ�
public:
	//�洢����
	virtual VOID __cdecl ExecuteProcess(LPCTSTR pszSPName, bool bRecordset);
	//ִ�����
	virtual VOID __cdecl ExecuteSentence(LPCTSTR pszCommand, bool bRecordset);
public:
	//�ӿں������õĺ���
	//�Ƿ��
	virtual bool __cdecl IsRecordsetOpened();
//�ڲ�����
	//��ȡ����
	LPCTSTR GetComErrorDescribe(CComError & ComError);
	//���ô���
	void SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe);

public:
	//���ô洢����
	virtual void __cdecl SetSPName(LPCTSTR pszSpName);
	virtual bool __cdecl ExecuteCommand(bool bRecordset);
	//�ֶνӿ�
public:
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, WORD & wValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, INT & nValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, LONG & lValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, bool & bValue);


	//ִ�нӿ�
public:
	//ִ�����
	virtual bool __cdecl Execute(LPCTSTR pszCommand);
	virtual void __cdecl ExecuteSql(LPCTSTR sql,LONG DBValue);	
};

//////////////////////////////////////////////////////////////////////////

//���ݿ������
class CDataBaseEngine : public IDataBaseEngine
{
	//�ں˱���
protected:
	bool							m_bService;							//���б�־
	IDataBaseEngineSink				* m_pIDataBaseEngineSink;			//֪ͨ����
	//��������
public:
	//���캯��
	CDataBaseEngine(void);
	//��������
	virtual ~CDataBaseEngine(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() {delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl ConcludeService();
	virtual bool __cdecl SetDataBaseEngineSink(IUnknownEx * pIUnknownEx);
	//���нӿ�
public:
	//ʱ���¼�
	virtual bool __cdecl PostDataBaseTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//�����¼�
	virtual bool __cdecl PostDataBaseControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool __cdecl PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

};

//////////////////////////////////////////////////////////////////////////

#endif