#pragma once

#include "Stdafx.h"
#include <vector>
#include "GameLogic.h"
#include "TimerControl.h"
#include "ScoreView.h"
//#include "GameParrentView.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_MJ					(WM_USER+101)						//������Ϣ
#define IDM_ADDFLOWER_MJ			(WM_USER+102)						//������Ϣ
#define IDM_HU_MJ					(WM_USER+103)						//������Ϣ
#define IDM_TING_MJ					(WM_USER+104)						//������Ϣ
#define IDM_GANG_MJ					(WM_USER+105)						//������Ϣ
#define IDM_PENG_MJ					(WM_USER+106)						//������Ϣ
#define IDM_CHI_MJ					(WM_USER+107)						//������Ϣ
#define IDM_GIVEUP					(WM_USER+108)						//������Ϣ
#define IDM_AUTO_OUTMJ				(WM_USER+109)                       //�Զ�����

#define IDM_LEFTHIT_MJ				(WM_USER+110)						//�������
#define IDM_RIGHTHIT_MJ				(WM_USER+111)						//�Ҽ�����

#define IDM_SELECT_CHI				(WM_USER+112)						//ѡ�����
#define IDM_SELECT_GANG				(WM_USER+113)						//ѡ�����

#define IDM_CANCEL2					(WM_USER+114)						//�ر���Ϣ
#define IDM_SHOWMINI				(WM_USER+115)						//��С����Ϣ
#define IDM_GAMESET					(WM_USER+116)						//��Ϸ����
/////////////////////////////////////////////////////////////////////////



//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;

public:
	int									m_iUpMj;						//�齫�����־
	int									m_tOutStartTime;				//���ƶ�����ʼʱ��
	int									m_tOut;							//���ƶ���ʱ��
	bool								m_bActionOut;					//���ƶ���
	bool								m_bPlay;						//���Ʊ�־
	int									m_tDice;						//���Ӷ���ʱ��
	CTimerControl						m_Timer;						//ʱ�ӿؼ�

	//���ñ���
protected:
	bool								m_bEnd;							//����
	bool								m_bEndEmpty;					//����
	bool								m_bAction;						//����


	int									m_nChiMode;						//����ģʽ
	int 								m_nGangMode;					//����ģʽ
	int									m_nChiType;						//��������
	int									m_nGangType;					//��������

	BYTE								m_byGangMj[(HAND_MJ_MAX-2)/3-1];//���ƿ�ѡ��

	BYTE								m_byOutMj;						//�������
	bool								m_bDeasilOrder;					//����˳��
	bool								m_bLookOn;						//�Թ�

	int									m_iOldUpMj;						//�ɱ�־
	WORD								m_wLastUser;					//�ϴγ�����
	WORD								m_wCurrentUser;					//��ǰ���
	WORD								m_wBankerUser;					//ׯ��


	//���ݱ���
protected:
	BYTE								m_byTouchMjCount[GAME_PLAYER];	//����������
	BYTE								m_byHandMjCount[GAME_PLAYER];	//��������
	std::vector< BYTE >					m_vcOutMjs[GAME_PLAYER];		//����������
	MJ_HU								m_hu[GAME_PLAYER];				//����������

	BYTE								m_byFengWei[GAME_PLAYER];		//��λ����
	WORD								m_wQuanWei;						//Ȧ��
	WORD								m_wQuanWeiBanker;				//Ȧ��ׯλ


	int									m_nActionOutFlag;				//���Ʊ�־
	int									m_nDiceCount1;					///������1
	int									m_nDiceCount2;					//������2


	int									m_tOutFlag;						//���Ʊ�־֡


	//λ�ñ���
protected:
	POINT								m_ptTouchMjs[GAME_PLAYER];		//��������ʼλ
	POINT								m_ptHuMjs[GAME_PLAYER];			//��������ʼλ
	POINT								m_ptOutMjs[GAME_PLAYER];		//��������ʼλ
	POINT								m_ptFaceMj[GAME_PLAYER];		//������ʼλ
	POINT								m_ptGetMj[GAME_PLAYER];			//����λ
	POINT								m_ptFengWei[GAME_PLAYER];		//��λ
	POINT								m_ptHua[GAME_PLAYER];			//����

	POINT								m_ptQuanFengBk;					//Ȧ�米��ͼ
	POINT								m_ptQuanFeng;					//Ȧ��
	POINT								m_ptActionOut;					//���ƶ�����ʼλ
	POINT								m_ptActionOutFlag;				//���Ʊ�־��ʼλ
	POINT								m_ptActionDice;					//���Ӷ�����ʼλ
	POINT								m_ptActionDice2;				//���Ӷ���

	//�������
protected:
	CSize								m_csTouchMj[GAME_PLAYER];		//�����齫��С
	CSize								m_csFaceMj[GAME_PLAYER];		//�����齫��С
	CSize								m_csOutMj[GAME_PLAYER];			//�����齫��С
	CSize								m_csHuMj[GAME_PLAYER];			//�������齫��С

	CSize								m_csFengWei;					//��λͼ��С
	CSize								m_csQuanFeng;					//Ȧ��ͼ��С
	CSize								m_csActionOut;					//���ƶ�����С
	CSize								m_csActionOutFlag;				//���Ʊ�־��С
	CSize								m_csActionDice1;				//���Ӷ�����С
	CSize								m_csActionDice2;				//���Ӵ�С
	CSize								m_csActionOption;				//����ͼ��С

	//�ؼ�����
public:
	CSkinButton							m_btStart;						//��ʼ��Ť
	CSkinButton							m_btHu;							//���ư�Ť
	CSkinButton							m_btTing;						//���ư�Ť
	CSkinButton							m_btGang;						//���ư�Ť
	CSkinButton							m_btPeng;						//���ư�Ť
	CSkinButton							m_btChi;						//���ư�Ť
	CSkinButton							m_btGiveUp;						//������Ť

	CSkinButton							m_btShowMini;					//��С����Ť
	CSkinButton							m_btCancel;						//�رհ�Ť
	CSkinButton							m_btGameSet;					//���ð�Ť

	CSkinButton							btTest;							//�����ð�Ť
	CSkinButton							btTest2;						//����2
	CSkinButton							btTest3;						//����3
	//CSkinButton						btTest4;						//����4

	//λͼ����
protected:
	CSkinImage							m_ImageBack;					//������Դ
	CSkinImage							m_ImageFengWei;					//��λ��Դ
	CSkinImage							m_ImageBack1;

	CSkinImage							m_ImageButtonBk;				//��Ť����ͼ

	CSkinImage							m_ImageFaceMj[GAME_PLAYER];		//���������
	CSkinImage							m_ImageBackMj;					//�齫����ͼ
	CSkinImage							m_ImageTouchMj[GAME_PLAYER];	//�������齫ͼ
	CSkinImage							m_ImageOutMj[GAME_PLAYER];		//�������齫ͼ
	CSkinImage							m_ImageHuMj[GAME_PLAYER];		//�������齫ͼ

	CSkinImage							m_ImageMaskMj;					//ѡ��ģʽ-����ͼ

	CSkinImage							m_ImageQuanFeng;				//Ȧ��ͼ
	CSkinImage							m_ImageQuanFengBk;				//Ȧ��ͼ����

	CSkinImage							m_ActionOut;					//���ƶ���
	CSkinImage							m_ActionOutFlag;				//���Ʊ�־����
	CSkinImage							m_ActionDice1;					//���Ӷ���
	CSkinImage							m_ActionDice2;					//���Ӷ�������
	CSkinImage							m_ActionOption;					//��������]

	CSkinImage							m_ImageEndEmpty;				//����

	//�齫��Ϣ
protected:
	BYTE								m_byHandMjs[4][14];				//�����齫


	CScoreView							m_scoreView;					//������ͼ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//��ͼ����
	//����
	void SetImageEmpty( CDC *pDC );
	//��������
	void SetImageEnd( CDC *pDC, BYTE byPlayer );
	//��������
	void SubTouchMJ();
	//������
	void SetImageTMj( CDC *pDC, BYTE byPlayer );
	//������
	void SetImageOMj( CDC *pDC, BYTE byPlayer );
	//�����������
	void SetImageFMj( CDC *pDC, BYTE byPlayer );
	//��������
	void SetImageSelfMj( CDC *pDC, BYTE byData[], BYTE byMjCount );
	//������
	void SetImageHMj( CDC *pDC, BYTE byPlayer );
	//��λ
	void SetImageFengWei( CDC *pDC, BYTE byPlayer );
	//Ȧ����
	void SetImageQuanFeng( CDC *pDC );
	//ѡ�����
	void SetImageSelectChi( CDC *pDC, BYTE byData[], BYTE byMjCount, int nChiMode, BYTE byMjOut );
	//ѡ�����
	void SetImageSeletGang( CDC *pDC, BYTE byData[], BYTE byMjCount, int nGangMode, BYTE byGangMj[]);

	//���Ӷ���
	void  ActionDice1( CDC *pDC,int nFrame);
	//���Ӷ���2
	void ActionDice2( CDC *pDC,int pos1, int pos2 );
	//���ƶ���
	void ActionOut( CDC *pDC, int x, int y, int nFrame );
	//��������
	void ActionOption( CDC *pDC, BYTE byPlayer, int OptionType );
	//��־���ƶ���
	void ActionOutFlag( CDC *pDC, int x, int y, int nFrame );
	//��ȡ�齫
	void GetHandMj( BYTE byMj[], BYTE byMjCount );
	//���������
	void AddOutMj( WORD wChairID,BYTE data );
	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ú���
public:
	//�û�˳��
	void SetUserOrder( bool bDeasilOrder );
	//��ȡ˳��
	bool IsDeasilOrder()	{ return m_bDeasilOrder; }

	//��������
public:
	//���ð�Ť
	void HideButton();
	//�����ͼ
	void CleanUpView();
	//�����������
	void SetArtText( CDC *pDC, int x, int y, COLORREF nColor, COLORREF nBkColor,LPCTSTR lpszString  );

	//��Ϣӳ��
protected:
	afx_msg void OnTest();
	afx_msg void OnTest2();
	afx_msg void OnTest3();
	//�رհ�Ť
	afx_msg void OnCancel();
	//��С������
	afx_msg void OnShowMini();
	//��Ϸ����
	afx_msg void OnGameSet();
	//��ʼ��Ť
	afx_msg void OnStart();
	//���ư�Ť
	afx_msg void OnHuMj();
	//���ư�Ť
	afx_msg void OnTingMj();
	//���ư�Ť
	afx_msg void OnGangMj();
	//���ư�Ť
	afx_msg void OnPengMj();
	//���ư�Ť
	afx_msg void OnChiMj();
	//������Ť
	afx_msg void OnGiveUp();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//����һ�
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);	
	//������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�ƶ����
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
