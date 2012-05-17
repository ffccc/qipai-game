#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//游戏配置
class CGameScoreView : public CSkinDialogEx
{


	//控件变量
public:
	CSkinButton						m_btOK;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮

	//函数定义
public:
	//构造函数
	CGameScoreView();
	//析构函数
	virtual ~CGameScoreView();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
public:
	void SetScoreText(BYTE bTextIndex, const TCHAR *strName, long lScore);
	void SetWinnerText(const TCHAR *strName);
	afx_msg void OnPaint();
};

//////////////////////////////////////////////////////////////////////////
