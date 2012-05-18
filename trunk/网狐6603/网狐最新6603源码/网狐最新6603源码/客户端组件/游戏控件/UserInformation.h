#ifndef USER_INFORMATION_HEAD_FILE
#define USER_INFORMATION_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//类说明
typedef CWHArray<tagCompanionInfo *> CCompanionInfoArray;				//用户关系

//////////////////////////////////////////////////////////////////////////////////

//用户信息
class SHARE_CONTROL_CLASS CUserInformation : public IUserInformation
{
	//接口变量
protected:
	IUserCompanionSink *			m_pIUserCompanionSink;				//关系回调

	//关系信息
protected:
	CCompanionInfoArray				m_CompanionInfoActive;				//激活子项
	CCompanionInfoArray				m_CompanionInfoBuffer;				//释放子项

	//静态变量
protected:
	static CUserInformation *		m_pUserInformation;					//关系接口

	//函数定义
public:
	//构造函数
	CUserInformation();
	//析构函数
	virtual ~CUserInformation();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//加载信息
	virtual bool LoadInformation();
	//设置接口
	virtual bool SetUserCompanionSink(IUnknownEx * pIUnknownEx);

	//关系接口
public:
	//枚举关系
	virtual tagCompanionInfo * EmunCompanionInfo(WORD wIndex);
	//寻找关系
	virtual tagCompanionInfo * SearchCompanionInfo(DWORD dwUserID);
	//保存关系
	virtual tagCompanionInfo * InsertCompanionInfo(IClientUserItem * pIClientUserItem, BYTE cbCompanion);

	//内部函数
private:
	//创建关系
	tagCompanionInfo * CreateCompanionItem();
	//释放关系
	bool DeleteCompanionItem(tagCompanionInfo * pCompanionInfo);

	//静态函数
public:
	//获取实例
	static CUserInformation * GetInstance() { return m_pUserInformation; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif