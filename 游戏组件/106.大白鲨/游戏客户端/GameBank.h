#ifndef BANK_HEAD_FILE
#define BANK_HEAD_FILE

#pragma once
#include "Stdafx.h"
#include "Resource.h"


class CGameBank : public CSkinDialogEx
{
	//变量定义
public:
	tagUserData	const				*m_pMeUserData;					//用户信息
	WORD							m_wBankerUser;					//当前庄家
	LONG							m_lApplyBankerCondition;			//申请条件
	LONG							m_lGameGold;					//身上钱数
	LONG							m_lStorageGold;					//银行钱数
	LONG							m_lInCount;						//输入游戏币
	LONG							m_lMeJetton;					//自己下注总数
	bool							m_bMeApplyBanker;					//申请标识
	//控件变量
protected:
	CSkinButton						m_btOK;							//确定按钮
	CSkinButton						m_btCancel;						//取消按钮

	//接口变量
protected:	
	IClientKernel					* m_pIClientKernel;				//内核接口

	//基础函数
public:
	//构造函数
	CGameBank(); 
	//析构函数
	virtual ~CGameBank();
	//辅助函数
public:
	//更新界面
	void UpdateView();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange* pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定按钮
	afx_msg void OnBnClickedOk();
	//鼠标信息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//鼠标信息
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//命令信息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//时间消息
	afx_msg void OnTimer(UINT nIDEvent);
	//输入信息
	afx_msg void OnEnChangeInCount();

	//功能函数
public:
	//是否选中
	bool IsButtonSelected(UINT uButtonID);
	//选中按钮
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//设置组件
	void SetSendInfo(IClientKernel *pClientKernel,const tagUserData *pMeUserDatas,LONG lMeJetton, WORD wBankerUser, LONG lApplyBankerCondition, bool bMeApplyBanker);
	//发送信息
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};

#endif
