#ifndef DLG_TABLE_PASSWORD_HEAD_FILE
#define DLG_TABLE_PASSWORD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//桌子密码
class CDlgTablePassword : public CSkinDialog
{
	//变量定义
public:
	TCHAR								m_szString[256];				//提示信息
	TCHAR								m_szPassword[LEN_PASSWORD];		//桌子密码

	//控件变量
protected:
	CSkinButton							m_btOk;							//确定按钮
	CSkinButton							m_btCancel;						//取消按钮

	//编辑控件
protected:
	CStatic								m_stString;						//提示控件
	CSkinEdit							m_edPassword;					//密码控件

	//函数定义
public:
	//构造函数
	CDlgTablePassword();
	//析构函数
	virtual ~CDlgTablePassword();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//功能函数
public:
	//设置提示
	VOID SetPromptString(LPCTSTR pszString);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif