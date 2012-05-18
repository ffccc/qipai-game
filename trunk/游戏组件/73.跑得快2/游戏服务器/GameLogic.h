#pragma once

//////////////////////////////////////////////////////////////////////////
#include "..\��Ϣ����\CMD_QuickRun.h"

//���ڱȽϴ�С�����飬�ܵÿ��дӴ�С����2��1��K��Q��J��10��....
//                   0, A, 2, 3, 4, 5, 6, 7, 8, 9,10, J, Q, K,King
const int compare[]={0,12,13, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,14};
//���ڻ�ԭ��С������
const int uncompare[]={0, 3, 4, 5, 6, 7, 8, 9,10,11,12,13, 1, 2};

//��Ϸ�߼���
class CGameLogic
{
	//��������
private:
	//ÿ���˵���
	CArrayTemplate<struct tagPisa,struct tagPisa&> m_PisaArray[GAME_PLAYER];
	//�������һ�γ��ƵĽ��
	struct tagSetOutPisa m_LastPisa;
	//�Ƿ��һ�γ���
	BOOL m_bFirst;
	//������Ϣ
	TCHAR m_chErrorMsg[100];

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//������ĳ��Ƽ�¼
	void EmptyLastCardInfo();
	//�Ƿ�ǰ��Ҿ������һ�γ��Ƶ���ң����Ѿ�����һȦ��
	BOOL IsUserBegin(int index);
	//��ǰ���Ƿ����������Ҫ��
	BOOL IsBiggerThanLastCard(const struct tagSetOutPisa *pisa);
	//��ǰ���Ƿ�Ϸ����ƹ���
	BOOL IsRegular(const struct tagSetOutPisa *pisa);
	//��ȡ�Ƚϴ�С�Ĵ�����Ϣ
	char *GetCompareError();
	//�Ե�ǰ�Ƹ����������ƣ�����ֵΪ������Ƿ�����
	BOOL SaveLastCard(const struct tagSetOutPisa *pisa);
	//��ʼһ������Ϸ������ֵ�Ƿ�Ƭ3���ڵ���������
	int StartANewGame();
	//��ȡÿ����ҵ���
	void GetInitPisa(int nIndex,struct tagSetInitPisa *pPisa);
	//��ȡ��ҵ�ǰʣ����Ӧ�˵ı���
	int GetPlayerLeftCard(int nIndex);
	//�ڲ�����
private:
	//�ж������Ƿ�Ϊը�������ص�Count�����������ص�PisaCount�������ֵ
	BOOL IsBomb(const struct tagSetOutPisa *pisa,int *Count,struct tagPisa *PisaPoint);
	//�ж������Ƿ�Ϊ���ԣ����ص�PisaCount�������ֵ
	BOOL IsContinue2(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint);
	//�ж������Ƿ�Ϊ����ͬ�ţ����ص�PisaCount�������ֵ
	BOOL IsContinue3(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint);
	//�ж������Ƿ�Ϊ˳�ӣ����ص�PisaCount�������ֵ��bSameColor��ʾ�Ƿ�Ϊͬ��˳
	BOOL IsStraight(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint,BOOL *bSameColor);
	//�ж������Ƿ�Ϊ�����������ص�PisaCount�������ֵ
	BOOL Is3And2(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint);
	//�ж������Ƿ�Ϊ�������ͣ����ص�PisaCount�������ֵ
	BOOL IsButterfly(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint);
	//���ú���
	BOOL _IsContinue(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint,int offest);
	//�Ƿ�ͬʱ����3��K
	BOOL IsBothHave3AndK(const struct tagSetOutPisa *pisa);
	//��ȡ��������ֵ��bEnd��ʾ�Ƿ�ͨ�����ػ���
	void GetMaxPoint(const struct tagSetOutPisa *pisa,struct tagPisa *PisaPoint);
	//�ж�����ֵ�Ƿ�����
	BOOL IsNeighbor(int index,int index2);
};
