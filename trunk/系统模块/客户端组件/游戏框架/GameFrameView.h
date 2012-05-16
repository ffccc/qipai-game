#ifndef GAME_FRAME_VIEW_HEAD_FILE
#define GAME_FRAME_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameFrame.h"
#include "ThreadDraw.h"

//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagFlowerEffectInfo
{
	//�ƶ���Ϣ
	WORD							wMoveCount;							//�ƶ�����
	WORD							wMoveIndex;							//�ƶ�����
	CPoint							ptFrom;								//����λ��
	CPoint							ptDest;								//Ŀ��λ��
	CPoint							ptCurPos;							//��ǰλ��

	//������Ϣ
	WORD							wSourceChairID;						//��ʼλ��
	WORD							wTargetChairID;						//����λ��
	UINT							uFlowerID;							//�ʻ���ʶ
	UINT							uFlowerEffectID;					//������ʶ
	bool							bBombEffect;						//��ը��ʶ
};

//�滭ģʽ
enum enImageMode 
{
	enMode_Spread,					//ƽ��ģʽ
	enMode_Centent,					//����ģʽ
	enMode_ElongGate,				//����ģʽ
};

//��˵��
class CClientKernelSink;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ���
class GAME_FRAME_CLASS CGameFrameView : public CWnd, public IGameFrameView
{
	//��Ԫ����
	friend class CGameFrameDlg;
	friend class CClientKernelSink;

	//λ�ñ���
protected:
	UINT							m_nAnimeStep;						//�ƶ����
	POINT							m_ptName[MAX_CHAIR];				//����λ��
	POINT							m_ptFace[MAX_CHAIR];				//ͷ��λ��
	POINT							m_ptTimer[MAX_CHAIR];				//ʱ��λ��
	POINT							m_ptReady[MAX_CHAIR];				//׼��λ��

	//�û�����
private:
	WORD							m_wTimer[MAX_CHAIR];				//�û�ʱ��
	tagUserData						* m_pUserItem[MAX_CHAIR];			//�û���Ϣ

	//���ָ��
private:
	void							* m_pReserve;						//����ָ��
	IUserFaceRes					* m_pIUserFaceRes;					//ͷ��ӿ�

	//��ͼ����
private:
	bool							m_bReDraw;							//�Ƿ��ػ�
	CSize							m_ImageSize;						//�����С
	CImage							m_ImageBuffer;						//����λͼ
	const bool						m_bDoubleBuf;						//�����ͼ
	const UINT						m_uColorCount;						//����ɫ��
	CThreadDraw						* const m_pThreadDraw;				//��ͼ��Ϣ

	//ֻ������
public:
	static const int				m_nXFace;							//ͷ��߶�
	static const int				m_nYFace;							//ͷ����
	static const int				m_nXTimer;							//��ʱ����
	static const int				m_nYTimer;							//��ʱ����
	static const int				m_nXBorder;							//��ܿ��
	static const int				m_nYBorder;							//��ܸ߶�

	//��չ����
private:
	CFlowerEffectControl			m_FlowerEffectControl[MAX_CHAIR_NORMAL];//�ʻ��ؼ�
	tagFlowerEffectInfo				m_FlowerInfo[MAX_FLOWER_COUNT];		//�ʻ���Ϣ
	int								m_nFlowerCount;						//�ʻ���Ŀ

	//��������
public:
	//���캯��
	CGameFrameView(bool bDoubleBuf=true ,UINT uColorCount=16, CThreadDraw * pThreadDraw=NULL);
	//��������
	virtual ~CGameFrameView();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView()=NULL;
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight)=NULL;
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight)=NULL;

	//���ܺ���
public:
	//��ȡʱ��
	WORD GetUserTimer(WORD wChairID);
	//��ȡ���
	const tagUserData * GetUserInfo(WORD wChairID);
	//���½���
	void UpdateGameView(const CRect * pRect);
	//��ȡ����
	bool GetGameViewImage(CImage & ImageBuffer, int nXPos, int nYPos, int nWidth, int nHeight);
	//����λ��
	void SetFlowerControlInfo(WORD wChairID,int nXPos, int nYPos, int nWidth=BIG_FACE_WIDTH, int nHeight=BIG_FACE_HEIGHT, UINT nAnimeStep=30);
	//�滭�ʻ�
	void ShowFlower(WORD wSourceChairID, WORD wTargetChairID, UINT uFlowerID, UINT uFlowerEffectID, bool bBombEffect=false);//?

	//��ͼ����
public:
	//�滭����
	void DrawViewImage(CDC * pDC, CSkinImage & SkinImage, enImageMode ImageMode);
	//�滭ͷ��
	void DrawUserFace(CDC * pDC,const tagUserData * pUserData, int nXPos, int nYPos, bool bOffLine);
	//�滭ͷ��
	void DrawCustomFace(CDC * pDC,const tagUserData * pUserData, int nXPos, int nYPos);
	//�滭׼��
	void DrawUserReady(CDC * pDC, int nXPos, int nYPos);
	//�滭ʱ��
	void DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
	//͸���滭
	void AlphaDrawImage(CDC * pDesDC, int nXDes, int nYDes, int nDesWidth, int nDesHeight, CDC * pScrDC, int nXScr, int nYScr, COLORREF crTransColor);
	//͸���滭
	void AlphaDrawImage(CDC * pDesDC, int nXDes, int nYDes, int nDesWidth, int nDesHeight, HBITMAP hBitBmp, int nXScr, int nYScr, COLORREF crTransColor);

	//�ڲ�����
private:
	//���ý���
	void ResetData();
	//���ýӿ�
	bool SetUserFaceRes(IUnknownEx * pIUnknownEx);
	//����ʱ��
	bool SetUserTimer(WORD wChairID, WORD wTimer);
	//�����û�
	bool SetUserInfo(WORD wChairID, tagUserData * pUserData);

	//��Ϣӳ��
protected:
	//�滭����
	afx_msg void OnPaint();
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//������Ϣ
	afx_msg void OnDestroy();
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif