#include "StdAfx.h"
#include "UserOrderParser.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CUserOrderParser::CUserOrderParser()
{
	//设置质数
	m_MapMemberDescribe.InitHashTable(23);
	m_MapMasterDescribe.InitHashTable(23);

	//默认设置
	m_MapMemberDescribe[1]=TEXT("普通会员");
	m_MapMemberDescribe[2]=TEXT("中级会员");
	m_MapMemberDescribe[3]=TEXT("高级会员");

	//默认设置
	m_MapMasterDescribe[1]=TEXT("游戏管理员");
	m_MapMasterDescribe[2]=TEXT("中级管理员");
	m_MapMasterDescribe[3]=TEXT("高级管理员");

	return;
}

//析构函数
CUserOrderParser::~CUserOrderParser()
{
}

//接口查询
VOID * CUserOrderParser::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserOrderParser,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IUserOrderParser,Guid,dwQueryVer);
	return NULL;
}

//解释数目
BYTE CUserOrderParser::GetMemberParserItemCount()
{
	return (BYTE)m_MapMemberDescribe.GetCount();
}

//解释数目
BYTE CUserOrderParser::GetMasterParserItemCount()
{
	return (BYTE)m_MapMasterDescribe.GetCount();
}

//设置解释
VOID CUserOrderParser::SetMemberParser(tagMemberItem MemberItem[], WORD wItemCount)
{
	//清理数据
	m_MapMemberDescribe.RemoveAll();

	//设置解释
	for (WORD i=0;i<wItemCount;i++) 
	{
		LPCTSTR pszMemberName(MemberItem[i].szMemberName);
		m_MapMemberDescribe[MemberItem[i].cbMemberOrder]=pszMemberName;
	}

	return;
}

//设置解释
VOID CUserOrderParser::SetMasterParser(tagMasterItem MasterItem[], WORD wItemCount)
{
	//清理数据
	m_MapMasterDescribe.RemoveAll();

	//设置解释
	for (WORD i=0;i<wItemCount;i++) 
	{
		LPCTSTR pszMasterName(MasterItem[i].szMasterName);
		m_MapMasterDescribe[MasterItem[i].cbMasterOrder]=pszMasterName;
	}

	return;
}

//获取描述
LPCTSTR CUserOrderParser::GetMemberDescribe(BYTE cbMemberOrder)
{
	CMapMemberDescribe::CPair * pPair=m_MapMemberDescribe.PLookup(cbMemberOrder);
	return (pPair!=NULL)?((LPCTSTR)pPair->value):TEXT("");
}

//获取描述
LPCTSTR CUserOrderParser::GetMasterDescribe(BYTE cbMasterOrder)
{
	CMapMasterDescribe::CPair * pPair=m_MapMasterDescribe.PLookup(cbMasterOrder);
	return (pPair!=NULL)?((LPCTSTR)pPair->value):TEXT("");
}

//获取解释
BYTE CUserOrderParser::GetMemberParser(tagMemberItem MemberItem[], WORD wItemCount)
{
	//变量定义
	BYTE cbCopyCount=0;
	CString strMemberName;
	POSITION Position=m_MapMemberDescribe.GetStartPosition();

	//设置变量
	while (Position!=NULL)
	{
		//完成判断
		if (cbCopyCount>=wItemCount) break;

		//获取子项
		BYTE cbMemberOrder=0;
		m_MapMemberDescribe.GetNextAssoc(Position,cbMemberOrder,strMemberName);

		//设置解释
		BYTE cbIndex=cbCopyCount++;
		MemberItem[cbIndex].cbMemberOrder=cbMemberOrder;
		lstrcpyn(MemberItem[cbIndex].szMemberName,strMemberName,CountArray(MemberItem[cbIndex].szMemberName));
	}

	return cbCopyCount;
}

//获取解释
BYTE CUserOrderParser::GetMasterParser(tagMasterItem MasterItem[], WORD wItemCount)
{
	//变量定义
	BYTE cbCopyCount=0;
	CString strMasterName;
	POSITION Position=m_MapMasterDescribe.GetStartPosition();

	//设置变量
	while (Position!=NULL)
	{
		//完成判断
		if (cbCopyCount>=wItemCount) break;

		//获取子项
		BYTE cbMasterOrder=0;
		m_MapMasterDescribe.GetNextAssoc(Position,cbMasterOrder,strMasterName);

		//设置解释
		BYTE cbIndex=cbCopyCount++;
		MasterItem[cbIndex].cbMasterOrder=cbMasterOrder;
		lstrcpyn(MasterItem[cbIndex].szMasterName,strMasterName,CountArray(MasterItem[cbIndex].szMasterName));
	}

	return cbCopyCount;
}

//////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(UserOrderParser);

//////////////////////////////////////////////////////////////////////////
