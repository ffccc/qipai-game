#pragma once
#ifndef CMD_DICECASTANIMATION_HEAD_FILE
#define CMD_DICECASTANIMATION_HEAD_FILE
#include "StdAfx.h"
#include "DxGraphics.h"
#define NOT_ACTIVE			 0
#define NO_ANIMATION		 1
#define FIRST_STAGE			 2
//#define SLOWING_STAGE		 3
//#define STOP_STAGE			 4
#define NO_ANIMATION_WAIT	 5


#define	DICE_LEFT_MOVE	 	 0
#define DICE_RIGHT_MOVE		 1

// 骰子投掷效果
class DiceCastAnimation{
public:
	cTexture m_DiceTexture;
	D3DXMATRIX m_MatrixTransform;
	USHORT m_usRandIndex_W;
	long m_lDownCount ;

	USHORT m_usXShift, m_usYShift;
	//float m_fXScale, m_fYScale;

	float m_fRotationAngle, m_fAngleReduceAcceler;
	BYTE m_cbStatus;
	BYTE m_cbResult;
//	long m_lStopCount;

	DiceCastAnimation();
	~DiceCastAnimation()
	{

	}
	void Draw(cGraphics *pGraphics, DWORD ElapseTime, BYTE cbMoveFlag);
	//初始化棋盘
	void InitDiceCast(cGraphics *pGraphics);
	void Reset(BYTE cbMoveFlag);

	void CleanUp(void);
	
};
#endif