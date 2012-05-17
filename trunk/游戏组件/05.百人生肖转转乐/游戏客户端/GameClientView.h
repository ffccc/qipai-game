#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ApplyUserList.h"
#include <afxtempl.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////
//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_FLASH_ANIMAL			102									//��������
#define IDT_RAND_FLASH				103									//�������
#define IDT_COUNT_TIME				104
//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				20									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
//#define IDM_SHOW_HISTORY            WM_USER+202

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��ʷ��¼
#define MAX_SCORE_HISTORY			256									//��ʷ����

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
};

//��������
typedef CArrayTemplate<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;
typedef CArray<tagApplyUser, tagApplyUser&> CApplyUserList;

//////////////////////////////////////////////////////////////////////////

class CSkinImageEx : public CSkinImage
{
public:
	bool LoadFromResource(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszResType, bool bAlphaChannel)
	{
		//��������
		HRSRC hResource = NULL;
		HGLOBAL hGlobal = NULL;
		IStream *pIStream = NULL;

		try
		{
			//λͼ��ʽ
			if (pszResType==RT_BITMAP)
			{
				HBITMAP hBitmap =  (HBITMAP)::LoadImage(hInstance,pszResource,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
				if (hBitmap==NULL) throw (0);
				Attach(hBitmap);
			}
			else
			{
				//������Դ
				hResource = FindResource(hInstance,pszResource,pszResType);
				if (hResource == NULL) throw 0;

				//������Դ
				hGlobal = LoadResource(hInstance,hResource);
				if (hGlobal == NULL) throw 0;

				//��ȡ��Դ
				LPVOID pImageBuffer = ::LockResource(hGlobal);
				DWORD dwImageSize = SizeofResource(hInstance,hResource);

				//��������
				CreateStreamOnHGlobal(NULL, TRUE, &pIStream);
				if (pIStream == NULL) throw 0;

				//д������
				pIStream->Write(pImageBuffer,dwImageSize,NULL);

				//װ��ͼ��
				if (Load(pIStream) != S_OK) throw 0;

				//�ͷ���Դ
				if (pIStream != NULL) 
					pIStream->Release();

				//�Ƿ���Դ
				if (hGlobal!=NULL)
				{
					UnlockResource(hGlobal);
					FreeResource(hGlobal);
				}
			}

			//��ʼ��ͨ��
			if (bAlphaChannel) InitAlphaChannel();

			return true;
		}
		catch(...)
		{
			//�ͷ���Դ
			if (pIStream != NULL)
				pIStream->Release();

			//�Ƿ���Դ
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


//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��ע��Ϣ
protected:
	LONG						m_lMeMaxScore;						//�����ע

	//ׯ����Ϣ
public:
	bool						m_bShowChangeBanker;				//�ֻ�ׯ��
	LONG						m_lAreaLimitScore;					//��������
	WORD						m_wCurrentBankerChairID;			//��ǰׯ��
	BYTE						m_cbBankerTime;						//��ׯ����
	LONG						m_lBankerScore;						//ׯ�ҳɼ�
	LONG						m_lBankerTreasure;					//ׯ�ҽ��

	//���ֳɼ�
public:
	INT64						m_lMeCurGameScore;					//�ҵĳɼ�
	INT64						m_lTempCurGameScore;
	INT64						m_lBankerCurGameScore;				//��������
	INT64						m_lBankerTotal;						//�ܼ�
	bool						m_bShowGameResult;					//��ʾ����	
	//�ؼ�����
public:
	CSkinButton					m_btApplyBanker;					//����ׯ��
	CSkinButton					m_btCancelBanker;					//ȡ��ׯ��
	CSkinButton					m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton					m_btScoreMoveR;						//�ƶ��ɼ�
	CSkinButton                 m_btSaveScroe;                      //��ݰ�ť
	CSkinButton                 m_btFetchScore;                     //ȡ�ݰ�ť
	CSkinButton                 m_btHistory;                        //��ʷ��¼

//////////////////////////////////////////////////////////////////////////


public:
	BYTE						m_cbAnimalPos;
	DWORD						m_dwCurrentAniPos;
	DWORD						m_dwCursorArea;
	CRect						m_rcCurAreaRect;
	HCURSOR                     m_hCursorArray[JETTON_COUNT];       //������

	CSkinButton					m_btJetton100;						//���밴ť
	CSkinButton					m_btJetton1K;						//���밴ť
	CSkinButton					m_btJetton1W;					    //���밴ť
	CSkinButton					m_btJetton10W;					    //���밴ť
	CSkinButton					m_btJetton100W;						//���밴ť
	CSkinButton					m_btJetton1000W;					//���밴ť
	CSkinButton					m_btBankerIn;
	CSkinButton					m_btBankerOut;
	CSkinButton					m_btPrevious;
	CSkinButton					m_btNext;
	CSkinButton					m_btBankerUp;
	CSkinButton					m_btBankerDown;

	CSkinImageEx				m_ImageAnimal[12];
	CSkinImageEx  				m_ImageViewBack;					//����ͼƬ
	CSkinImageEx				m_ImageBackBuf;	
	CSkinImageEx				m_ImageJettonView;					//������ͼ
	CSkinImageEx				m_ImgChipNum;
	CSkinImageEx				m_ImgChipNum1;
	CSkinImageEx				m_ImgChipNum2;
	CSkinImageEx				m_ImageText;
//	CSkinImageEx				m_ImageViewFill;					//����λͼ
	CSkinImageEx                m_ImageTimerBack;                   //��ʱ����
	CSkinImageEx				m_ImageTimerNumber;                 //����ͼƬ
	CSkinImageEx                m_ImageGameEnd;                     //����ͼƬ
	CSkinImageEx				m_ImageFuhao;						//
	CSkinImageEx				m_ImageBG;
	CSkinImageEx				m_TimerText;

	BYTE						m_cbRandCount;						//18-36
	INT64						m_nTotalUserBet[MAX_AREA];
	INT64						m_nUserBet[MAX_AREA];

	
	BOOL						m_bFlashWinner;						//
	CApplyUserList              m_ApplyUserList;                    //�����б�
	std::vector<DWORD>			m_vecHistory;						//��ʷ��¼
	BOOL						m_bDisPlayEnd;
	int							m_cbTimeCount;
	WORD						m_wCursors;
	WORD						m_wBankerCursors;
	INT64						m_nValidMoney;						//�мҿ���Ǯ
	LONG						m_lCurrentJetton;					//��ǰ����
//////////////////////////////////////////////////////////////////////////


	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ܺ���
public:
	//�����ע
	void SetAreaLimitScore(LONG lAreaLimitScore);
	//���ó���
	void SetCurrentJetton(LONG lCurrentJetton);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//ׯ����Ϣ
	void SetBankerInfo( WORD wChairID, BYTE cbBankerTime, LONG lScore );
	//ׯ�ҽ��
	void SetBankerTreasure(LONG lBankerTreasure);
	//�ɼ�����
	void SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore);
	//���ñ�ʶ
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
	
	//���溯��
public:
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//
	void DrawRect( CDC *pDC , CRect *pRect );
	//
	void DrawChipNum(CDC * pDC, Graphics *pGraphics , int centerx, int centery, INT64 num, int numtype, int nAlign=0);
	//
	void DrawUerBet(CDC *pDC);
	//
	void DrawHighLightAnimal(CDC *pDC ,int nWidth ,int nHeight );

	//��������
public:
	//��ǰ����
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }

	//�ڲ�����
private:
	//��ʾ���
	void DrawGameResult(CDC *pDC, int nXPos, int nYPos);
	//���ƶ�ʱ
	void DrawTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, bool bAnimate=false);
	//����
	
	//��ť��Ϣ
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
	//���밴ť
	afx_msg void OnJettonButton100();
	//���밴ť
	afx_msg void OnJettonButton1K();
	//���밴ť
	afx_msg void OnJettonButton1W();
	//���밴ť
	afx_msg void OnJettonButton10W();
	//���밴ť
	afx_msg void OnJettonButton100W();
	//���밴ť
	afx_msg void OnJettonButton500W();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////
#endif