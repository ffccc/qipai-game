#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "GameScoreWnd.h"
#include "HuCardDlg.h"
#include "dragdropmanager.h"

//////////////////////////////////////////////////////////////////////////
//消息定义
#define IDM_TUOGUAN					(WM_USER+103)						//托管消息
#define IDM_DISTUOGUAN				(WM_USER+104)

#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_OUT_CARD				(WM_USER+101)						//出牌消息
#define IDM_HD						(WM_USER+102)						//海底消息

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameView
{
public:			
	bool					m_IsStartSendCard;

	//标志变量
protected:
	CDragDropManager        m_ddManager;

	bool					m_bOutCard;							//出牌标志
	bool					m_bWaitOther;						//等待标志
	bool					m_bHuangZhuang;						//荒庄标志

	//游戏属性
protected:
	LONG					m_lCellScore;						//基础积分
	WORD					m_wBankerUser;						//庄家用户
	//用户状态
protected:
	BYTE					m_cbCardData;						//出牌扑克
	WORD					m_wOutCardUser;						//出牌用户
	BYTE					m_cbUserAction[3];					//用户动作

	//位置变量
protected:
	CPoint					m_UserFlagPos[3];					//标志位置

	//位图变量
protected:
	CSkinImage				m_ImageBack;						//背景图案
	CSkinImage				m_ImageWait;						//等待提示
	CSkinImage				m_ImageOutCard;						//出牌提示
	CSkinImage				m_ImageUserFlag;					//用户标志
	CSkinImage				m_ImageUserAction;					//用户动作
	CSkinImage				m_ImageActionBack;					//动作背景
	CSkinImage				m_ImageHuangZhuang;					//荒庄标志
	CSkinImage				m_ImageHeadBg;						//头像背景
	
public:
	//桌面显示牌的各种数据
	//堆牌信息
	CHeapCard				m_LHeapCard;                       //仅在显示时，依据牌数目而显示牌的个数
	WORD					m_leaveHeapCardData;               //用户保存堆牌中的牌数目
	
	//每个用户组合牌信息
	CWeaveCard				m_WeaveCard[3][8];					//组合扑克
	
	//每个用户已出牌的信息
	CDiscardCard			m_DiscardCard[3];					//丢弃扑克
	
	//保存0号与2号用户手中牌的信息
	CUserCard				m_UserCard[3];						//用户扑克--另两个用户手中牌的显示
	
	//保存0号与2号用户结束之后手中牌的信息
	CTableCard				m_TableCard[3];						//桌面扑克
	
	CHandCard               m_HandCard[9];                     //保存用户手中牌在9个分区中的情况

	//扑克控件
public:
	BYTE					m_cbCurrentCard;					//当前牌
	
	//控件变量
public:

	
	CSkinButton						m_btDisTuoGuan;						//取消托管按钮
	CSkinButton						m_btTuoGuan;						//托管按钮


	CSkinButton				m_btStart;							//开始按钮

	CControlWnd				m_ControlWnd;						//操作提示窗口
	CGameScoreWnd			m_GameScoreWnd;						//结束时记分窗口
	
	CHuCardDlg              m_HuCardDlg;                       //胡牌提示窗口

	BYTE                    m_DecideBankerCard[3];             //为定庄所发的牌数
	CPoint                  m_UserDecideCardPos[3];            //放发的定庄的牌的位置
	bool                    m_bDecideBanker;                   //是否显示定庄的牌

	int						m_nAreaLeftBorder;					//所有窗口间隔左边大小
	int						m_nAreaYValue;						//区域窗口的y基点位置
	int						m_nAreaWidth;						//牌窗口区域宽度
	CPoint					m_CurAreaPoiBench;					//当前窗口的基点
	BYTE					m_cbTongCount[3];					//保存三家藏牌次数
	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//继承函数
private:
	//重置界面
	virtual void ResetGameView();
	//调整控件
	virtual void RectifyGameView(int nWidth, int nHeight);
	//绘画界面
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//功能函数
public:
	//基础积分
	void SetCellScore(LONG lCellScore);
	//庄家用户
	void SetBankerUser(WORD wBankerUser);
	//荒庄设置
	void SetHuangZhuang(bool bHuangZhuang);
	//海底设置
	void SetHD(bool bHD);
	//状态标志
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//出牌信息
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//动作信息
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//获得当前存在组合区域
	int GetCardAreaCount();
	//允许用户出牌
	void EnableOutCard();
	//禁止用户出牌
	void DisableOutCard();

	//辅助函数
protected:
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//消息映射
protected:
		//托管按钮
	afx_msg void OnDisTuoGuan();
	afx_msg void OnTuoGuan();

	//开始按钮
	afx_msg void OnStart();
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	afx_msg LRESULT OnSonMouseMove(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg LRESULT OnDragEnter(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDragDrop(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDragAbort(WPARAM wp, LPARAM lp);
};

//////////////////////////////////////////////////////////////////////////
