#include "StdAfx.h"
#include "Math.h"
#include "DataBaseEngine.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
_COM_SMARTPTR_TYPEDEF(IADORecordBinding,__uuidof(IADORecordBinding));

//Ч������
#define EfficacyResult(hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }

//////////////////////////////////////////////////////////////////////////

//���캯��
CADOError::CADOError()
{
	m_enErrorType=SQLException_None;
}

//��������
CADOError::~CADOError()
{
}

//�ӿڲ�ѯ
void * __cdecl CADOError::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseException,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseException,Guid,dwQueryVer);
	return NULL;
}

//���ô���
void CADOError::SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe)
{
	//���ô���
	m_enErrorType=enErrorType;
	m_strErrorDescribe=pszDescribe;

	//�׳�����
	throw QUERY_ME_INTERFACE(IDataBaseException);

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBase::CDataBase() : m_dwResumeConnectCount(30L),m_dwResumeConnectTime(30L)
{
	//״̬����
	m_dwConnectCount=0;
	m_dwConnectErrorTime=0L;

	//��������
	m_DBCommand.CreateInstance(__uuidof(Command));
	m_DBRecordset.CreateInstance(__uuidof(Recordset));
	m_DBConnection.CreateInstance(__uuidof(Connection));

	//Ч������
	ASSERT(m_DBCommand!=NULL);
	ASSERT(m_DBRecordset!=NULL);
	ASSERT(m_DBConnection!=NULL);
	if (m_DBCommand==NULL) throw TEXT("���ݿ�������󴴽�ʧ��");
	if (m_DBRecordset==NULL) throw TEXT("���ݿ��¼�����󴴽�ʧ��");
	if (m_DBConnection==NULL) throw TEXT("���ݿ����Ӷ��󴴽�ʧ��");

	//���ñ���
	m_DBCommand->CommandType=adCmdStoredProc;

	return;
}

//��������
CDataBase::~CDataBase()
{
	//�ر�����
	CloseConnection();

	//�ͷŶ���
	m_DBCommand.Release();
	m_DBRecordset.Release();
	m_DBConnection.Release();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CDataBase::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBase,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBase,Guid,dwQueryVer);
	return NULL;
}

//������
void __cdecl CDataBase::OpenConnection()
{
	//�������ݿ�
	try
	{
		//�ر�����
		CloseConnection();

		//�������ݿ�
		EfficacyResult(m_DBConnection->Open(_bstr_t(m_strConnect),L"",L"",adConnectUnspecified));
		m_DBConnection->CursorLocation=adUseClient;
		m_DBCommand->ActiveConnection=m_DBConnection;

		//���ñ���
		m_dwConnectCount=0L;
		m_dwConnectErrorTime=0L;

		return ;
	}
	catch (CComError & ComError) { SetErrorInfo(SQLException_Connect,GetComErrorDescribe(ComError)); }

	return ;
}



//�ر�����
void __cdecl CDataBase::CloseConnection()
{
	try
	{
		CloseRecordset();
		if ((m_DBConnection!=NULL)&&(m_DBConnection->GetState()!=adStateClosed))
		{
			EfficacyResult(m_DBConnection->Close());
		}
		return ;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return ;
}


//������Ϣ
bool __cdecl CDataBase::SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//Ч����� 
	ASSERT(szDBName!=NULL);
	ASSERT(szUser!=NULL);
	ASSERT(szPassword!=NULL);

	struct   sockaddr_in   local; 
	local.sin_family   =   AF_INET; 
	local.sin_addr.s_addr  =  dwDBAddr; 
	char   *   pIP   =   inet_ntoa(local.sin_addr); 

	//���������ַ���
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		szPassword,szUser,szDBName,pIP,wPort);

	return true;
}

//������Ϣ
bool __cdecl CDataBase::SetConnectionInfo(LPCTSTR szIP, WORD wPort, LPCTSTR szData, LPCTSTR szName, LPCTSTR szPass)
{
	//Ч����� 
	ASSERT(szIP!=NULL);
	ASSERT(szData!=NULL);
	ASSERT(szName!=NULL);
	ASSERT(szPass!=NULL);

	//���������ַ���
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		szPass,szName,szData,szIP,wPort);

	return true;
}

VOID __cdecl CDataBase::ClearParameters()
{
	try 
	{
		long lParameterCount=m_DBCommand->Parameters->Count;
		if (lParameterCount>0L)
		{
			for (long i=lParameterCount;i>0;i--)
			{
				m_DBCommand->Parameters->Delete(i-1);
			}
		}
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }
}
//��ò���
void __cdecl CDataBase::GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue)//(LPCTSTR pszParamName, _variant_t & vtValue)
{
	//Ч�����
	ASSERT(pszParamName!=NULL);

	//��ȡ����
	try 
	{
		DBVarValue.Clear();
		DBVarValue=m_DBCommand->Parameters->Item[pszParamName]->Value;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}
//ִ�����
VOID __cdecl CDataBase::AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue)
{
	ASSERT(pszName!=NULL);
	try 
	{
        _ParameterPtr Parameter=m_DBCommand->CreateParameter(pszName,Type,Direction,lSize,DBVarValue);
		m_DBCommand->Parameters->Append(Parameter);
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;

	return ;
}

VOID __cdecl CDataBase::NextRecordset()
{
	try 
	{ 
		m_DBRecordset->MoveNext(); 
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}

//�رռ�¼
VOID __cdecl CDataBase::CloseRecordset()
{
	try 
	{
		if (IsRecordsetOpened()) EfficacyResult(m_DBRecordset->Close());
		return ;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return ;
}

VOID __cdecl CDataBase::BindToRecordset(CADORecordBinding * pBind)
{
	ASSERT(pBind!=NULL);
	try 
	{
        IADORecordBindingPtr pIBind(m_DBRecordset);
		pIBind->BindToRecordset(pBind);
		return ;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return ;
}
//�����ƶ�
void __cdecl CDataBase::MoveToNext()
{
	try 
	{ 
		m_DBRecordset->MoveNext(); 
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}
//�Ƶ���ͷ
void __cdecl CDataBase::MoveToFirst()
{
	try 
	{ 
		m_DBRecordset->MoveFirst(); 
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}
//�Ƿ����
bool __cdecl CDataBase::IsRecordsetEnd()
{
	try 
	{
		return (m_DBRecordset->EndOfFile==VARIANT_TRUE); 
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return true;
}
//��ȡ��Ŀ
long __cdecl CDataBase::GetRecordCount()
{
	try
	{
		if (m_DBRecordset==NULL) return 0;
		return m_DBRecordset->GetRecordCount();
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return 0;
}
//��ȡ������ֵ
long __cdecl CDataBase::GetReturnValue()
{
	try 
	{
        _ParameterPtr Parameter;
		long lParameterCount=m_DBCommand->Parameters->Count;
		for (long i=0;i<lParameterCount;i++)
		{
			Parameter=m_DBCommand->Parameters->Item[i];
			if (Parameter->Direction==adParamReturnValue) return Parameter->Value.lVal;
		}
		ASSERT(FALSE);
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return 0;
}
//��ȡ������ֵ
void __cdecl CDataBase::GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	//Ч�����
	ASSERT(pszItem!=NULL);
	try 
	{ 
		DBVarValue.Clear();
		DBVarValue=m_DBRecordset->Fields->GetItem(pszItem)->Value;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }
	return ;
}
//ִ�����
VOID __cdecl CDataBase::ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)
{
	

	return ;
}
//ִ�����
VOID __cdecl CDataBase::ExecuteSentence(LPCTSTR pszCommand, bool bRecordset)
{
	ASSERT(pszCommand!=NULL);
	try
	{
		m_DBConnection->CursorLocation=adUseClient;
		m_DBConnection->Execute(pszCommand,NULL,adExecuteNoRecords);
		return ;
	}
	catch (CComError & ComError) 
	{ 
			SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	catch(...)
	{
		CloseRecordset();
		CloseConnection();
	}
	return ;
}
//�ӿں������õĺ���

//�Ƿ��
bool __cdecl CDataBase::IsRecordsetOpened()
{
	if (m_DBRecordset==NULL) return false;
	if (m_DBRecordset->GetState()==adStateClosed) return false;
	return true;
}

//��ȡ����
LPCTSTR CDataBase::GetComErrorDescribe(CComError & ComError)
{
	_bstr_t bstrDescribe(ComError.Description());
	m_strErrorDescribe.Format(TEXT("ADO ����0x%8x��%s"),ComError.Error(),(LPCTSTR)bstrDescribe);
	return m_strErrorDescribe;
}

//���ô���
void CDataBase::SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe)
{
	m_ADOError.SetErrorInfo(enErrorType,pszDescribe);
	return;
}
///�ӿں������õĺ���



//���ô洢����
void __cdecl CDataBase::SetSPName(LPCTSTR pszSpName)
{
	ASSERT(pszSpName!=NULL);
	try 
	{ 
		m_DBCommand->CommandText=pszSpName; 
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}

//ִ�����
bool __cdecl CDataBase::Execute(LPCTSTR pszCommand)
{
	ASSERT(pszCommand!=NULL);
	try
	{
		m_DBConnection->CursorLocation=adUseClient;
		m_DBConnection->Execute(pszCommand,NULL,adExecuteNoRecords);
		return true;
	}
	catch (CComError & ComError) 
	{ 
			SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	catch(...)
	{
		CloseRecordset();
		CloseConnection();
	}
	return false;
}

//ִ������
bool __cdecl CDataBase::ExecuteCommand(bool bRecordset)
{
	try 
	{
		//�رռ�¼��
		CloseRecordset();

		//ִ������
		if (bRecordset==true)
		{
			m_DBRecordset->PutRefSource(m_DBCommand);
			m_DBRecordset->CursorLocation=adUseClient;
			EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand,vtMissing,adOpenForwardOnly,adLockReadOnly,adOptionUnspecified));
		}
		else 
		{
			m_DBConnection->CursorLocation=adUseClient;
			EfficacyResult(m_DBCommand->Execute(NULL,NULL,adExecuteNoRecords));
		}
		return true;
	}
	catch (CComError & ComError) 
	{ 
			SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	catch(...)
	{
		CloseRecordset();
		CloseConnection();
	}
	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue)
{
	try
	{
		bValue=0;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			{
				bValue=(vtFld.boolVal!=0)?1:0;
				break;
			}
		case VT_I2:
		case VT_UI1:
			{
				bValue=(vtFld.iVal>0)?1:0;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				bValue=0;
				break;
			}
		default: bValue=(BYTE)vtFld.iVal;
		}	
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue)
{
	try
	{
		ulValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) ulValue=vtFld.lVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue)
{	
	try
	{
		dbValue=0.0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_R4:
			{
				dbValue=vtFld.fltVal;
				break;
			}
		case VT_R8:
			{
				dbValue=vtFld.dblVal;
				break;
			}
		case VT_DECIMAL:
			{
				dbValue=vtFld.decVal.Lo32;
				dbValue*=(vtFld.decVal.sign==128)?-1:1;
				dbValue/=pow((float)10,vtFld.decVal.scale); 
				break;
			}
		case VT_UI1:
			{
				dbValue=vtFld.iVal;
				break;
			}
		case VT_I2:
		case VT_I4:
			{
				dbValue=vtFld.lVal;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				dbValue=0.0L;
				break;
			}
		default: dbValue=vtFld.dblVal;
		}
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, LONG & lValue)
{
	try
	{
		lValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) lValue=vtFld.lVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue)
{
	try
	{
		ulValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) ulValue=vtFld.ulVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, INT & nValue)
{
	try
	{
		nValue=0;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			{
				nValue = vtFld.boolVal;
				break;
			}
		case VT_I2:
		case VT_UI1:
			{
				nValue = vtFld.iVal;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				nValue = 0;
				break;
			}
		default: nValue = vtFld.iVal;
		}	
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue)
{
	try
	{
		llValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) llValue=vtFld.llVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize)
{
	try
	{
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if (vtFld.vt==VT_BSTR)
		{
			lstrcpy(szBuffer,(char*)_bstr_t(vtFld));
			return true;
		}
		return false;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, WORD & wValue)
{
	try
	{
		wValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) wValue=(WORD)vtFld.ulVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_DATE:
			{
				COleDateTime TempTime(vtFld);
				Time=TempTime;
				break;
			}
		case VT_EMPTY:
		case VT_NULL:
			{
				Time.SetStatus(COleDateTime::null);
				break;
			}
		default: return false;
		}
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, bool & bValue)
{
	try
	{
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_BOOL:
			{
				bValue=(vtFld.boolVal==0)?false:true;
				break;
			}
		case VT_EMPTY:
		case VT_NULL:
			{
				bValue = false;
				break;
			}
		default:return false;
		}
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}











void CDataBase::ExecuteSql(LPCTSTR strSQL,LONG DBValue)
{
	_variant_t var;			//��ʱ����
	try
	{
		m_DBRecordset->Open(_variant_t(strSQL), 
							m_DBConnection.GetInterfacePtr(),
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
		if(!IsRecordsetEnd())
		{
			//while (!m_DBRecordset->adoEOF)//��¼β
			while (!IsRecordsetEnd())//��¼β
			{
				int iUserRight=0;		//�û�Ȩ��
				//��ȡ��FaceID���ֶ�
				var = m_DBRecordset->GetCollect("UserRight");	
				if (var.vt != VT_NULL)
				{
					iUserRight = atoi( (LPCSTR)_bstr_t(var) );
				}
				iUserRight=iUserRight&DBValue;
				m_DBRecordset->PutCollect("UserRight", _variant_t(iUserRight));
				m_DBRecordset->Update();//��������
				m_DBRecordset->MoveNext();
			}
		}
		m_DBRecordset->Close();
	}		
	catch(_com_error *)
	{
		//AfxMessageBox(e->ErrorMessage());
	}
	catch(...)
	{
		//AfxMessageBox("�������ݿ������ʱ�����쳣.");
		return;
	}
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseEngine::CDataBaseEngine(void)
{
	//���ñ���
	m_bService=false;
	m_pIDataBaseEngineSink=NULL;
	return;
}

//��������
CDataBaseEngine::~CDataBaseEngine(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CDataBaseEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngine,Guid,dwQueryVer);
	return NULL;
}

//��������
bool __cdecl CDataBaseEngine::StartService()
{
	//�ж�״̬
	if (m_bService==true) 
	{
		CTraceService::TraceString(TEXT("���ݿ������ظ�������������������"),TraceLevel_Warning);
		return true;
	}
	//��ҽӿ�
	if (m_pIDataBaseEngineSink==NULL)
	{
		CTraceService::TraceString(TEXT("���ݿ�������ҷ��񲻴���"),TraceLevel_Exception);
		return false;
	}
	//�������
	if (m_pIDataBaseEngineSink->OnDataBaseEngineStart(QUERY_ME_INTERFACE(IUnknownEx))==false)
	{
		CTraceService::TraceString(TEXT("���ݿ�������ҷ�������ʧ��"),TraceLevel_Exception);
		return false;
	}

	//���ñ���
	m_bService=true;

	return true;
}
//ֹͣ����
bool __cdecl CDataBaseEngine::ConcludeService()
{
	//���ñ���
	m_bService=false;

	if(m_pIDataBaseEngineSink!=NULL)
	{
		m_pIDataBaseEngineSink->OnDataBaseEngineStop(QUERY_ME_INTERFACE(IUnknownEx));
	}
	m_pIDataBaseEngineSink=NULL;
	return true;
}
//
bool __cdecl CDataBaseEngine::SetDataBaseEngineSink(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pIUnknownEx!=NULL);
	ASSERT(m_pIDataBaseEngineSink==NULL);
	if (pIUnknownEx==NULL) return false;
	if (m_pIDataBaseEngineSink!=NULL) return false;

	//��ѯ�ӿ�
	m_pIDataBaseEngineSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDataBaseEngineSink);
	if (m_pIDataBaseEngineSink==NULL)
	{
		CTraceService::TraceString(TEXT("���ݿ�������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"),TraceLevel_Exception);
		return false;
	}

	return true;
}

bool __cdecl CDataBaseEngine::PostDataBaseTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return true;
}
bool __cdecl CDataBaseEngine::PostDataBaseControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return true;
}
bool __cdecl CDataBaseEngine::PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	ASSERT(m_pIDataBaseEngineSink!=NULL);
	m_pIDataBaseEngineSink->OnDataBaseEngineRequest(wRequestID, dwContextID, pData, wDataSize);
	return true;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateDataBaseEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CDataBaseEngine * pDataBaseEngine=NULL;
	try
	{
		pDataBaseEngine=new CDataBaseEngine();
		if (pDataBaseEngine==NULL) throw TEXT("����ʧ��");
		void * pObject=pDataBaseEngine->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pDataBaseEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateDataBase(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CDataBase * pDataBase=NULL;
	try
	{
		pDataBase=new CDataBase();
		if (pDataBase==NULL) throw TEXT("����ʧ��");
		void * pObject=pDataBase->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pDataBase);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
