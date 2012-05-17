#ifndef DLG_WHISPER_HEAD_FILE
#define DLG_WHISPER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CDlgWhisper;
class CServerViewItem;

//数组定义
typedef CWHArray<CDlgWhisper *> CDlgWhisperItemArray;					//私聊数组
typedef CWHArray<IClientUserItem *> CClientUserItemArray;				//用户数组

//////////////////////////////////////////////////////////////////////////////////

//配置结构
struct tagWhisper
{
	//数据变量
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名字

	//接口变量
	ITCPSocket *					pITCPSocket;						//网络组件
	IClientUserItem *				pIMySelfUserItem;					//自己指针
	IUserOrderParser *				pIUserOrderParser;					//等级接口
	IGameLevelParser *				pIGameLevelParser;					//等级接口
};

//////////////////////////////////////////////////////////////////////////////////

//私聊窗口
class CDlgWhisper : public CSkinDialog, public IExpressionSink
{
	//状态变量
protected:
	bool							m_bCreateFlag;						//创建标志

	//数据变量
protected:
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名字

	//接口变量
protected:
	ITCPSocket *					m_pITCPSocket;						//网络组件
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针
	IUserOrderParser *				m_pIUserOrderParser;				//等级组件
	IGameLevelParser *				m_pIGameLevelParser;				//等级接口

	//变量定义
protected:
	DWORD							m_dwConversationID;					//会话标识
	CClientUserItemArray			m_ClientUserItemArray;				//用户数组

	//控制按钮
protected:
	CSkinButton						m_btCancel;							//关闭按钮
	CSkinButton						m_btSendChat;						//发送聊天
	CSkinButton						m_btUserMenu;						//用户菜单
	CSkinButton						m_btSendControl;					//发送控制

	//聊天按钮
protected:
	CSkinButton						m_btChatHistory;					//聊天历史
	CSkinButton						m_btChatButton1;					//颜色设置
	CSkinButton						m_btChatButton2;					//表情按钮
	CSkinButton						m_btChatButton3;					//清屏按钮

	//聊天控件
protected:
	CEdit							m_ChatInput;						//聊天输入
	CRichEditMessage				m_ChatMessage;						//聊天信息

	//组件变量
protected:
	CServerViewItem *				m_pServerViewItem;					//房间接口
	CExpressionControl *			m_pExpressionControl;				//表情窗口

	//资源变量
protected:
	CEncircleBMP					m_EncircleChatInput;				//输入环绕
	CEncircleBMP					m_EncircleChatMessage;				//聊天环绕

	//函数定义
public:
	//构造函数
	CDlgWhisper();
	//析构函数
	virtual ~CDlgWhisper();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//重置函数
protected:
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);
	//表情事件
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	//配置函数
public:
	//设置接口
	VOID SetTCPSocket(ITCPSocket * pITCPSocket) { m_pITCPSocket=pITCPSocket; }
	//设置接口
	VOID SetServerViewItem(CServerViewItem * pServerViewItem) { m_pServerViewItem=pServerViewItem; }
	//设置接口
	VOID SetMySelfUserItem(IClientUserItem * pIMySelfUserItem) { m_pIMySelfUserItem=pIMySelfUserItem; }
	//设置接口
	VOID SetUserOrderParser(IUserOrderParser * pIUserOrderParser) { m_pIUserOrderParser=pIUserOrderParser; }
	//设置接口
	VOID SetGameLevelParser(IGameLevelParser * pIGameLevelParser) { m_pIGameLevelParser=pIGameLevelParser; }

	//功能函数
public:
	//废弃判断
	bool DisuseEstimate();
	//用户比较
	bool CompareUserItem(DWORD dwUserID[], WORD wUserCount);
	//聊天效验
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);
	//创建窗口
	bool CreateWhisperWnd(DWORD dwConversationID, IClientUserItem * pIClientUserItem[], WORD wUserCount, CWnd * pParentWnd);

	//事件函数
public:
	//用户进入
	VOID OnEventUserEnter(IClientUserItem * pIClientUserItem);
	//用户离开
	VOID OnEventUserLeave(IClientUserItem * pIClientUserItem);
	//用户状态
	VOID OnEventUserStatus(IClientUserItem * pIClientUserItem);
	//用户表情
	VOID OnEventExpression(IClientUserItem * pIClientUserItem, WORD wItemIndex);
	//用户私聊
	VOID OnEventWisperChat(IClientUserItem * pIClientUserItem, LPCTSTR pszString, COLORREF rcColor);

	//内部函数
protected:
	//更新标题
	VOID RectifyTitle();
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//排序函数
	VOID SortUserIDData(DWORD dwUserID[], WORD wItemCount);
	//绘画控制
	VOID DrawFunctionArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);
	//绘画用户
	VOID DrawUserInfoArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//网络命令
protected:
	//发送表情
	bool SendExpressionPacket(IClientUserItem * pIClientUserItem, WORD wItemIndex);
	//发送聊天
	bool SendWhisperChatPacket(IClientUserItem * pIClientUserItem, LPCTSTR pszChatString);
	//发送会话
	bool SendColloquyChatPacket(IClientUserItem * pIClientUserItem[], WORD wUserCount, LPCTSTR pszChatString);

	//按钮消息
protected:
	//用户菜单
	VOID OnBnClickedUserMenu();
	//发送消息
	VOID OnBnClickedSendChat();
	//发送控制
	VOID OnBnClickedSendControl();
	//聊天按钮
	VOID OnBnClickedChatButton1();
	//聊天按钮
	VOID OnBnClickedChatButton2();
	//聊天按钮
	VOID OnBnClickedChatButton3();

	//菜单命令
protected:
	//更多颜色
	VOID OnSelectMoreColor();
	//选择颜色
	VOID OnSelectChatColor(UINT nCommandID);

	//消息函数
protected:
	//销毁消息
	VOID OnNcDestroy();
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif