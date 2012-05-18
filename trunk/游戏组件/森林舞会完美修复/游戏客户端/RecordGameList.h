#ifndef RECORD_GAME_LIST_HEAD_FILE
#define RECORD_GAME_LIST_HEAD_FILE

#define HISTORY_RECORD_SHOW			10									//ÿҳ��ʾ����ʷ��¼��
#pragma once

#include "Stdafx.h"

//��Ϸ��¼
class CGameRecord /*: public CGameWnd*/
{
	//��ʷ����
public:
	WORD							m_wPageNO;							//��ǰ��ʾҳ
	CArrayTemplate<tagHistory>		m_HistoryArray;						//���������ʷ��¼
	WORD							m_wHistoryWinCount[3];				//��ʷ��¼ׯ�к͵þ���

	BYTE                            m_Cardsot[12];



public:

	//��������
public:
	//���캯��
	CGameRecord();
	//��������
	virtual ~CGameRecord();

	//���ܺ���
public:
	void OnDraw(SURFACE* psurfTarget,SURFACE* psurfSource,int nx,int ny);

public:

   void  DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource1,SURFACE* psurfSource2, LONG lNumber, INT nXPos, INT nYPos, bool isleft,BYTE cbNumCount);

   //�滭����
   void DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource, SURFACE* psurfSource2,LONG lNumber, INT nXPos, INT nYPos, BYTE cbNumCount);
   //�滭����
   void DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource, LONG lNumber, INT nXPos, INT nYPos);

	//��ʷ�������
	void SetHistoryResult(tagHistory[], WORD Count);

	//��ʷ�������
	void AddHistoryResult(BYTE cbPlayerValue);
};

//////////////////////////////////////////////////////////////////////////

#endif