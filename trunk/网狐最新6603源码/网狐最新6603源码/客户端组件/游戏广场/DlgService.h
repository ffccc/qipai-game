#ifndef DLG_SERVICE_HEAD_FILE
#define DLG_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgServiceItem.h"
#include "DlgServicePassword.h"
#include "DlgServiceIndividual.h"

//////////////////////////////////////////////////////////////////////////////////

//用户服务
#define SERVICE_ACCOUNTS			1									//修改资料
#define SERVICE_INDIVIDUAL			2									//个人资料
#define SERVICE_PASSWORD			3									//修改密码
#define SERVICE_PASSPROTECT			4									//密码保护
#define SERVICE_SECURITY			5									//安全设置
#define SERVICE_MEMBER_PAYMENT		6									//会员充值

//会员服务
#define SERVICE_GAME_RECORD			10									//游戏记录
#define SERVICE_PAY_RECORD			11									//充值记录
#define SERVICE_CLEAN_FLEE			12									//清逃跑率
#define SERVICE_CLEAN_SCORE			13									//负分清零
#define SERVICE_MEMBER_INFO			14									//会员介绍

//////////////////////////////////////////////////////////////////////////////////

//数组说明
typedef CWHArray<CDlgServiceItem *> CDlgServiceItemArray;

//////////////////////////////////////////////////////////////////////////////////

//选择控制
class CDlgServiceBar : public CWnd
{
	//活动信息
protected:
	bool							m_bHovering;						//盘旋标志
	WORD							m_wItemDown;						//按下子项
	WORD							m_wItemHover;						//盘旋子项
	WORD							m_wItemActive;						//激活子项

	//子项信息
protected:
	CSize							m_SizeButton;						//按钮大小
	CRect							m_rcItemCreate;						//创建位置
	CDlgServiceItemArray			m_DlgServiceItemArray;				//控制数组

	//函数定义
public:
	//构造函数
	CDlgServiceBar();
	//析构函数
	virtual ~CDlgServiceBar();

	//重载函数
public:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//功能函数
public:
	//设置区域
	bool SetItemCreateRect(CRect rcItemCreate);
	//插入子项
	bool InsertServiceItem(CDlgServiceItem * pDlgServiceItem);
	//激活设置
	bool SetActiveServiceItem(CDlgServiceItem * pDlgServiceItem);

	//内部函数
private:
	//索引切换
	WORD SwitchToButtonIndex(CPoint MousePoint);

	//消息函数
protected:
	//重画消息
	VOID OnPaint();
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//服务专区
class CDlgService : public CSkinDialog
{
	//服务变量
protected:
	BYTE							m_cbServiceID;						//期望标识

	//控件变量
protected:
	CDlgServiceBar					m_DlgServiceBar;					//子项按钮

	//网页服务
public:
	CDlgServiceBrowser				m_DlgServiceBrowser1;				//网页连接
	CDlgServiceBrowser				m_DlgServiceBrowser2;				//网页连接
	CDlgServiceBrowser				m_DlgServiceBrowser3;				//网页连接
	CDlgServiceBrowser				m_DlgServiceBrowser4;				//网页连接
	CDlgServiceBrowser				m_DlgServiceBrowser5;				//网页连接
	CDlgServiceBrowser				m_DlgServiceBrowser6;				//网页连接

	//程序服务
public:
	CDlgServicePassword				m_DlgServicePassword;				//修改密码
	CDlgServiceIndividual			m_DlgServiceIndividual;				//基本资料

	//函数定义
public:
	//构造函数
	CDlgService();
	//析构函数
	virtual ~CDlgService();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual VOID OnOK();

	//功能函数
public:
	//期望标识
	VOID SetExpectationID(BYTE cbServiceID);
	//获取子项
	CDlgServiceItem * GetServiceItem(BYTE cbServiceID);

	//消息映射
protected:
	//列表消息
	VOID OnActionServiceUser(NMHDR * pNMHDR, LRESULT * pResult);
	//列表消息
	VOID OnActionServiceMember(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif