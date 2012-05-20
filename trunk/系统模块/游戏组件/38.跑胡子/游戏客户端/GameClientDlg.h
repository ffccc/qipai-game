#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//动作类型
#define ACTION_TIP					1									//提示动作
#define ACTION_TI_CARD				2									//提牌动作
#define ACTION_PAO_CARD				3									//跑牌动作
#define ACTION_WEI_CARD				4									//偎牌动作
#define ACTION_PENG_CARD			5									//碰牌动作
#define ACTION_CHI_CARD				6									//吃牌动作
#define ACTION_OUT_CARD				7									//出牌动作
#define ACTION_SEND_CARD			8									//发牌动作
#define ACTION_OPERATE_NOTIFY		9									//发牌动作
#define ACTION_OUT_CARD_NOTIFY		10									//发牌动作

//动作子项
struct tagUserAction
{
	BYTE							cbActionKind;						//动作类型
	BYTE							cbActionBuffer[64];					//动作内容
};

//提示动作
struct tagActionTip
{
	WORD							wChairID;							//用户位置
	BYTE							cbActionFlags;						//动作标识
};

//提牌动作
struct tagActionTiCard
{
	BYTE							cbIndex;							//索引号码
	WORD							wChairID;							//用户位置
	BYTE							bHuXiCount;							//胡息数目
	BYTE							cbRemoveCount;						//删除数目
};

//跑牌动作
struct tagActionPaoCard
{
	BYTE							cbIndex;							//索引号码
	WORD							wChairID;							//用户位置
	BYTE							bHuXiCount;							//胡息数目
	BYTE							cbRemoveCount;						//删除数目
};

//偎牌动作
struct tagActionWeiCard
{
	BYTE							cbIndex;							//索引号码
	WORD							wChairID;							//用户位置
	BYTE							bHuXiCount;							//胡息数目
};

//碰牌动作
struct tagActionPengCard
{
	BYTE							cbIndex;							//索引号码
	WORD							wChairID;							//用户位置
	BYTE							bHuXiCount;							//胡息数目
};

//吃牌动作
struct tagActionChiCard
{
	BYTE							cbIndex;							//索引号码
	WORD							wChairID;							//用户位置
	BYTE							bHuXiCount;							//胡息数目
	BYTE							cbActionCard;						//碰牌扑克
	BYTE							cbResultCount;						//结果数目
	BYTE							cbCardData[3][3];					//吃牌组合
};

//出牌动作
struct tagActionOutCard
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
};

//发牌动作
struct tagActionSendCard
{
	WORD							wAttachUser;						//绑定用户
	BYTE							cbSendCardData;						//发牌扑克
};

//操作提示
struct tagActionOperateNotify
{
	WORD							wResumeUser;						//还原用户
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbCurrentCard;						//当前扑克
};

//出牌提示
struct tagActionOutCardNotify
{
	BYTE							bOutCard;							//出牌标志
	WORD							wCurrentUser;						//当前用户
};

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
	//游戏变量
protected:
	WORD							m_wBankerUser;						//庄家用户
	BYTE							m_bUserCardCount[3];				//扑克数目
	BYTE							m_bUserHuXiCount[3];				//胡息数目

	//辅助变量
protected:
	bool							m_bOutCard;							//出牌标志
	WORD							m_wResumeUser;						//还原用户
	WORD							m_wCurrentUser;						//当前用户

	//组合扑克
protected:
	BYTE							m_bWeaveItemCount[3];				//组合数目
	tagWeaveItem					m_WeaveItemArray[3][7];				//组合扑克

	//扑克变量
protected:
	WORD							m_wOutCardUser;						//出牌用户
	BYTE							m_cbOutCardData;					//出牌扑克
	BYTE							m_cbCardIndex[MAX_INDEX];			//手中扑克

	//动作变量
protected:
	WORD							m_wActionCount;						//动作数目
	tagUserAction					m_UserActionArray[32];				//用户动作

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图
	
	//函数定义
public:
	//构造函数
	CGameClientDlg();
	//析构函数
	virtual ~CGameClientDlg();

	//常规继承
private:
	//初始函数
	virtual bool InitGameFrame();
	//重置框架
	virtual void ResetGameFrame();
	//游戏设置
	virtual void OnGameOptionSet();
	//时间消息
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//旁观状态
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//网络消息
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//游戏场景
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);

	//动作函数
protected:
	//中止动作
	bool HangUserAction();
	//执行动作
	bool ExecuteActionSendCard(BYTE cbCardData, WORD wAttachUser);
	//执行动作
	bool ExecuteActionOutCard(WORD wOutCardUser, BYTE cbOutCardData);
	//执行动作
	bool ExecuteActionOutCardNotify(WORD wCurrentUser, BYTE bOutCard);
	//执行动作
	bool ExecuteAction(BYTE cbActionKind, void * pActionInfo, WORD wDataSize);
	//执行动作
	bool ExecuteActionOperateNotify(BYTE cbOperateCode, BYTE cbOperateCard, WORD wResumeUser);

	//操作动作
protected:
	//执行动作
	bool ExecuteActionWeiCard(WORD wChairID, BYTE cbCardData);
	//执行动作
	bool ExecuteActionPengCard(WORD wChairID, BYTE cbCardData);
	//执行动作
	bool ExecuteActionTiCard(WORD wChairID, BYTE cbCardData, BYTE cbRemoveCount);
	//执行动作
	bool ExecuteActionPaoCard(WORD wChairID, BYTE cbCardData, BYTE cbRemoveCount);
	//执行动作
	bool ExecuteActionChiCard(WORD wChairID, BYTE cbCardData, BYTE cbResultCount, BYTE cbResultData[][3]);

	//辅助函数
protected:
	//更新胡息
	bool UpdateUserHuXiCount(WORD wChairID);
	//删除扑克
	bool RemoveControlCard(WORD wChairID, BYTE cbRemoveCard[], WORD wCardCount);

	//消息处理
protected:
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户提牌
	bool OnSubUserTiCard(const void * pBuffer, WORD wDataSize);
	//用户跑牌
	bool OnSubUserPaoCard(const void * pBuffer, WORD wDataSize);
	//用户偎牌
	bool OnSubUserWeiCard(const void * pBuffer, WORD wDataSize);
	//用户碰牌
	bool OnSubUserPengCard(const void * pBuffer, WORD wDataSize);
	//用户吃牌
	bool OnSubUserChiCard(const void * pBuffer, WORD wDataSize);
	//操作提示
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//出牌提示
	bool OnSubOutCardNotify(const void * pBuffer, WORD wDataSize);
	//用户出牌
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//发牌命令
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//消息映射
protected:
	//时间消息
	afx_msg void OnTimer(UINT nIDEvent);
	//开始消息
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//出牌操作
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//选牌操作
	LRESULT OnChooseCard(WPARAM wParam, LPARAM lParam);
	//扑克操作
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
