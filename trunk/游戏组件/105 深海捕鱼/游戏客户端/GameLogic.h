#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CGameLogic
{
    //��������
public:
    //���캯��
    CGameLogic();
    //��������
    virtual ~CGameLogic();

public:
    float GetBulletRadius(int nCannonStyle);
    float GetFishNetColor(int nCannonStyle);
    int GetFishBonus(int nFishStyle);
    unsigned long GetFireInterval(int nBulletStyle);

};

//////////////////////////////////////////////////////////////////////////

#endif