#pragma once

#include "CMD_LLShow.h"

#define		WAY_UP			0
#define		WAY_RIGHT		1
#define		WAY_DOWN		2
#define		WAY_LEFT		3

class CLogic
{
public:
	CLogic(void);
	~CLogic(void);
	
	// �жϾ����Ƿ��ཻ
	bool IsLink(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W], CPoint first, CPoint second,CArrayTemplate<CPoint> *v,CPoint *ipoint1,CPoint *ipoint2);
	// ����
	bool FindMap(CArrayTemplate<CPoint> *v,BYTE  chess[11][19],CPoint point,  int way, bool isFirst);
	// λ���Ƿ�Ϸ�
	bool InChess(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W],int w, int h);
	// �ҳ�����
	bool AssertHaveSame(CArrayTemplate<CPoint> * f, CArrayTemplate<CPoint> * s, CPoint *same);
	// ��������
	void ShortPath(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W],CArrayTemplate<CPoint> * r, CPoint one , CPoint two, CPoint* ipoint);
	// ������ʾ
	bool LinkPrompt(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W], CPoint * first, CPoint * end);
	// ��������
	void Realign(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W]);


	//����ȼ�
	LPCTSTR AssertUserClass(LONG score);
	//
	float AssertUserPer(LONG score);
	//����ҳ
	void OpenUrl(CString url);
	// �����Ƿ�Ϊ��
	bool ChessEmpty(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W]);
};
