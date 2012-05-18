#ifndef USER_ITEM_ELEMENT_HEAD_FILE
#define USER_ITEM_ELEMENT_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//接口说明
interface IUserOrderParser;
interface IFaceItemControl;

//////////////////////////////////////////////////////////////////////////////////

//密码等级
#define PASSWORD_LEVEL_0			0									//没有等级
#define PASSWORD_LEVEL_1			1									//没有等级
#define PASSWORD_LEVEL_2			2									//没有等级
#define PASSWORD_LEVEL_3			3									//没有等级

//////////////////////////////////////////////////////////////////////////////////

//用户元素
class SHARE_CONTROL_CLASS CUserItemElement
{
	//配置变量
protected:
	IUserOrderParser *				m_pIUserOrderParser;				//等级组件
	IFaceItemControl *				m_pIFaceItemControl;				//头像组件

	//静态变量
protected:
	static CUserItemElement *		m_pUserItemElement;					//对象指针

	//函数定义
public:
	//构造函数
	CUserItemElement();
	//析构函数
	virtual ~CUserItemElement();

	//配置函数
public:
	//设置接口
	VOID SetUserOrderParser(IUserOrderParser * pIUserOrderParser);
	//设置接口
	VOID SetFaceItemControl(IFaceItemControl * pIFaceItemControl);

	//功能函数
public:
	//绘画经验
	VOID DrawExperience(CDC * pDC, INT nXPos, INT nYPos, DWORD dwExperience);
	//构造名片
	VOID ConstructNameCard(IClientUserItem * pIClientUserItem, IGameLevelParser * pIGameLevelParser, CImage&ImageNameCard);

	//等级函数
public:
	//密码等级
	BYTE GetPasswordLevel(LPCTSTR pszPassword);
	//获取等级
	WORD GetExperienceLevel(DWORD dwExperience);
	//等级信息
	WORD GetExperienceIndex(WORD wUserLevel, BYTE cbOrderIndex[], WORD wMaxCount);

	//合法效验
public:
	//效验帐号
	bool EfficacyAccounts(LPCTSTR pszAccounts, TCHAR szDescribe[], WORD wMaxCount);
	//效验昵称
	bool EfficacyNickName(LPCTSTR pszNickName, TCHAR szDescribe[], WORD wMaxCount);
	//效验密码
	bool EfficacyPassword(LPCTSTR pszPassword, TCHAR szDescribe[], WORD wMaxCount);
	//效验证件
	bool EfficacyPassPortID(LPCTSTR pszPassPortID, TCHAR szDescribe[], WORD wMaxCount);

	//静态函数
public:
	//获取对象
	static CUserItemElement * GetInstance() { return m_pUserItemElement; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif