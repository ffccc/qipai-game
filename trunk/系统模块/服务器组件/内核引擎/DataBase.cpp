#include "StdAfx.h"
#include "Math.h"
#include "TraceService.h"
#include "DataBase.h"

//////////////////////////////////////////////////////////////////////////

//宏定义
_COM_SMARTPTR_TYPEDEF(IADORecordBinding, __uuidof(IADORecordBinding));

//效验结果宏
#define EfficacyResult(hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }

//////////////////////////////////////////////////////////////////////////

//构造函数
CADOError::CADOError()
{
	m_enErrorType = SQLException_None;
}

//析构函数
CADOError::~CADOError()
{
}

//接口查询
void * __cdecl CADOError::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseException, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseException, Guid, dwQueryVer);
	return NULL;
}

//设置错误
void CADOError::SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe)
{
	//设置错误
	m_enErrorType = enErrorType;
	m_strErrorDescribe = pszDescribe;

	//抛出错误
	throw QUERY_ME_INTERFACE(IDataBaseException);

	return;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBase::CDataBase() : m_dwResumeConnectCount(30L), m_dwResumeConnectTime(30L)
{
	//状态变量
	m_dwConnectCount = 0;
	m_dwConnectErrorTime = 0L;

	//创建对象
	m_DBCommand.CreateInstance(__uuidof(Command));
	m_DBRecordset.CreateInstance(__uuidof(Recordset));
	m_DBConnection.CreateInstance(__uuidof(Connection));

	//效验数据
	ASSERT(m_DBCommand != NULL);
	ASSERT(m_DBRecordset != NULL);
	ASSERT(m_DBConnection != NULL);
	if (m_DBCommand == NULL) throw TEXT("数据库命令对象创建失败");
	if (m_DBRecordset == NULL) throw TEXT("数据库记录集对象创建失败");
	if (m_DBConnection == NULL) throw TEXT("数据库连接对象创建失败");

	//设置变量
	m_DBCommand->CommandType = adCmdStoredProc;

	return;
}

//析构函数
CDataBase::~CDataBase()
{
	//关闭连接
	CloseConnection();

	//释放对象
	m_DBCommand.Release();
	m_DBRecordset.Release();
	m_DBConnection.Release();

	return;
}

//接口查询
void * __cdecl CDataBase::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBase, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBase, Guid, dwQueryVer);
	return NULL;
}

//打开连接
VOID __cdecl CDataBase::OpenConnection()
{
	//连接数据库
	try
	{
		//关闭连接
		CloseConnection();

		//连接数据库
		EfficacyResult(m_DBConnection->Open(_bstr_t(m_strConnect), L"", L"", adConnectUnspecified));
		m_DBConnection->CursorLocation = adUseClient;
		m_DBCommand->ActiveConnection = m_DBConnection;

		//设置变量
		m_dwConnectCount = 0L;
		m_dwConnectErrorTime = 0L;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//关闭记录
VOID __cdecl CDataBase::CloseRecordset()
{
	try
	{
		if (IsRecordsetOpened()) EfficacyResult(m_DBRecordset->Close());
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//关闭连接
VOID __cdecl CDataBase::CloseConnection()
{
	try
	{
		CloseRecordset();
		if ((m_DBConnection != NULL) && (m_DBConnection->GetState() != adStateClosed))
		{
			EfficacyResult(m_DBConnection->Close());
		}
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//重新连接
bool __cdecl CDataBase::TryConnectAgain(bool bFocusConnect, CComError * pComError)
{
	try
	{
		//判断重连
		bool bReConnect = bFocusConnect;
		if (bReConnect == false)
		{
			DWORD dwNowTime = (DWORD)time(NULL);
			if ((m_dwConnectErrorTime + m_dwResumeConnectTime) > dwNowTime) bReConnect = true;
		}
		if ((bReConnect == false) && (m_dwConnectCount > m_dwResumeConnectCount)) bReConnect = true;

		//设置变量
		m_dwConnectCount++;
		m_dwConnectErrorTime = (DWORD)time(NULL);
		if (bReConnect == false)
		{
			if (pComError != NULL) SetErrorInfo(SQLException_Connect, GetComErrorDescribe(*pComError));
			return false;
		}

		//重新连接
		OpenConnection();
		return true;
	}
	catch (IDataBaseException * pIDataBaseException)
	{
		//重新连接错误
		if (pComError != NULL) SetErrorInfo(SQLException_Connect, GetComErrorDescribe(*pComError));
		else throw pIDataBaseException;
	}

	return false;
}

//连接信息
bool __cdecl CDataBase::SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//效验参数
	ASSERT(dwDBAddr != 0);
	ASSERT(szDBName != NULL);
	ASSERT(szUser != NULL);
	ASSERT(szPassword != NULL);

	BYTE a = (BYTE)((dwDBAddr >> 24) & 0xFF);
	BYTE b = (BYTE)((dwDBAddr >> 16) & 0xFF);
	BYTE c = (BYTE)((dwDBAddr >> 8) & 0xFF);
	BYTE d = (BYTE)(dwDBAddr & 0xFF);
	try
	{
		//构造连接字符串
		m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%d.%d.%d.%d;Data Source=%s,%ld;"),
		                    szPassword, szUser, szDBName, a, b, c, d, wPort);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
	return true;
}

//切换记录
VOID __cdecl CDataBase::NextRecordset()
{
	try
	{
		VARIANT lngRec;
		m_DBRecordset->NextRecordset(&lngRec);
		return;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

}

//设置信息
bool __cdecl CDataBase::SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//效验参数
	ASSERT(szDBAddr != NULL);
	ASSERT(szDBName != NULL);
	ASSERT(szUser != NULL);
	ASSERT(szPassword != NULL);
	try
	{
		//构造连接字符串
		m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		                    szPassword, szUser, szDBName, szDBAddr, wPort);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
	return true;
}

//是否连接错误
bool __cdecl CDataBase::IsConnectError()
{
	try
	{
		//状态判断
		if (m_DBConnection == NULL) return true;
		if (m_DBConnection->GetState() == adStateClosed) return true;

		//参数判断
		long lErrorCount = m_DBConnection->Errors->Count;
		if (lErrorCount > 0L)
		{
			ErrorPtr pError = NULL;
			for (long i = 0; i < lErrorCount; i++)
			{
				pError = m_DBConnection->Errors->GetItem(i);
				if (pError->Number == 0x80004005) return true;
			}
		}

		return false;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//是否打开
bool __cdecl CDataBase::IsRecordsetOpened()
{
	if (m_DBRecordset == NULL) return false;
	if (m_DBRecordset->GetState() == adStateClosed) return false;
	return true;
}

//往下移动
void __cdecl CDataBase::MoveToNext()
{
	try
	{
		m_DBRecordset->MoveNext();
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return;
}

//移到开头
void __cdecl CDataBase::MoveToFirst()
{
	try
	{
		m_DBRecordset->MoveFirst();
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return;
}

//是否结束
bool __cdecl CDataBase::IsRecordsetEnd()
{
	try
	{
		return (m_DBRecordset->EndOfFile == VARIANT_TRUE);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return true;
}

//获取数目
long __cdecl CDataBase::GetRecordCount()
{
	try
	{
		if (m_DBRecordset == NULL) return 0;
		return m_DBRecordset->GetRecordCount();
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return 0;
}

//获取大小
long __cdecl CDataBase::GetActualSize(LPCTSTR pszParamName)
{
	ASSERT(pszParamName != NULL);
	try
	{
		return m_DBRecordset->Fields->Item[pszParamName]->ActualSize;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return -1;
}

//绑定对象
VOID __cdecl CDataBase::BindToRecordset(CADORecordBinding * pBind)
{
	ASSERT(pBind != NULL);
	try
	{
		IADORecordBindingPtr pIBind(m_DBRecordset);
		pIBind->BindToRecordset(pBind);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//获取数据
VOID __cdecl CDataBase::GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	ASSERT(pszItem != NULL);
	try
	{
		DBVarValue = m_DBRecordset->Fields->GetItem(pszItem)->Value;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue)
{
	try
	{
		bValue = 0;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_BOOL:
			{
				bValue = (vtFld.boolVal != 0) ? 2 : 0;
				break;
			}
			case VT_I2:
			case VT_UI1:
			{
				bValue = (vtFld.iVal > 0) ? 2 : 0;
				break;
			}
			case VT_NULL:
			case VT_EMPTY:
			{
				bValue = 0;
				break;
			}
			default:
				bValue = (BYTE)vtFld.iVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue)
{
	try
	{
		ulValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) ulValue = vtFld.lVal;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue)
{
	try
	{
		dbValue = 0.0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_R4:
			{
				dbValue = vtFld.fltVal;
				break;
			}
			case VT_R8:
			{
				dbValue = vtFld.dblVal;
				break;
			}
			case VT_DECIMAL:
			{
				dbValue = vtFld.decVal.Lo32;
				dbValue *= (vtFld.decVal.sign == 128) ? -1 : 1;
				dbValue /= pow(10.0, vtFld.decVal.scale);
				break;
			}
			case VT_UI1:
			{
				dbValue = vtFld.iVal;
				break;
			}
			case VT_I2:
			case VT_I4:
			{
				dbValue = vtFld.lVal;
				break;
			}
			case VT_NULL:
			case VT_EMPTY:
			{
				dbValue = 0.0L;
				break;
			}
			default:
				dbValue = vtFld.dblVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, LONG & lValue)
{
	try
	{
		lValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) lValue = vtFld.lVal;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue)
{
	try
	{
		ulValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) ulValue = vtFld.ulVal;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, INT & nValue)
{
	try
	{
		nValue = 0;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
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
			default:
				nValue = vtFld.iVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue)
{
	try
	{
		llValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) llValue = vtFld.llVal;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if (vtFld.vt == VT_BSTR)
		{
			lstrcpy(szBuffer, (char*)_bstr_t(vtFld));
			return true;
		}
		return false;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, WORD & wValue)
{
	try
	{
		wValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL) && (vtFld.vt != VT_EMPTY)) wValue = (WORD)vtFld.ulVal;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_DATE:
			{
				COleDateTime TempTime(vtFld);
				Time = TempTime;
				break;
			}
			case VT_EMPTY:
			case VT_NULL:
			{
				Time.SetStatus(COleDateTime::null);
				break;
			}
			default:
				return false;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取参数
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, bool & bValue)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch (vtFld.vt)
		{
			case VT_BOOL:
			{
				bValue = (vtFld.boolVal == 0) ? false : true;
				break;
			}
			case VT_EMPTY:
			case VT_NULL:
			{
				bValue = false;
				break;
			}
			default:
				return false;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return false;
}

//获取返回数值
long __cdecl CDataBase::GetReturnValue()
{
	try
	{
		_ParameterPtr Parameter;
		long lParameterCount = m_DBCommand->Parameters->Count;
		for (long i = 0; i < lParameterCount; i++)
		{
			Parameter = m_DBCommand->Parameters->Item[i];
			if (Parameter->Direction == adParamReturnValue) return Parameter->Value.lVal;
		}
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return 0;
}

//删除参数
void __cdecl CDataBase::ClearParameters()
{
	try
	{
		long lParameterCount = m_DBCommand->Parameters->Count;
		if (lParameterCount > 0L)
		{
			for (long i = lParameterCount; i > 0; i--)
			{
				m_DBCommand->Parameters->Delete(i - 1);
			}
		}
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return;
}


//获得参数
void __cdecl CDataBase::GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue)
{
	//效验参数
	ASSERT(pszParamName != NULL);

	//获取参数
	try
	{
		DBVarValue.Clear();
		DBVarValue = m_DBCommand->Parameters->Item[pszParamName]->Value;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}

	return;
}

//插入参数
void __cdecl CDataBase::AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue)
{
	ASSERT(pszName != NULL);
	try
	{
		_ParameterPtr Parameter = m_DBCommand->CreateParameter(pszName, Type, Direction, lSize, DBVarValue);
		m_DBCommand->Parameters->Append(Parameter);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//执行语句
VOID __cdecl CDataBase::ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)
{
	ASSERT(pszSPName != NULL);
	try
	{
		//关闭记录集
		CloseRecordset();

		m_DBCommand->CommandText = pszSPName;

		//执行命令
		if (bRecordset == true)
		{
			m_DBRecordset->PutRefSource(m_DBCommand);
			m_DBRecordset->CursorLocation = adUseClient;
			EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand, vtMissing, adOpenForwardOnly, adLockReadOnly, adOptionUnspecified));
		}
		else
		{
			m_DBConnection->CursorLocation = adUseClient;
			EfficacyResult(m_DBCommand->Execute(NULL, NULL, adExecuteNoRecords));
		}
	}
	catch (CComError & ComError)
	{
		if (IsConnectError() == true)	TryConnectAgain(false, &ComError);
		else SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//执行命令
VOID __cdecl CDataBase::ExecuteSentence(LPCTSTR pszCommand, bool bRecordset)
{
	ASSERT(pszCommand != NULL);
	try
	{
		m_DBConnection->CursorLocation = adUseClient;
		m_DBConnection->Execute(pszCommand, NULL, adExecuteNoRecords);
	}
	catch (CComError & ComError)
	{
		if (IsConnectError() == true)	TryConnectAgain(false, &ComError);
		else SetErrorInfo(SQLException_Syntax, GetComErrorDescribe(ComError));
	}
}

//获取错误
LPCTSTR CDataBase::GetComErrorDescribe(CComError & ComError)
{
	_bstr_t bstrDescribe(ComError.Description());
	m_strErrorDescribe.Format(TEXT("ADO 错误：0x%8x，%s"), ComError.Error(), (LPCTSTR)bstrDescribe);
	return m_strErrorDescribe;
}

//设置错误
void CDataBase::SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe)
{
	m_ADOError.SetErrorInfo(enErrorType, pszDescribe);
	return;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseEngine::CDataBaseEngine(void)
{
	//设置变量
	m_bService = false;
	m_pIDataBaseEngineSink = NULL;

	return;
}

//析构函数
CDataBaseEngine::~CDataBaseEngine(void)
{
}

//接口查询
void * __cdecl CDataBaseEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngine, Guid, dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngine, Guid, dwQueryVer);
	return NULL;
}

//注册接口
bool __cdecl CDataBaseEngine::SetDataBaseEngineSink(IUnknownEx * pIUnknownEx)
{
	//效验参数
	ASSERT(pIUnknownEx != NULL);
	ASSERT(m_pIDataBaseEngineSink == NULL);
	if (pIUnknownEx == NULL) return false;
	if (m_pIDataBaseEngineSink != NULL) return false;

	//查询接口
	m_pIDataBaseEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IDataBaseEngineSink);
	if (m_pIDataBaseEngineSink == NULL)
	{
		CTraceService::TraceString(TEXT("数据库引擎外挂服务接口获取失败，挂接操作失败"), TraceLevel_Exception);
		return false;
	}

	return true;
}

//启动服务
bool __cdecl CDataBaseEngine::StartService()
{
	//判断状态
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("数据库引擎重复启动，启动操作忽略"), TraceLevel_Warning);
		return true;
	}

	//外挂接口
	if (m_pIDataBaseEngineSink == NULL)
	{
		CTraceService::TraceString(TEXT("数据库引擎外挂服务不存在"), TraceLevel_Exception);
		return false;
	}

	//设置队列
	if (m_QueueServiceEvent.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		CTraceService::TraceString(TEXT("数据库引擎与队列服务绑定失败"), TraceLevel_Exception);
		return false;
	}

	//启动外挂
	if (m_pIDataBaseEngineSink->OnDataBaseEngineStart(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		CTraceService::TraceString(TEXT("数据库引擎外挂服务启动失败"), TraceLevel_Exception);
		return false;
	}

	//设置变量
	m_bService = true;

	return true;
}

//停止服务
bool __cdecl CDataBaseEngine::ConcludeService()
{
	//设置变量
	m_bService = false;

	//停止请求队列
	m_QueueServiceEvent.SetQueueServiceSink(NULL);

	//停止外挂
	if (m_pIDataBaseEngineSink != NULL)
	{
		m_pIDataBaseEngineSink->OnDataBaseEngineStop(QUERY_ME_INTERFACE(IUnknownEx));
		m_pIDataBaseEngineSink = NULL;
	}

	m_QueueServiceEvent.SetQueueServiceSink(NULL);

	return true;
}

//队列接口
void __cdecl CDataBaseEngine::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize)
{
	//判断状态
	if (m_bService == false) return;

	//请求处理
	switch (wIdentifier)
	{
		case EVENT_DATABASE:
		{
			//效验参数
			ASSERT(pBuffer != NULL);
			ASSERT(wDataSize >= sizeof(NTY_DataBaseEvent));
			if (wDataSize < sizeof(NTY_DataBaseEvent)) return;

			//变量定义
			NTY_DataBaseEvent * pDataBaseEvent = (NTY_DataBaseEvent *)pBuffer;
			WORD wHandleBuffer = wDataSize - sizeof(NTY_DataBaseEvent);

			//处理数据
			ASSERT(m_pIDataBaseEngineSink != NULL);
			m_pIDataBaseEngineSink->OnDataBaseEngineRequest(pDataBaseEvent->wRequestID, pDataBaseEvent->dwContextID, pDataBaseEvent + 1, wHandleBuffer);

			return;
		}
		case EVENT_TIMER://时间事件
		{
			//效验参数
			ASSERT(pBuffer != NULL);
			ASSERT(wDataSize >= sizeof(NTY_TimerEvent));
			if (wDataSize < sizeof(NTY_TimerEvent)) return;

			//变量定义
			NTY_TimerEvent * pDataBaseEvent = (NTY_TimerEvent *)pBuffer;
			WORD wHandleBuffer = wDataSize - sizeof(NTY_TimerEvent);

			//处理数据
			ASSERT(m_pIDataBaseEngineSink != NULL);
			m_pIDataBaseEngineSink->OnDataBaseEngineTimer(pDataBaseEvent->dwTimerID, pDataBaseEvent->dwBindParameter);

			return;
		}
		case EVENT_CONTROL://控制事件
		{
			//效验参数
			ASSERT(pBuffer != NULL);
			ASSERT(wDataSize >= sizeof(NTY_ControlEvent));
			if (wDataSize < sizeof(NTY_ControlEvent)) return;

			//变量定义
			NTY_ControlEvent * pDataBaseEvent = (NTY_ControlEvent *)pBuffer;
			WORD wHandleBuffer = wDataSize - sizeof(NTY_ControlEvent);

			//处理数据
			ASSERT(m_pIDataBaseEngineSink != NULL);
			m_pIDataBaseEngineSink->OnDataBaseEngineControl(pDataBaseEvent->wControlID, pDataBaseEvent + 1, wHandleBuffer);

			return;
		}
		default:
		{
			ASSERT(false);
		}
	}

	return;
}

//控制事件
//////////////////////////////////////////////////////////////////////////
//时间事件
bool __cdecl CDataBaseEngine::PostDataBaseTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return m_QueueServiceEvent.PostTimerEvent(dwTimerID, dwBindParameter);
}

//控制事件
bool __cdecl CDataBaseEngine::PostDataBaseControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return m_QueueServiceEvent.PostControlEvent(wControlID, pData, wDataSize);
}

//请求事件
bool __cdecl CDataBaseEngine::PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	return m_QueueServiceEvent.PostDataBaseEvent(wRequestID, dwContextID, pData, wDataSize);
}

//////////////////////////////////////////////////////////////////////////

//建立对象函数
extern "C" __declspec(dllexport) void * __cdecl CreateDataBaseEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//建立对象
	CDataBaseEngine * pDataBaseEngine = NULL;
	try
	{
		pDataBaseEngine = new CDataBaseEngine();
		if (pDataBaseEngine == NULL) throw TEXT("创建失败");
		void * pObject = pDataBaseEngine->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pDataBaseEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//建立对象函数
extern "C" __declspec(dllexport) void * __cdecl CreateDataBase(const GUID & Guid, DWORD dwInterfaceVer)
{
	//建立对象
	CDataBase * pDataBase = NULL;
	try
	{
		pDataBase = new CDataBase();
		if (pDataBase == NULL) throw TEXT("创建失败");
		void * pObject = pDataBase->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pDataBase);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
