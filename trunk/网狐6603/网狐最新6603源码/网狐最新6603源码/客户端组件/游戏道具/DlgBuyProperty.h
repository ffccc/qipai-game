#ifndef DLG_BUY_PROPERTY_HEAD_FILE
#define DLG_BUY_PROPERTY_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CGamePropertyItem;

//////////////////////////////////////////////////////////////////////////////////

//道具购买
class CDlgBuyProperty : public CSkinDialog
{
	//友元定义
	friend class CGamePropertyItem;

	//功能按钮
protected:
	CSkinButton			 			m_btOk;								//确定按钮
	CSkinButton			 			m_btCancel;							//放弃按钮

	//控件变量
protected:
	CSkinEdit						m_edNickName;						//用户昵称
	CSkinEdit						m_edPropertyCount;					//道具数目

	//接口变量
protected:
	CGamePropertyItem *				m_pGamePropertyItem;				//道具对象
	IGamePropertySink *				m_pIGamePropertySink;				//回调接口

	//静态变量
protected:
	static CDlgBuyProperty *		m_pDlgBuyProperty;					//对象指针

	//函数定义
public:
	//构造函数
	CDlgBuyProperty();
	//析构函数
	virtual ~CDlgBuyProperty();

	//重置函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//重置函数
protected:
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//功能函数
protected:
	//更新状态
	VOID UpdateCtrlStatus();

	//静态函数
protected:
	//显示窗口
	static VOID ShowBuyPropertyWnd(CGamePropertyItem * pGamePropertyItem, IGamePropertySink * pIGamePropertySink);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif