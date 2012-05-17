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
    unsigned long GetFishNetColor(int nCannonStyle);
    float GetFishNetScale(int nCannonStyle);
    int GetFishBonus(int nFishStyle);
    int GetFishRadius(int nFishStyle);
    unsigned long GetFireInterval(int nBulletStyle);

	/// ������ײ���
    bool CollisionShark(const Point &pos1, const Point &pos2, int nRadius1, int nRadius2);
	/// �ӵ�ײ����
    bool CollisionFish(const Point &pos1, const Point &pos2, int nRadius1, int nRadius2);
	///  ������
    bool Collision(const Point &pos1, const Point &pos2, int nRadius1, int nRadius2);
};

//////////////////////////////////////////////////////////////////////////

#endif