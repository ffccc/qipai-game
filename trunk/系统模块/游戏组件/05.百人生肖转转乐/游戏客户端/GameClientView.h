#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ApplyUserList.h"
#include <afxtempl.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////
//时间标识
#define IDI_FLASH_WINNER			100									//闪动标识
#define IDI_SHOW_CHANGE_BANKER		101									//轮换庄家
#define IDI_FLASH_ANIMAL			102									//闪动动物
#define IDT_RAND_FLASH				103									//随机闪动
#define IDT_COUNT_TIME				104
//筹码定义
#define JETTON_COUNT				6									//筹码数目
#define JETTON_RADII				20									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息
//#define IDM_SHOW_HISTORY            WM_USER+202

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//历史记录
#define MAX_SCORE_HISTORY			256									//历史个数

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
typedef CArray<tagApplyUser, tagApplyUser&> CApplyUserList;

//////////////////////////////////////////////////////////////////////////

class CSkinImageEx : public CSkinImage
{
public:
	bool LoadFromResource(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszResType, bool bAlphaChannel)
	{
		//变量定义
		HRSRC hResource = NULL;
		HGLOBAL hGlobal = NULL;
		IStream *pIStream = NULL;

		try
		{
			//位图格式
			if (pszResType==RT_BITMAP)
			{
				HBITMAP hBitmap =  (HBITMAP)::LoadImage(hInstance,pszResource,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
				if (hBitmap==NULL) throw (0);
				Attach(hBitmap);
			}
			else
			{
				//查找资源
				hResource = FindResource(hInstance,pszResource,pszResType);
				if (hResource == NULL) throw 0;

				//加载资源
				hGlobal = LoadResource(hInstance,hResource);
				if (hGlobal == NULL) throw 0;

				//读取资源
				LPVOID pImageBuffer = ::LockResource(hGlobal);
				DWORD dwImageSize = SizeofResource(hInstance,hResource);

				//创建数据
				CreateStreamOnHGlobal(NULL, TRUE, &pIStream);
				if (pIStream == NULL) throw 0;

				//写入数据
				pIStream->Write(pImageBuffer,dwImageSize,NULL);

				//装入图像
				if (Load(pIStream) != S_OK) throw 0;

				//释放资源
				if (pIStream != NULL) 
					pIStream->Release();

				//是否资源
				if (hGlobal!=NULL)
				{
					UnlockResource(hGlobal);
					FreeResource(hGlobal);
				}
			}

			//初始化通道
			if (bAlphaChannel) InitAlphaChannel();

			return true;
		}
		catch(...)
		{
			//释放资源
			if (pIStream != NULL)
				pIStream->Release();

			//是否资源
			if (hGlobal!=NULL)
			{
				UnlockResource(hGlobal);
				FreeResource(hGlobal);
			}

			return false;
		}
	}

	bool LoadFromResource(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pszResType, bool bAlphaChannel)
	{
		return LoadFromResource(hInstance,MAKEINTRESOURCE(nIDResource), pszResType, bAlphaChannel);
	}
protected:
	bool InitAlphaChannel()
	{
		ASSERT(IsNull()==false);
		ASSERT(GetBPP()==32);

		if (IsNull()) return false;
		if (GetBPP()!=32) return false;

		LONG lWidth = GetWidth();
		LONG lHeight = GetHeight();
		BYTE *pPixel = NULL;
		BYTE bAlpha = 0;
		for (LONG y = 0; y < lHeight; y++)
		{
			for (LONG x = 0; x < lWidth; x++)
			{
				pPixel = (BYTE*)GetPixelAddress(x, y);
				bAlpha = pPixel[3];
				if (bAlpha < 255)
				{
					pPixel[0] = pPixel[0] * bAlpha / 255;
					pPixel[1] = pPixel[1] * bAlpha / 255;
					pPixel[2] = pPixel[2] * bAlpha / 255;
				}
			}
		}

		m_bHasAlphaChannel=true;
		return true;
	}

	bool								m_bHasAlphaChannel;
};


//游戏视图
class CGameClientView : public CGameFrameView
{
	//下注信息
protected:
	LONG						m_lMeMaxScore;						//最大下注

	//庄家信息
public:
	bool						m_bShowChangeBanker;				//轮换庄家
	LONG						m_lAreaLimitScore;					//区域限制
	WORD						m_wCurrentBankerChairID;			//当前庄家
	BYTE						m_cbBankerTime;						//做庄次数
	LONG						m_lBankerScore;						//庄家成绩
	LONG						m_lBankerTreasure;					//庄家金币

	//当局成绩
public:
	INT64						m_lMeCurGameScore;					//我的成绩
	INT64						m_lTempCurGameScore;
	INT64						m_lBankerCurGameScore;				//返还积分
	INT64						m_lBankerTotal;						//总计
	bool						m_bShowGameResult;					//显示积分	
	//控件变量
public:
	CSkinButton					m_btApplyBanker;					//申请庄家
	CSkinButton					m_btCancelBanker;					//取消庄家
	CSkinButton					m_btScoreMoveL;						//移动成绩
	CSkinButton					m_btScoreMoveR;						//移动成绩
	CSkinButton                 m_btSaveScroe;                      //存份按钮
	CSkinButton                 m_btFetchScore;                     //取份按钮
	CSkinButton                 m_btHistory;                        //历史记录

//////////////////////////////////////////////////////////////////////////


public:
	BYTE						m_cbAnimalPos;
	DWORD						m_dwCurrentAniPos;
	DWORD						m_dwCursorArea;
	CRect						m_rcCurAreaRect;
	HCURSOR                     m_hCursorArray[JETTON_COUNT];       //筹码光标

	CSkinButton					m_btJetton100;						//筹码按钮
	CSkinButton					m_btJetton1K;						//筹码按钮
	CSkinButton					m_btJetton1W;					    //筹码按钮
	CSkinButton					m_btJetton10W;					    //筹码按钮
	CSkinButton					m_btJetton100W;						//筹码按钮
	CSkinButton					m_btJetton1000W;					//筹码按钮
	CSkinButton					m_btBankerIn;
	CSkinButton					m_btBankerOut;
	CSkinButton					m_btPrevious;
	CSkinButton					m_btNext;
	CSkinButton					m_btBankerUp;
	CSkinButton					m_btBankerDown;

	CSkinImageEx				m_ImageAnimal[12];
	CSkinImageEx  				m_ImageViewBack;					//背景图片
	CSkinImageEx				m_ImageBackBuf;	
	CSkinImageEx				m_ImageJettonView;					//筹码视图
	CSkinImageEx				m_ImgChipNum;
	CSkinImageEx				m_ImgChipNum1;
	CSkinImageEx				m_ImgChipNum2;
	CSkinImageEx				m_ImageText;
//	CSkinImageEx				m_ImageViewFill;					//背景位图
	CSkinImageEx                m_ImageTimerBack;                   //计时背景
	CSkinImageEx				m_ImageTimerNumber;                 //数字图片
	CSkinImageEx                m_ImageGameEnd;                     //分数图片
	CSkinImageEx				m_ImageFuhao;						//
	CSkinImageEx				m_ImageBG;
	CSkinImageEx				m_TimerText;

	BYTE						m_cbRandCount;						//18-36
	INT64						m_nTotalUserBet[MAX_AREA];
	INT64						m_nUserBet[MAX_AREA];

	
	BOOL						m_bFlashWinner;						//
	CApplyUserList              m_ApplyUserList;                    //申请列表
	std::vector<DWORD>			m_vecHistory;						//历史记录
	BOOL						m_bDisPlayEnd;
	int							m_cbTimeCount;
	WORD						m_wCursors;
	WORD						m_wBankerCursors;
	INT64						m_nValidMoney;						//闲家可用钱
	LONG						m_lCurrentJetton;					//当前筹码
//////////////////////////////////////////////////////////////////////////


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
	//最大下注
	void SetAreaLimitScore(LONG lAreaLimitScore);
	//设置筹码
	void SetCurrentJetton(LONG lCurrentJetton);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//庄家信息
	void SetBankerInfo( WORD wChairID, BYTE cbBankerTime, LONG lScore );
	//庄家金币
	void SetBankerTreasure(LONG lBankerTreasure);
	//成绩设置
	void SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore);
	//设置标识
	void SetShowGameFlag(bool bShowGameResult);
	//
	void StartMovingAnimal(BYTE animalpos );
	//
	void StartRandAnimal();
	//
	void DrawUserInfo( CDC *pDC , int nWidth , int nHeight );
	//
	void FormatNum( IN OUT TCHAR ch[] , IN INT64 num );
	//
	void RectifyArea( const int nWidth , const int nHeight );
	//
	void DeleteApplyList();
	
	//界面函数
public:
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//
	void DrawRect( CDC *pDC , CRect *pRect );
	//
	void DrawChipNum(CDC * pDC, Graphics *pGraphics , int centerx, int centery, INT64 num, int numtype, int nAlign=0);
	//
	void DrawUerBet(CDC *pDC);
	//
	void DrawHighLightAnimal(CDC *pDC ,int nWidth ,int nHeight );

	//内联函数
public:
	//当前筹码
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }

	//内部函数
private:
	//显示结果
	void DrawGameResult(CDC *pDC, int nXPos, int nYPos);
	//绘制定时
	void DrawTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, bool bAnimate=false);
	//绘制
	
	//按钮消息
protected:

	afx_msg void OnBankDown();
	//
	afx_msg void OnBankUp();
	//
	afx_msg void OnNext();
	//
	afx_msg void OnPrevious();
	//
	afx_msg	void OnBankIn();
	//
	afx_msg void OnBankOut();
	//筹码按钮
	afx_msg void OnJettonButton100();
	//筹码按钮
	afx_msg void OnJettonButton1K();
	//筹码按钮
	afx_msg void OnJettonButton1W();
	//筹码按钮
	afx_msg void OnJettonButton10W();
	//筹码按钮
	afx_msg void OnJettonButton100W();
	//筹码按钮
	afx_msg void OnJettonButton500W();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////
#endif