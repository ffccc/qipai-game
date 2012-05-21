#ifndef BANKER_STORAGE_HEAD_FILE
#define BANKER_STORAGE_HEAD_FILE

#include "PropertyModule.h"

#pragma once

//////////////////////////////////////////////////////////////////////////

//兑换游戏币 
class PROPERTY_MODULE_CLASS CBankStorage : public CDialog
{
	//变量定义
public:
	tagUserData						*m_pMeUserData;					//用户信息
	LONG						m_lGameGold;					//银行数目
	LONG						m_lStorageGold;					//存储数目
	LONG						m_lInCount;						//输入游戏币
	CBrush							m_brBakcground;

	CString							m_strGameGold;					//银行数目
	CString							m_strStorageGold;				//存储数目

	//控件变量
protected:
	CSkinButton						m_btOK;							//确定按钮
	CSkinButton						m_btCancel;						//取消按钮

	//接口变量
protected:	
	IClientKernel					* m_pIClientKernel;				//内核接口
	ITCPSocket						* m_pClientSocket;				//网络接口

	//基础函数
public:
	//构造函数
	CBankStorage(CWnd* pParent = NULL); 
	//析构函数
	virtual ~CBankStorage();

	//辅助函数
public:
	//更新界面
	void UpdateView();

	//重载函数
public:
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
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserDatas);
	//发送信息
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
//////////////////////////////////////////////////////////////////////////

//兑换游戏币 
class PROPERTY_MODULE_CLASS CBankDrawing : public CDialog
{
	//变量定义
public:
	tagUserData						*m_pMeUserData;					//用户信息
	LONG						m_lGameGold;					//银行数目
	LONG						m_lStorageGold;					//存储数目
	LONG						m_lInCount;						//输入游戏币
	CBrush							m_brBakcground;

	CString							m_strGameGold;					//银行数目
	CString							m_strStorageGold;				//存储数目

	//控件变量
protected:
	CSkinButton						m_btOK;							//确定按钮
	CSkinButton						m_btCancel;						//取消按钮

	//接口变量
protected:	
	IClientKernel					* m_pIClientKernel;				//内核接口
	ITCPSocket						* m_pClientSocket;				//网络接口

	//基础函数
public:
	//构造函数
	CBankDrawing(CWnd* pParent = NULL); 
	//析构函数
	virtual ~CBankDrawing();

	//辅助函数
public:
	//更新界面
	void UpdateView();
	void Process();

	//重载函数
public:
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
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserDatas);
	//发送信息
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

void MakeString(CString &strNum,LONG lNumber);
void MakeStringToNum(CString str,LONG &Num);
//////////////////////////////////////////////////////////////////////////
#endif