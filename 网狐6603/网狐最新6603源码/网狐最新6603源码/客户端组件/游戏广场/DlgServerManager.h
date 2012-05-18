#ifndef DLG_SERVER_MANAGER_HEAD_FILE
#define DLG_SERVER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//房间管理
class CDlgServerManager : public CSkinDialog
{
	//资源变量
protected:
	CPen							m_BoradPen;							//资源变量
	CBrush							m_FrameBrush;						//资源变量

	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针

	//功能按钮
protected:
	CSkinButton			 			m_btOk;								//存储退出
	CSkinButton			 			m_btCancel;							//放弃按钮

	//列表配置
protected:
	CSkinEdit						m_edKindID;							//编辑控件
	CSkinEdit						m_edNodeID;							//编辑控件
	CSkinEdit						m_edSortID;							//编辑控件

	//房间配置
protected:
	CSkinEdit						m_edServerName;						//编辑控件
	CSkinEdit						m_edRevenueRatio;					//编辑控件
	CSkinEdit						m_edRestrictScore;					//编辑控件

	//条件限制
protected:
	CSkinEdit						m_edMinEnterScore;					//编辑控件
	CSkinEdit						m_edMaxEnterScore;					//编辑控件
	CSkinEdit						m_edMinTableScore;					//编辑控件
	CSkinComboBox					m_comMinEnterMember;				//编辑控件
	CSkinComboBox					m_comMaxEnterMember;				//编辑控件

	//函数定义
public:
	//构造函数
	CDlgServerManager();
	//析构函数
	virtual ~CDlgServerManager();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//重载函数
protected:
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//功能函数
public:
	//配置管理
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//设置配置
	VOID SetServerOptionInfo(tagServerOptionInfo & ServerOptionInfo, DWORD dwRuleMask);

	//消息映射
protected:
	//控件颜色
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif