#ifndef USER_ORDER_PARSER_HEAD_FILE
#define USER_ORDER_PARSER_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "UserServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//索引说明
typedef CMap<BYTE,BYTE,CString,CString &> CMapMemberDescribe;			//描述索引
typedef CMap<BYTE,BYTE,CString,CString &> CMapMasterDescribe;			//描述索引

//会员解释
class USER_SERVICE_CLASS CUserOrderParser : public IUserOrderParser
{
	//变量定义
protected:
	CMapMemberDescribe				m_MapMemberDescribe;				//会员描述
	CMapMasterDescribe				m_MapMasterDescribe;				//管理描述

	//函数定义
public:
	//构造函数
	CUserOrderParser();
	//析构函数
	virtual ~CUserOrderParser();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//解释数目
	virtual BYTE GetMemberParserItemCount();
	//解释数目
	virtual BYTE GetMasterParserItemCount();
	//设置解释
	virtual VOID SetMemberParser(tagMemberItem MemberItem[], WORD wItemCount);
	//设置解释
	virtual VOID SetMasterParser(tagMasterItem MasterItem[], WORD wItemCount);

	//功能接口
public:
	//获取描述
	virtual LPCTSTR GetMemberDescribe(BYTE cbMemberOrder);
	//获取描述
	virtual LPCTSTR GetMasterDescribe(BYTE cbMasterOrder);
	//获取解释
	virtual BYTE GetMemberParser(tagMemberItem MemberItem[], WORD wItemCount);
	//获取解释
	virtual BYTE GetMasterParser(tagMasterItem MasterItem[], WORD wItemCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif