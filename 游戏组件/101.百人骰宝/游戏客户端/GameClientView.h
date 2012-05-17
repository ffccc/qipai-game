#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../消息定义/GameLogic.h"
#include "TransparentCtrl.h"
#include "SkinButtonEx.h"
#include "afxtempl.h"
//////////////////////////////////////////////////////////////////////////

//筹码定义
#define JETTON_COUNT				9									//筹码数目
#define JETTON_RADII				68									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			(WM_USER + 200)							// 加住信息
#define IDM_APPLY_BANKER			(WM_USER + 201)							// 申请信息
#define IDM_CHANGE_SICBO_TYPE       (WM_USER + 202)                         // 摇骰子方式
#define IDM_OPEN_BANK               (WM_USER + 203)                         // 银行操作
#define IDM_USE_WINNER              (WM_USER + 204)                    // 玩家控制

#define SICBO_ANIM_FRAME_COUNT      18
//////////////////////////////////////////////////////////////////////////

//结构定义

//记录信息
struct tagClientGameRecord
{
	bool					bFlags;						// 是开大还是开小
	int                     iCards[MAX_COUNT_SICBO];
};

//筹码信息
struct tagJettonInfo
{
	int								nXPos;								//筹码位置
	int								nYPos;								//筹码位置
	BYTE							cbJettonIndex;						//筹码索引
};

// 正常时，骰子的动画轨迹
struct tagSicboAnim
{
	BYTE byResultX;  // 骰子的点数(点数确定了静态时取X列)
	BYTE byResultY;  // 静止状态时取Y行
	BYTE byAnimX;   // 运动过程中最后一个骰子取x列
	BYTE byAnimY;   // 运动过程中最后一个骰子取y行
	CPoint ptEnd;   // 最后停止的坐标
};

struct tagApplyUser
{
	//玩家信息
	CString							strUserName;						//玩家帐号
	__int64							lUserScore;							//玩家金币
};


//筹码数组
typedef CArrayTemplate<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//累声明
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameView
{
	//限制信息
protected:
	__int64							m_lMeMaxScore;						// 最大下注
	__int64                         m_lAllMaxScore;                     // 当局可以下注的总注
    __int64							m_lAreaLimitScore;					//区域限制

	//下注信息
protected:
	__int64							m_lMeScore[COUNT_AZIMUTH];	        //每个方位总注

	//全体下注
protected:
	__int64							m_lAllScore[COUNT_AZIMUTH];			//每个方位总注

	//位置信息
protected:
	int								m_nWinFlagsExcursionX;				//偏移位置
	int								m_nWinFlagsExcursionY;				//偏移位置
	CPoint                          m_ptApplyListPos;
	int								m_nScoreHead;						//成绩位置
	CRect							m_rcUser[COUNT_AZIMUTH];			//玩家下注区域

	//扑克信息
protected:	
	E_SICBO_NUMBER					m_enCards[MAX_COUNT_SICBO];         // 骰子点数
	bool					        m_bWinner[COUNT_AZIMUTH];           // 输赢结果

	//动画变量
protected:
	CPoint                          m_ptSicboAnim[SICBOTYPE_COUNT];         // 骰子动画坐标(每种摇骰子的方式坐标不同)
	tagSicboAnim                    m_SicboAnimInfo[MAX_COUNT_SICBO];       // 正常、与空中摇骰子的动画信息
	BYTE                            m_bySicboDraw[MAX_COUNT_SICBO];         // 画骰子的顺序
	bool                            m_bSicboShow;                           // 动画完成后显示结果
	bool							m_bSicboEffect;		                    // 摇骰子效果
	BYTE                            m_bySicboFrameIndex;                    // 当前帧
	bool							m_bBombEffect[COUNT_AZIMUTH];			// 爆炸效果
	BYTE							m_cbBombFrameIndex[COUNT_AZIMUTH];		// 帧数索引

	//历史信息
protected:
	__int64                         m_lMeStatisticScore;				  // 游戏成绩
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];// 游戏记录
	int								m_nRecordFirst;						  // 开始记录
	int								m_nRecordLast;						  // 最后记录
	CArray<tagApplyUser>            m_ApplyUserArray;                     // 申请庄家队列
	int                             m_iApplyFirst;                        // 重第几个开始显示

	//状态变量
protected:
	WORD							m_wMeChairID;						//我的位置
	BYTE							m_cbWinnerSide;						//胜利玩家
	BYTE							m_cbAreaFlash;						//胜利玩家
	__int64							m_lCurrentJetton;					//当前筹码
	CString							m_strDispatchCardTips;				//发牌提示
	bool							m_bShowChangeBanker;				//轮换庄家
	bool							m_bShowChangeSicbo;				    // 改变摇骰子方式

	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	__int64							m_lBankerScore;						//庄家积分
	__int64							m_lTmpBankerWinScore;				//庄家成绩	
	bool							m_bEnableSysBanker;					//系统做庄
	E_CARD_TYPE                     m_enSelArea;                        // 选择区域
	__int64                         m_lSelAreaMax;                      // 选择区域可下分

	//当局成绩
public:
	__int64							m_lMeCurGameScore;					//我的成绩
	__int64							m_lMeCurGameReturnScore;			//我的成绩
	__int64							m_lBankerCurGameScore;				//庄家成绩
	__int64							m_lGameRevenue;						//游戏税收
	__int64							m_lBankerWinScore;					//庄家成绩

	//数据变量
protected:
	CPoint							m_PointJetton[COUNT_AZIMUTH];					//筹码位置
	CPoint							m_PointJettonNumber[COUNT_AZIMUTH];				//数字位置
	CJettonInfoArray				m_JettonInfoArray[COUNT_AZIMUTH];				//筹码数组

	//控件变量
public:
	E_SICBO_TYPE                    m_enSicboType;                       // 摇骰子类型
	CSkinButtonEx						m_btJetton100;						//筹码按钮
	CSkinButtonEx						m_btJetton1000;						//筹码按钮
	CSkinButtonEx						m_btJetton10000;					//筹码按钮
	CSkinButtonEx						m_btJetton100000;					//筹码按钮
	CSkinButtonEx						m_btJetton500000;					//筹码按钮
	CSkinButtonEx						m_btJetton1000000;					//筹码按钮
	CSkinButtonEx						m_btJetton5000000;					//筹码按钮
	
	CSkinButtonEx						m_btApplyBanker;					//申请庄家
	CSkinButtonEx						m_btCancelBanker;					//取消庄家
	CSkinButtonEx						m_btBankerMoveUp;					//移动庄家
	CSkinButtonEx						m_btBankerMoveDown;					//移动庄家

	CSkinButton						m_btScoreMoveL;						//移动成绩
	CSkinButton						m_btScoreMoveR;						//移动成绩
	
	CSkinButton                     m_btSicboDesktop;                    // 桌面摇骰子
	CSkinButton                     m_btSicboInMidair;                   // 半空摇骰子
	CSkinButton                     m_btSicboInTheAir;                   // 空中摇骰子

	CSkinButton                     m_btOpenBank;                       // 银行

	//控件变量
public:
	CGameClientDlg					*m_pGameClientDlg;					//父类指针
	CGameLogic						m_GameLogic;						//游戏逻辑

	//界面变量
protected:
	CSkinImage						m_ImageViewFill;					//背景位图
	CSkinImage						m_ImageViewBack;					//背景位图
	CSkinImage						m_ImageWinFlags;					//标志位图
	CSkinImage						m_ImageCardDot;					    //骰子点数
	CPngImage						m_ImageJettonView;					//筹码视图
	CSkinImage						m_ImageScoreNumber;					//数字视图
	CSkinImage						m_ImageMeScoreNumber;				//数字视图
	CSkinImage						m_ImageTimeFlag;					//时间标识
	CPngImage						m_ImageBombEffect;					//动画图片
	CPngImage						m_ImgSicboEffectDesktop;			//动画图片
	CPngImage						m_ImgSicboEffectInMidair;			//动画图片
	CPngImage						m_ImgSicboEffectInTheAir;			//动画图片
	CPngImage						m_ImgSicboEffectNormal;			    //动画图片
	CPngImage						m_ImgSicboEffectResult;			    //动画图片
	CPngImage						m_ImgSicboEffectBack;			    //动画图片
	CPngImage						m_ImgSicboGiftInTheAir;			    //动画图片
	bool                            m_bShowSicboEffectBack;             // 是否显示动画背景
	CPngImage						m_PngSicboArea[COUNT_AZIMUTH];		// 每个区域鼠标移动时的背景图

	CPngImage						m_ImgSicboEPlan;			        //动画图片
	CPngImage						m_ImgSicboPlan;			            //动画图片
	CPngImage						m_ImgSicboNum;			            //动画图片
	CPngImage						m_ImgSicboAnim[SICBO_ANIM_FRAME_COUNT]; //动画图片

	//边框
protected:
	CPngImage						m_ImageFrameWin;		//边框图片
	CSkinImage						m_ImageMeBanker;					//切换庄家
	CSkinImage						m_ImageChangeBanker;				//切换庄家
	CSkinImage						m_ImageNoBanker;					//切换庄家

	CSkinImage                      m_ImageSicboDesktop;                 // 桌面摇骰子
	CSkinImage                      m_ImageSicboInMidair;                // 半空摇骰子
	CSkinImage                      m_ImageSicboInTheAir;                // 空中摇骰子

	//结束资源
protected:
	CPngImage						m_PngGameEnd;						//成绩图片
	CSkinImage						m_ImageCardType;					//牌型图片
	CSkinImage						m_ImageSicboPoint;					//牌点数图片
	CMutex                          m_ApplyBankerLock;
	CTransparentCtrl                m_TCPrompt;

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

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	//设置函数
public:
	//设置信息
	void SetGameMaxScore(__int64 lMeMaxScore, __int64 lAllMaxScore);

	//我的位置
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID;}
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount);
	//庄家信息
	void SetBankerInfo(WORD wBankerUser, __int64 lBankerScore);
	//庄家成绩
	void SetBankerScore(WORD wBankerTime, __int64 lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//当局成绩
	void SetCurGameScore(__int64 lMeCurGameScore, __int64 lMeCurGameReturnScore, __int64 lBankerCurGameScore, __int64 lGameRevenue);
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, __int64 lScoreCount);
	//区域限制
	void SetAreaLimitScore(__int64 lAreaLimitScore);	
	//设置扑克
	void SetCardInfo(E_SICBO_NUMBER enCards[MAX_COUNT_SICBO], bool bWinner[COUNT_AZIMUTH]);
	//设置筹码
	void SetCurrentJetton(__int64 lCurrentJetton);
	//历史记录
	void SetGameHistory(tagClientGameRecord &gameRecord);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//成绩设置
	void SetGameScore(__int64 lMeCurGameScore, __int64 lMeCurGameReturnScore, __int64 lBankerCurGameScore);
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}

	//改变摇骰子方式
	void SetSicboType(E_SICBO_TYPE enSicboType);

	void AddApplyBanker(tagApplyUser &applyUser);
	void CancelApplyBanker(tagApplyUser &applyUser);
	void UpdateApplyBanker(tagApplyUser &applyUser);

	//动画函数
public:	
	// 开始发牌
	void DispatchCard();
	void DispatchCardSicbo();
	void DispatchCardNormal();

	//结束发牌
	void FinishDispatchCard();
	//设置爆炸
	bool SetBombEffect(bool bBombEffect, int iAreaIndex);
	//绘画爆炸
	void DrawBombEffect(CDC *pDC);

	// 摇骰子动画
	void DrawSicboAnim(CDC *pDC);
	void DrawSicboAnimNormal(CDC *pDC);
	void DrawSicboAnimDesktop(CDC *pDC);
	void DrawSicboAnimInMidair(CDC *pDC);
	void DrawSicboAnimInTheAir(CDC *pDC);
	void DrawSicboAnimSicbo(CDC *pDC);

	//最大下注
	__int64 GetMaxPlayerScore(int iArea);

	//绘画函数
protected:
	//闪烁边框
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//界面函数
public:
	//清理筹码
	void CleanUserJetton();
	//设置胜方
	void SetWinnerSide(BYTE cbWinnerSide);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//绘画数字
	void DrawMeJettonNumber(CDC *pDC);

	//内联函数
public:
	//当前筹码
	inline __int64 GetCurrentJetton() { return m_lCurrentJetton; }	

	//内部函数
private:
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//绘画数字
	void DrawNumberString(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, CRect rcRect, INT nFormat=-1);
	//绘画标识
	void DrawWinFlags(CDC * pDC);
	//显示结果
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//透明绘画
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);

	//按钮消息
protected:
	//筹码按钮
	afx_msg void OnJettonButton100();
	//筹码按钮
	afx_msg void OnJettonButton1000();
	//筹码按钮
	afx_msg void OnJettonButton10000();
	//筹码按钮
	afx_msg void OnJettonButton100000();
	//筹码按钮
	afx_msg void OnJettonButton500000();
	//筹码按钮
	afx_msg void OnJettonButton1000000();
	//筹码按钮
	afx_msg void OnJettonButton5000000();
	//移动庄家
	afx_msg void OnBtnBankerMoveUp();
	afx_msg void OnBtnBankerMoveDown();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//移动按钮
	afx_msg void OnScoreMoveL();
	//移动按钮
	afx_msg void OnScoreMoveR();
	// 桌面摇骰子
	afx_msg void OnBtnSicboDesktop();
	// 半空摇骰子
	afx_msg void OnBtnSicboInMidair();
	// 空中摇骰子
	afx_msg void OnBtnSicboInTheAir();
	// 银行操作
	afx_msg void OnBtnOpenBank();
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

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////

#endif
