#ifndef DATA_BASE_ENGINE_HEAD_FILE
#define DATA_BASE_ENGINE_HEAD_FILE

#pragma once

//组件头文件
#include "KernelEngineHead.h"
#include "KernelEngine.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//ADO 错误类
class CADOError : public IDataBaseException
{
	//变量定义
protected:
	enSQLException					m_enErrorType;						//错误代号
	CString							m_strErrorDescribe;					//错误信息

	//函数定义
public:
	//构造函数
	CADOError();
	//析构函数
	virtual ~CADOError();

	//基础接口
public:
	//释放对象
	virtual void __cdecl Release() { return ; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//功能接口
public:
	//异常代码
	virtual HRESULT __cdecl GetExceptionResult(){return 0;};
	//异常类型
	virtual enSQLException __cdecl GetExceptionType() { return m_enErrorType; }
	//异常描述
	virtual LPCTSTR __cdecl GetExceptionDescribe(){ return m_strErrorDescribe; }

	//功能函数
public:
	//设置错误
	void SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe);
};

//////////////////////////////////////////////////////////////////////////

//数据库对象
class KERNEL_ENGINE_CLASS CDataBase : public IDataBase
{
	//信息变量
protected:
	CADOError						m_ADOError;							//错误对象
	CString							m_strConnect;						//连接字符串
	CString							m_strErrorDescribe;					//错误信息

	//状态变量
protected:
	DWORD							m_dwConnectCount;					//重试次数
	DWORD							m_dwConnectErrorTime;				//错误时间
	const DWORD						m_dwResumeConnectCount;				//恢复次数
	const DWORD						m_dwResumeConnectTime;				//恢复时间

	//内核变量
protected:
	_CommandPtr						m_DBCommand;						//命令对象
	_RecordsetPtr					m_DBRecordset;						//记录集对象
	_ConnectionPtr					m_DBConnection;						//数据库对象

	//函数定义
public:
	//构造函数
	CDataBase();
	//析构函数
	virtual ~CDataBase();

	//基础接口
public:
	//释放对象
	virtual void __cdecl Release() {delete this; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//打开连接
	virtual void __cdecl OpenConnection();
	//关闭连接
	virtual void __cdecl CloseConnection();
	virtual bool __cdecl SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//设置信息
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);


	//清除参数
	virtual VOID __cdecl ClearParameters();
	//获取参数
	virtual VOID __cdecl GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue);
	//插入参数
	virtual VOID __cdecl AddParameter(LPCTSTR pszName, DataTypeEnum Type, ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue);
	//切换记录
	virtual VOID __cdecl NextRecordset();
	//关闭记录
	virtual VOID __cdecl CloseRecordset();
	//绑定对象
	virtual VOID __cdecl BindToRecordset(CADORecordBinding * pBind);


	//记录接口
public:
	//往下移动
	virtual VOID __cdecl MoveToNext();
	//移到开头
	virtual VOID __cdecl MoveToFirst();
	//是否结束
	virtual bool __cdecl IsRecordsetEnd();
	//获取数目
	virtual LONG __cdecl GetRecordCount();
	//返回数值
	virtual LONG __cdecl GetReturnValue();
	//获取数据
	virtual VOID __cdecl GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue);

	//控制接口
public:
	//存储过程
	virtual VOID __cdecl ExecuteProcess(LPCTSTR pszSPName, bool bRecordset);
	//执行语句
	virtual VOID __cdecl ExecuteSentence(LPCTSTR pszCommand, bool bRecordset);
public:
	//接口函数引用的函数
	//是否打开
	virtual bool __cdecl IsRecordsetOpened();
//内部函数
	//获取错误
	LPCTSTR GetComErrorDescribe(CComError & ComError);
	//设置错误
	void SetErrorInfo(enSQLException enErrorType, LPCTSTR pszDescribe);

public:
	//设置存储过程
	virtual void __cdecl SetSPName(LPCTSTR pszSpName);
	virtual bool __cdecl ExecuteCommand(bool bRecordset);
	//字段接口
public:
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, WORD & wValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, INT & nValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, LONG & lValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time);
	//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, bool & bValue);


	//执行接口
public:
	//执行语句
	virtual bool __cdecl Execute(LPCTSTR pszCommand);
	virtual void __cdecl ExecuteSql(LPCTSTR sql,LONG DBValue);	
};

//////////////////////////////////////////////////////////////////////////

//数据库管理类
class CDataBaseEngine : public IDataBaseEngine
{
	//内核变量
protected:
	bool							m_bService;							//运行标志
	IDataBaseEngineSink				* m_pIDataBaseEngineSink;			//通知钩子
	//函数定义
public:
	//构造函数
	CDataBaseEngine(void);
	//析构函数
	virtual ~CDataBaseEngine(void);

	//基础接口
public:
	//释放对象
	virtual void __cdecl Release() {delete this; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//服务接口
public:
	//启动服务
	virtual bool __cdecl StartService();
	//停止服务
	virtual bool __cdecl ConcludeService();
	virtual bool __cdecl SetDataBaseEngineSink(IUnknownEx * pIUnknownEx);
	//队列接口
public:
	//时间事件
	virtual bool __cdecl PostDataBaseTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//控制事件
	virtual bool __cdecl PostDataBaseControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool __cdecl PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

};

//////////////////////////////////////////////////////////////////////////

#endif