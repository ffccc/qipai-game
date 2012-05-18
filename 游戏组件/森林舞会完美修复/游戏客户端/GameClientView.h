#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

#include "RecordGameList.h"
#include ".\appuserlist.h"
#include "RecordLZ.h"
#include <time.h>
#include "..\..\..\qplib\include\ui\TimeCtrlSprite.h"
///////////////////

#define JETTON_TYPE_COUNT			 3									//筹码数目
  #define       HISTORY_RECORD_SHOW 10
//数值掩码
#define	CARD_MASK_COLOR				0xF0								//花色掩码
#define	CARD_MASK_VALUE				0x0F								//数值掩码
#define INVALID_CARD				0xff								//无效扑克

//筹码对应的金额定义
const LONG JETTON_AMOUNT[JETTON_TYPE_COUNT] =
{
	 1,  5, 10
};


//////////出左右牌
enum
{
	OUT_LEFT_CARD=1,
	OUT_RIGHT_CARD=2,        
};


//////////////////////////////////////////////////////////////////////////
//结构定义


//筹码信息
struct tagJettonInfo
{
	int								nXPos;								//筹码位置
	int								nYPos;								//筹码位置
	BYTE							cbJettonIndex;						//筹码索引
};

//筹码数组
typedef CArrayTemplate<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView: public CGameFrameView, public CGameWnd
{
	//下注信息
public:
	LONG							m_lMyMaxStake;						//最大下注
	LONG							m_lAllMaxScore[INDEX_MAX];			//每个区域的最大下注额
	LONG							m_lMyStakes[INDEX_MAX];				//自己的的下注金额
	LONG							m_lAllStakes[INDEX_MAX];			//只按下注位置统计的下注金额
	LONG							m_Prizemoney  ;			            //彩金
	LONG							m_lmaxStakes[INDEX_MAX];			//每个区域的可下注额
	LONG							m_lBankerMaxLoss;					//庄家最大赔付
	LONG							m_lBankers;					        //庄家分数
	LONG							m_lBankerwin;					    //庄家成绩
	LONG							m_lMywin;					        //玩家分数
	WORD							m_wMyChairID;						//我的位置
	bool                            m_endsrok;                          //结算标志
    bool                            m_endsrok2;                         //结算标志
	int                             m_timenum;
	bool                            m_timesound;
	bool                            m_move;
    CPoint                          m_lzPoint;
	bool                            m_bDrawWinArea;
	int								m_pZPIndex;							// 转盘现在所在位置
	BOOL                            m_iSMyWin;                          //是否显示自已的分数
	LONG                            m_wBankerScore;

	LONG                           m_MySlof;                           //玩家分数

	long							m_wMyWinCount;						//赢的局数


	//庄家信息
protected:
	SURFACE*						m_pImageShowBanker;					//轮换庄家
	WORD							m_wBankerCount;						//做庄次数
	LONG							m_lBankerWinScore;					//庄家成绩


	//状态变量
protected:

	DWORD							m_dwWinnerFlash;					//胜利玩家
	INT								m_nWinnerFlashIndex;				//胜利动画帧
	LONG							m_lCurrentJetton;					//当前筹码
	BYTE							m_cbPlayerValue;					//闲家牌点
	BYTE							m_cbBankerValue;					//庄家牌点
	CString							m_strGameHint;						//游戏提示信息
    CPoint                          m_showXy[24];
	int                             m_showPrize;
	bool                            m_showPrizeBZ;

   
	CPoint                          m_time;                          //时间坐标
	//数据变量
public:
	CRect							m_RectJetton[INDEX_MAX];			//矩形筹码位置
	CJettonInfoArray				m_JettonInfoArray[INDEX_MAX];		//筹码数组


    int                             m_pTipsX;


	//控件变量
	BYTE                             m_showT[24];   
	int                              m_showTX[24];  
	BYTE                             m_showPL[INDEX_MAX]; 

	int                              m_showrobst[24];  
	bool                            m_showForT;
  	bool                            vBankerShow;
	CGameButton						m_btnJettons[JETTON_TYPE_COUNT];	//筹码按钮数组
	CGameButton						m_pImagefang;					    //取分
	CAppUserList					m_AppList;
	CGameRecord						m_GameRecord;						//记录列表
	CGameButton						m_btnApplyBanker;					//申请庄家
	CGameButton						m_btnCancelBanker;					//取消庄家
	CGameButton						m_infoup;					        //记录
	CGameButton						m_infodown;					        //记录
	CGameButton						m_btnsupper[8];					    //庄家下翻
	CGameButton						m_pGameB;					    //购买游戏币
	CGameButton						m_pGame50;					    //购买游戏币
	CGameButton						m_pGame100;					    //购买游戏币
	CGameButton						m_pGame500;					    //购买游戏币
	CGameButton						m_pGame1000;					    //购买游戏币
	CGameButton						m_pGameClose;					    //购买游戏币
	CGameButton						m_pSettlement;					    //结算
	CGameButton                     m_BankerShow;

    CPoint                          m_pGameBZ;
	LONG                            m_MyGame;                           //自已的游戏币
	bool                            m_supper;                           //是否超级
	bool                            m_GameBi;
	BYTE                            m_pYP;
	WORD							m_wCurrentBanker;					//当前庄家
	CGameLogic						m_GameLogic;						//游戏逻辑
	BYTE							m_cbCardCount;			        	//扑克数目
	BYTE                            m_Card;
	CWnd*							m_pWinCallbak;						//消息回调接口
	BYTE                            m_timezt;

	bool                            m_endsrok3;
	//界面资源相关
protected:
	CString							m_strResPath;						//资源文件的路径

	SURFACE*                        m_Bankerbg;	                        //庄家.psd
	SURFACE*						m_pImageViewBack;					//背景位图
 	SURFACE*						m_pImageStakeAmountTotal2;			//总下注额位图2
	SURFACE*						m_pImageStakeAmountMine2;			//自己下注额位图
 	SURFACE*					    m_pImageHistoryWin;					//赢的数字位图
	SURFACE*						m_pImageTimer;                      //数字时间
	SURFACE*					    m_pImageChangeBanker;				//轮换庄家
	SURFACE*					    m_pImageHistory;					//历史记录图
	HCURSOR							m_JettonCursors;	//鼠标形状
	SURFACE*						m_pImageTimerbg;                      //数字时间
	SURFACE*                         	m_pImageplay;
	SURFACE*                         	m_pImagesend;
	SURFACE*					    m_pImageNoBanker;					//无庄家
	SURFACE*					    m_pImageMeBanker;					//自己当庄
	SURFACE*                        m_pImagekuan;
	SURFACE*						m_show[12];						// 转盘图片
	SURFACE*						m_WinTips;   					// 中彩金提示
	SURFACE*						m_WinTitle;						// 中彩金题
	SURFACE*						m_WinPrize;						// 彩灯
	SURFACE*						m_Winbg;						// 中彩背景
	SURFACE*					    m_pImageBankbg;					//房间信息
	SURFACE*	                 	m_kuang;

	int								time;
public:

	BOOL							g_Tips;			
	BOOL							g_Title;		//彩金动画
    BOOL                            ISDrawlight;
	LONG                            cdPrize;

private:
	UINT IDD;
	CWinThread *pShakeThread;
    //函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();
    void StartHyMovie();
	void Create(RECT rc, CGameWnd *pWnd);
	void Animation(bool bClose);
	void Drawlight(SURFACE* psurfTarget,BYTE x);
    void TimerFLASH();
    void TipsFLASH();
    void StartWin();
    void CloseWin();
	//继承函数
private:
	//重置界面
	virtual void ResetGameView();
	//调整控件
	virtual void RectifyGameView(int nWidth, int nHeight);
	//调整位置
   char* DrawNUM(INT64 num);
   string s1;
	//virtual void OnSize();
	//绘画界面
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//功能函数
public:
	//设置最大下注额
	void SetMyMaxScore(LONG lMyMaxStake);

	void KillTimerFLASH();
	
	//设置每个区域的最大下注额
	void SetAllMaxScore(const INT64 lMaxScore[]);

	//设置筹码
	void SetCurrentJetton(LONG lCurrentJetton);

	//设置自己所有区域的下注信息
	void SetMyStake(const INT64 plMyStakes[]);

	//设置自己单个区域的下注信息
	void SetMyStake(BYTE cbBetArea, LONG lBetAmount);

	//开启游戏以来的累计数据
	void SetHistoryScore(WORD wWinCount, WORD wLossCount, INT64 lMyWinAmountSum);
	
	//庄家信息
	void SetBankerInfo( WORD wChairID, WORD wBankerCount, LONG lBankerWinScore ,LONG lBankerScore );

	//显示开奖结果
	void ShowWinnerFlash(SURFACE* purSource, int nWidth, int nHeight) ;

	void DrawNumber(SURFACE* psurfTarget,int nTime,int x,int y,bool i);

	void updateYazhumoney(SURFACE* psurfTarget,int nXPos,int nYPos);

    void Drawumoney(SURFACE* psurfTarget,int g_money,int nX,int nXP,int nYP);
	//界面函数
public:

	//清理筹码
	void CleanUserJetton();
	
	//设置多个区域的筹码
	void PlaceUserJetton(const INT64 plBetAmounts[]);

	//设置单个区域的筹码
	void PlaceUserJetton(BYTE cbBetArea, LONG lBetAmount,int cbchar);

	//设置胜方
	void SetWinner(BYTE cbPlayerValue, BYTE cbBankerValue, DWORD dwWinner);

	void SetWinAmount(LONG lWinAmount);

	void SetGameHint(LPCTSTR pszFormat, ...);

	//内联函数
public:
	//当前筹码
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }
	//我的位置
	inline void SetMyChairID(WORD wMyChairID) { m_wMyChairID=wMyChairID; }

	virtual void SetUserLeft(BOOL bShow){};

	virtual void SetLoadFinish(){};

	void DrawUserTimer(SURFACE* psurfTarget, int xPos, int yPos, int Timer);

    void InsertSystemString(LPCTSTR pszString, ...);

	virtual void OnDraw(SURFACE* psurfTarget);

	bool OnCreate();

	//内部函数
private:
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);

	void LoadRes();

	//按钮消息
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//筹码按钮点击事件
	LRESULT OnButtonClick(WPARAM wParam, LPARAM lParam);

    static UINT ShowAnimalFunction( LPVOID pParam); //动画线程

	//消息映射
protected:
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//鼠标消息
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//上庄按钮
	afx_msg void OnApplyBanker();

	//下庄按钮
	afx_msg void OnCancelBanker();

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////

#endif