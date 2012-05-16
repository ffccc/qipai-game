#ifndef IDC_BANK_SERVICE_DLG
#define IDC_BANK_SERVICE_DLG

#pragma once

#include "Stdafx.h"

#include "BankStorage.h"
//////////////////////////////////////////////////////////////////////////

//登陆 TAB 控件
class CTabCtrlBank : public CTabCtrl
{
	//函数定义
public:
	//构造函数
	CTabCtrlBank();
	//析够函数
	virtual ~CTabCtrlBank();

	//消息函数
protected:
	//重画函数	
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//存款窗口
class CBankItemDlg : public CDialog
{
	//控件变量
public:
	LONG							m_lInputCount;
	CString								m_strNickname;
	CString								m_strPassword;
	int									m_nSelectType;
	CBrush								m_brBakcground;
	DWORD								m_dwUserID;

	//接口变量
protected:	
	IClientKernel					* m_pIClientKernel;				//内核接口
	ITCPSocket						* m_pClientSocket;				//网络接口
	tagUserData						*m_pMeUserData;					//用户信息

	//函数定义
public:
	//构造函数
	CBankItemDlg();
	//析构函数
	virtual ~CBankItemDlg();

	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual void OnOK() { return; };
	//取消消息
	virtual void OnCancel() { return; };

	//功能函数
public:
	//设置组件
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserDatas);
	bool CheckInput();

	//消息函数
protected:
	//重画函数
	afx_msg void OnPaint();
	//大小消息
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedTargetUser();
	afx_msg void OnBnClickedTargetUser2();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
};

//银行服务
class CBankFillDlg : public CDialog
{
public:
	CString							m_strOriginPassword;
	CString							m_strNewPassword;
	CString							m_strConfirmPassword;

	//控件变量
public:
	CBrush							m_brBakcground;

	//函数定义
public:
	//构造函数
	CBankFillDlg();
	//析构函数
	virtual ~CBankFillDlg();
	bool CheckInput();

	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
};
//////////////////////////////////////////////////////////////////////////

//银行服务
class CBankServiceDlg : public CSkinDialogEx
{
	//变量定义
protected:
	CDialog	 *							m_pActiveItem;					//激活界面

	//控件变量
public:
	CBankStorage						m_BankSavingDlg;				//存入游戏币
	CBankDrawing						m_BankDrawingDlg;				//存入游戏币
	CBankItemDlg						m_BankTransferDlg;				//转帐服务
	CBankFillDlg						m_BankChangePassDlg;			//修改密码

	CTabCtrlBank						m_TabServiceMode;				//类型选择

	//控件变量
public:
	CSkinButton							m_btOK;							//确定按钮
	CSkinButton							m_btCancel;						//取消按钮

	tagUserData						*m_pMeUserData;					//用户信息

		//接口变量
public:	
	IClientKernel					* m_pIClientKernel;				//内核接口
	ITCPSocket						* m_pClientSocket;				//网络接口

	//函数定义
public:
	//构造函数
	CBankServiceDlg();
	//析构函数
	virtual ~CBankServiceDlg();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual void OnOK();

	//消息函数
public:
	//类型改变
	afx_msg void OnTcnSelchangeServiceType(NMHDR * pNMHDR, LRESULT * pResult);

	//发送信息
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////



//兑换魅力
extern "C" PROPERTY_MODULE_CLASS VOID __cdecl ShowBankStorageDlg(IClientKernel *pIClientKernel,ITCPSocket *pClientSocket,tagUserData *pUserData);

//////////////////////////////////////////////////////////////////////////

#endif