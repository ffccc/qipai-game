#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

const KillFishTask g_KillFishTask[] = 
{
	{ 90, 0, 15, 100},
	{120, 0, 20, 150},
	{180, 0, 30, 200},

	{ 90, 1, 10, 120},
	{120, 1, 20, 180},
	{180, 1, 30, 230},
	
	{ 90, 2, 10, 150},
	{120, 2, 15, 180},
	{180, 2, 20, 240},
	
	{ 90, 3, 10, 120},
	{120, 3, 15, 260},
	{180, 3, 20, 300},
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CGameLogic
{
    //��������
protected:
   static const BYTE m_cbTaskProbable[3][100];
   //static const KillFishTask m_KillFishTask[8];

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

public:
	inline int FishGoldAndBulletToScore(int nFishGold) { return nFishGold*1; }

	inline int ScoreToFishGoldAndBullet(int nScore) { return nScore/1; }

	inline int CannonConsumeBullet(int nCannonStyle) { return nCannonStyle; }

	/*inline */int FishGoldByStyle(int nFishStyle) ;

	int GetTaskByGroup(int nGroup);

	int GetExp(int nCannonStyle)
	{
		switch (nCannonStyle)
		{
		case 0: return rand()%2;
		case 1: return rand()%2;
		case 2: return rand()%3;
		case 3: return rand()%3;
		case 4: return rand()%2+1;
		case 5: return rand()%2+1;
		case 6: return rand()%3+1;
		default: return rand()%2;
		}
	}

	int GetProbability(int nCannonStyle, int nFishStyle)
	{
		switch (nCannonStyle)
		{
		case 0:		/// 1����
			{
				switch (nFishStyle)
				{
				case  0: return 1200;	///  1���� 1���  	1000
				case  1: return 330	;	///  2���� 2���  	500	
				case  2: return 200	;	///  3���� 3���  	330	
				case  3: return 155	;	///  4���� 4���  	250	
				case  4: return 110	;	///  5���� 5���  	200	
				case  5: return 50	;	///  6���� 10��� 	100	
				case  6: return 25	;	///  7���� 20��� 	50	
				case  7: return 16	;	///  8���� 30��� 	33	
				case  8: return 12	;	///  9���� 40��� 	25	
				case  9: return 10	;	/// 10���� 50��� 	20	
				case 10: return 7	;	/// 11���� 70��� 	14	
				case 11: return 3	;	/// 12���� 100���	10	
				default: return 0	;					  	
				}
			}
		case 1:		/// 2����
			{
				switch (nFishStyle)
				{
				case  0: return 1200;	///  1���� 1���
				case  1: return 600;	///  2���� 2���
				case  2: return 370	;	///  3���� 3���
				case  3: return 290	;	///  4���� 4���
				case  4: return 180	;	///  5���� 5���
				case  5: return 160	;	///  6���� 10���
				case  6: return 70	;	///  7���� 20���
				case  7: return 60	;	///  8���� 30���
				case  8: return 35	;	///  9���� 40���
				case  9: return 23	;	/// 10���� 50���
				case 10: return 9	;	/// 11���� 70��� 
				case 11: return 6	;	/// 12���� 100���
				default: return 0	;
				}
			}
		case 2:		/// 3����
			{
				switch (nFishStyle)
				{
				case  0: return 1200;	///  1���� 1���
				case  1: return 730	;	///  2���� 2���
				case  2: return 430	;	///  3���� 3���
				case  3: return 320	;	///  4���� 4���
				case  4: return 210	;	///  5���� 5���
				case  5: return 150	;	///  6���� 10���
				case  6: return 85	;	///  7���� 20���
				case  7: return 78	;	///  8���� 30���
				case  8: return 45	;	///  9���� 40���
				case  9: return 29	;	/// 10���� 50���
				case 10: return 10	;	/// 11���� 70��� 
				case 11: return 7	;	/// 12���� 100���
				default: return 0	;
				}
			}
		case 3:		/// 4����
			{
				switch (nFishStyle)
				{
				case  0: return 1200;	///  1���� 1���
				case  1: return 650	;	///  2���� 2���
				case  2: return 500	;	///  3���� 3���
				case  3: return 405	;	///  4���� 4���
				case  4: return 300	;	///  5���� 5���
				case  5: return 220	;	///  6���� 10���
				case  6: return 160	;	///  7���� 20���
				case  7: return 120	;	///  8���� 30���
				case  8: return 70	;	///  9���� 40���
				case  9: return 32	;	/// 10���� 50���
				case 10: return 11	;	/// 11���� 70��� 
				case 11: return 9	;	/// 12���� 100���
				default: return 0	;
				}
			}
		case 4:		/// 5����
			{
				switch (nFishStyle)
				{
				case  0: return 1200;	///  1���� 1���
				case  1: return 750;	///  2���� 2���
				case  2: return 600	;	///  3���� 3���
				case  3: return 450	;	///  4���� 4���
				case  4: return 330	;	///  5���� 5���
				case  5: return 240	;	///  6���� 10���
				case  6: return 190	;	///  7���� 20���
				case  7: return 130	;	///  8���� 30���
				case  8: return 80	;	///  9���� 40���
				case  9: return 50	;	/// 10���� 50���
				case 10: return 14	;	/// 11���� 70��� 
				case 11: return 11	;	/// 12���� 100���
				default: return 0	;
				}
		case 5:		/// 6����
			{
				switch (nFishStyle)
				{
				case  0: return 1200;	///  1���� 1���
				case  1: return 800;	///  2���� 2���
				case  2: return 700;	///  3���� 3���
				case  3: return 550	;	///  4���� 4���
				case  4: return 440	;	///  5���� 5���
				case  5: return 330	;	///  6���� 10���
				case  6: return 220	;	///  7���� 20���
				case  7: return 170	;	///  8���� 30���
				case  8: return 90	;	///  9���� 40���
				case  9: return 50	;	/// 10���� 50���
				case 10: return 16	;	/// 11���� 70��� 
				case 11: return 13	;	/// 12���� 100���
				default: return 0	;
				}
			}
		case 6:		/// 7����
			{
				switch (nFishStyle)
				{
				case  0: return 1200;	///  1���� 1���
				case  1: return 850;	///  2���� 2���
				case  2: return 750 ;	///  3���� 3���
				case  3: return 600	;	///  4���� 4���
				case  4: return 500	;	///  5���� 5���
				case  5: return 400	;	///  6���� 10���
				case  6: return 300	;	///  7���� 20���
				case  7: return 200	;	///  8���� 30���
				case  8: return 100	;	///  9���� 40���
				case  9: return 70	;	/// 10���� 50���
				case 10: return 20	;	/// 11���� 70��� 
				case 11: return 15	;	/// 12���� 100���
				default: return 0	;
				}
			}
		default: return 0;
			}

			return 0;
		}
	}
  //inline int GetCannonMFValue(int nCannonStyle)
  //{ 
  //  switch(nCannonStyle)
  //  {
  //  case 0: return 500;
  //  case 1: return 600;
  //  case 2: return 700;
  //  case 3: return 750;
  //  case 4: return 800;
  //  case 5: return 900;
  //  case 6: return 950;
  //  default:  return 0;
  //  }
  //}

  //inline int GetFishDropValue(int nFishStyle) 
  //{ 
  //  switch(nFishStyle)
  //  {
  //  case 11: return 10;
  //  case 10: return 15;
  //  case 9: return 30;
  //  case 8: return 50;
  //  case 7: return 55;
  //  case 6: return 60;
  //  case 5: return 65;
  //  case 4: return 70;
  //  case 3: return 85;
  //  case 2: return 90;
  //  case 1: return 95;
  //  case 0: return 100;
  //  case 12: return 0;
  //  default:  return 0;
  //  }
  //}
};

//////////////////////////////////////////////////////////////////////////

#endif