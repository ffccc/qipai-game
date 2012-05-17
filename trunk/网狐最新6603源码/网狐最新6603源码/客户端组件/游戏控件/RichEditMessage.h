#ifndef RICH_EDIT_MESSAGE_HEAD_FILE
#define RICH_EDIT_MESSAGE_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//颜色定义
#define COLOR_DEBUG					RGB(0,128,128)						//调试颜色
#define COLOR_EXCEPTION				RGB(200,0,0)						//异常颜色

//颜色定义
#define COLOR_WARN					RGB(255,128,0)						//警告颜色
#define COLOR_EVENT					RGB(125,125,125)					//事件消息
#define COLOR_NORMAL				RGB(0,0,0)							//普通消息
#define COLOR_SYSTEM_STRING			RGB(10,10,10)						//系统消息

//私聊颜色
#define COLOR_USER					RGB(0,0,255)						//对方颜色
#define COLOR_MYSELF				RGB(0,128,64)						//自己颜色

//位置定义
#define STRING_OFFSET				90									//上移位置

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CExpressionManager;

//字符消息
class SHARE_CONTROL_CLASS CRichEditMessage : public CSkinRichEdit, public IStringMessage
{
	//图片资源
protected:
	COLORREF						m_crFillColor;						//填充颜色
	CDataObject						m_ImageDataSystem;					//资源变量
	CDataObject						m_ImageDataPrompt;					//资源变量
	CDataObject						m_ImageDataAffiche;					//资源变量

	//组件变量
protected:
	CSkinScrollBar					m_SkinScrollBar;					//滚动条类

	//组件变量
protected:
	CExpressionManager *			m_pExpressionManager;				//表情管理
	IRichEditMessageSink *			m_pIRichEditMessageSink;			//接口变量

	//函数定义
public:
	//构造函数
	CRichEditMessage();
	//析构函数
	virtual ~CRichEditMessage();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//事件消息
public:
	//进入事件
	virtual bool InsertUserEnter(LPCTSTR pszUserName);
	//离开事件
	virtual bool InsertUserLeave(LPCTSTR pszUserName);
	//断线事件
	virtual bool InsertUserOffLine(LPCTSTR pszUserName);

	//字符消息
public:
	//普通消息
	virtual bool InsertNormalString(LPCTSTR pszString);
	//系统消息
	virtual bool InsertSystemString(LPCTSTR pszString);
	//提示消息
	virtual bool InsertPromptString(LPCTSTR pszString);
	//公告消息
	virtual bool InsertAfficheString(LPCTSTR pszString);

	//定制消息
public:
	//定制消息
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor);
	//定制消息
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor);

	//表情消息
public:
	//用户表情
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath);
	//用户表情
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString);
	//用户表情
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath);

	//聊天消息
public:
	//用户聊天
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor);
	//用户聊天
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor);
	//用户私聊
	virtual bool InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString);

	//重载函数
protected:
	//控件绑定
	virtual VOID PreSubclassWindow();

	//配置函数
public:
	//设置接口
	bool SetRichEditMessageSink(IUnknownEx * pIUnknownEx);
	//设置组件
	bool SetExpressionManager(CExpressionManager * pExpressionManager, COLORREF crFillColor);

	//功能函数
public:
	//插入时间
	bool InsertSystemTime(COLORREF crColor);
	//用户名字
	bool InsertUserAccounts(LPCTSTR pszAccounts);
	//翻译字符
	bool TranslateInsertString(LPCTSTR pszString, COLORREF crColor);

	//内部函数
protected:
	//处理图片
	bool LoadRichEditImage(HINSTANCE hInstance, UINT uResourceID, CDataObject&ImageDataObject);

	//消息映射
protected:
	//连接消息
	VOID OnEventLink(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif