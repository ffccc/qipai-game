#ifndef PASSWORD_CONTROL_HEAD_FILE
#define PASSWORD_CONTROL_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//密码键盘
class SHARE_CONTROL_CLASS CPasswordKeyboard : public CDialog
{
	//配置变量
protected:
	INT								m_nRandLine[4];						//随机数字
	INT								m_nItemCount[4];					//字符数目
	TCHAR							m_szKeyboradChar[4][2][14];			//键盘字符

	//键盘状态
protected:
	bool							m_bShiftStatus;						//按键状态
	bool							m_bCapsLockStatus;					//按键状态

	//状态变量
protected:
	WORD							m_wHoverRow;						//盘旋列数
	WORD							m_wHoverLine;						//盘旋行数
	bool							m_bMouseDown;						//按下状态

	//资源大小
protected:
	CSize							m_SizeImageItem1;					//资源大小
	CSize							m_SizeImageItem2;					//资源大小
	CSize							m_SizeImageItem3;					//资源大小
	CSize							m_SizeBackGround;					//背景大小
	CSize							m_SizeImageButton;					//资源大小

	//函数定义
public:
	//构造函数
	CPasswordKeyboard();
	//析构函数
	virtual ~CPasswordKeyboard();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK() { return; }
	//取消消息
	virtual VOID OnCancel() { return; }

	//辅助函数
protected:
	//更新位置
	VOID SetCurrentStation(CPoint MousePoint);
	//虚拟编码
	WORD GetVirualKeyCode(WORD wHoverLine, WORD wHoverRow);

	//消息定义
protected:
	//界面绘画
	VOID OnNcPaint();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//失去焦点
	VOID OnKillFocus(CWnd * pNewWnd);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//密码编辑
class SHARE_CONTROL_CLASS CPasswordControl : public CWnd
{
	//友元定义
	friend class CPasswordKeyboard;

	//状态变量
protected:
	bool							m_bModify;							//修改标志
	bool							m_bFalsity;							//虚假标志
	bool							m_bDrawBorad;						//绘画边框
	bool							m_bRenderImage;						//渲染标志
	TCHAR							m_szPassword[LEN_PASSWORD];			//用户密码

	//控件变量
protected:
	CSkinEditEx						m_edPassword;						//密码控件
	CSkinButton						m_btKeyboard;						//键盘按钮
	CPasswordKeyboard				m_PasswordKeyboard;					//密码键盘

	//函数定义
public:
	//构造函数
	CPasswordControl();
	//析构函数
	virtual ~CPasswordControl();

	//重载函数
protected:
	//绑定函数
	virtual VOID PreSubclassWindow();

	//状态函数
public:
	//修改状态
	bool IsModifyPassword() { return m_bModify; }
	//虚假状态
	bool IsFalsityPassword() { return m_bFalsity; }

	//功能函数
public:
	//边框设置
	VOID SetDrawBorad(bool bDrawBorad);
	//设置密码
	VOID SetUserPassword(LPCTSTR pszPassword);
	//获取密码
	LPCTSTR GetUserPassword(TCHAR szPassword[LEN_PASSWORD]);

	//消息定义
protected:
	//重画消息
	VOID OnPaint();
	//重画消息
	VOID OnNcPaint();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//获取焦点
	VOID OnSetFocus(CWnd * pOldWnd);

	//控件消息
protected:
	//密码改变
	VOID OnEnChangePassword();
	//键盘按钮
	VOID OnBnClickedKeyboard();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif