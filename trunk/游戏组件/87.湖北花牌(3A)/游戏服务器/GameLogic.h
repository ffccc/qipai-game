#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"
//////////////////////////////////////////////////////////////////////////
//��������

#define MAX_WEAVE					4									//������
#define MAX_INDEX					22									//�������
#define MAX_COUNT					42									//�����Ŀ
#define MAX_REPERTORY				110									//�����

//////////////////////////////////////////////////////////////////////////
//�߼�����

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//��������

////////////////////////////////////babala////////////////////////////
//������־
#define WIK_NULL					0x00								//û������
#define WIK_DUI 					0x01								//��������
#define WIK_SAN_ZHAO				0x02								//����һ����
#define WIK_SI_ZHAO			    	0x04								//����һ����
#define WIK_TONG_GIVE_UP	    	0x08					        	//��һ�ֵ�����£��������ť����������͵Ĳ�����

#define WIK_SI_TONG     	    	0x10								//��ͳ����
#define WIK_WU_TONG					0x20								//��ͳ����
#define WIK_QI_TA			    	0x40					        	//������һ�������
#define WIK_CHI_HU					0x80							   //��������

//////////////////////////////////////////////////////////////////////////

//������־
#define WIK_LEFT					0x21								//�������
#define WIK_CENTER					0x22								//�г�����
#define WIK_RIGHT					0x24								//�ҳ�����
#define WIK_PENG					0x28								//��������
#define WIK_FILL					0x23								//��������
#define WIK_GANG					0x34								//��������
//#define WIK_CHI_HU					0x40								//�Ժ�����
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//���ƶ���

//С������
#define CHK_NULL					0x00000							     //�Ǻ�����
#define CHK_PING_HU		        	0x00001								//ƽ������

//����
#define CHK_QUAN_QIU_REN			0x00010								//ȫ���˺��ƣ�
#define CHK_MEN_QIAN_QING			0x00020								//��ǰ��
#define CHK_JUE_ZHANG  				0x00040								//����

//���轫�ĺ�������
#define CHK_PENG_PENG				0x01000								//��������
#define CHK_QING_YI_SE				0x02000								//��ɫ���ͣ���������һɫ����һɫ��Ͳһɫ��
#define CHK_FENG_YI_SE				0x04000								//��һɫ���ҽ�
#define CHK_JIANG_JIANG				0x08000								//�������ƣ�����һɫ
#define CHK_QI_DUI  				0x00100								//�߶�
#define CHK_HAO_HUA_QI_DUI  	    0x00200								//�����߶�

//�轫�ĺ�������
#define CHK_QIANG_GANG				0x10000								//����
#define CHK_HAI_DI					0x20000								//��������
#define CHK_GANG_SHANG_KAI_HUA      0x40000                              //���Ͽ���

//�ҽ�Ȩλ
#define CHR_QING_YI_SE				0x0100								//��ɫȨλ����������һɫ����һɫ��Ͳһɫ����һɫ
#define CHR_QUAN_QIU_REN			0x0200								//ȫ��Ȩλ
#define CHR_MEN_QIAN_QIN			0x0400			                	//��ǰ��Ȩλ
#define CHR_JUE_ZHANG               0x0800				                //����Ȩλ

#define CHR_QIANG_GANG				0x1000								//����Ȩλ
#define CHR_HAI_DI					0x2000								//����Ȩλ
#define CHR_GANG_SHANG_KAI_HUA      0x4000                              //���Ͽ���Ȩλ

//��������
struct tagKindItem
{
	BYTE				cbWeaveKind;						//�������
	BYTE				cbCenterCard;						//�����˿�����
	BYTE				cbCardIndex[5];						//�˿�����
	BYTE                cbCardCount;                        //�˿�����
};

//�������
struct tagWeaveItem
{
	BYTE				cbWeaveKind;						//�������
	BYTE				cbCenterCard;						//�����˿�
	BYTE				cbPublicCard;						//������־
	WORD				wProvideUser;						//��Ӧ�û�
};

//���ƽ��
struct tagChiHuResult
{
	WORD				wChiHuKind;							//�Ժ�����
	WORD				wChiHuRight;						//����Ȩλ
	WORD				bChiHuUser;							//�Ժ����
};

//���ƵĽ��
struct tagTongResult
{
	BYTE				cbCardCount;						//���ƵĴ���
	BYTE				cbCardData[6];						//ÿ������Ӧ�������Ƶ�����
};

struct tagChiChuItem
{
	BYTE                 cbCenterCard[8];               //8����ϵ�������ֵ
	BYTE                 cbWeaveKind[8];                //8����ϵĲ�������
	BYTE                 cbYaKou[2];                    //һ��Ѿ�ڵ�ֵ
	BYTE                 cbHuScore;                     //���Ƶķ���
	BYTE                 cbGoldCard;                    //���Ƶ�������
};

//��������
struct tagAnalyseItem
{
	BYTE				 cbCardEye[2];						//Ѿ������
	BYTE				 cbWeaveKind[8];						//�������
	BYTE				 cbCenterCard[8];					//�����˿�
	WORD                 cbPoint;                            //����Ƶ���ѵ���
	BYTE                 cbGoldCard;                         //ѡ��������

	//����ʱ�õ���Ϣ
	BYTE				 cbGoldPoint[3];					    //�������������������ķ���
	BYTE				 cbKindPoint[8];	     				//ÿһ�ֺ��������ÿһ�������ĵ���
};

//���ƽ��
struct tagHuCardResult
{
	bool                            IsHu;                               //�Ƿ���Ժ���
	BYTE                            HuScore;                            //���Ƶķ���
	WORD                            bHuPoint;                           //����
	BYTE                            bRealGold;                          //���� 
	tagAnalyseItem                  AnalyseItem;                        //����ʱ������
};

//////////////////////////////////////////////////////////////////////////
//����˵��
typedef CArrayTemplate<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//��ӡ������Ϣ���˵��
//���������˼���ǣ�һ�ν���ص���Ϣ��ӡ��ָ���Ŀͻ���
//ʹ�õĹ��̣�
//1������BeginPrint����������ַ����е���Ϣ
//2�����ö����صĴ�ӡ����������ص���Ϣ���浽�ַ�����
//3������EndPrint���������ַ����е���Ϣ���͵��ͻ���
class CDebugMessage
{
private:
	//�����л�������������Ϣ
	static CString m_strCardsMessage[22];
	//��Ϸ���ӿ�ܶ����������ͱ������Ϣ���ͻ���
	ITableFrame		*m_pITableFrame;
	//���汾�ִ�ӡ����Ϣ
	CString m_strMessage;
	////�ڱ��ִ�ӡ�����У���������������ֻ����һ���û����д�ӡ
	//CThreadLock m_Lock;
public:
	//���캯������������
	CDebugMessage();
	~CDebugMessage();

public:
	//��ʼ����ӡ�����е��������
	void InitDebug(ITableFrame	*pITableFrame);

	//��ʼ��ӡ������Ϣ
	void BeginPrint();
	//������ӡ������Ϣ
	void EndPrint(WORD wChairID);

	//��ӡ�û����е�����Ϣ
	void PrintCardMessage(BYTE cbCardIndex[MAX_INDEX]);

	//��ӡ�û���ϵ�����Ϣ
	void PrintWeaveItemsMessage(tagWeaveItem WeaveItem[], BYTE cbWeaveCount);

	//��ӡ�û������Ƶ����������Ϣ
	void PrintKindItemsMessage(tagKindItem KindItem[],BYTE cbKindItemCount);

	//��ӡ�û������ƺϷ��������Ϣ
	void PrintValidKindItemsMessage(CAnalyseItemArray & AnalyseItemArray);

	//��ӡ����֮ǰ�������ĵ����������Ƶ��ܵ������������ܵ���
	void PrintValidKindItemPointBeforeMessage(tagAnalyseItem *pAnalyseItem,BYTE cbWeavePoint,BYTE cbHandPoint);

	//��ӡ����֮��������ĵ����������Ƶ��ܵ������������ܵ���
	void PrintValidKindItemPointAfterMessage(tagAnalyseItem *pAnalyseItem,BYTE cbWeavePoint,BYTE cbHandPoint);
	
	//��ӡ�����ϵ���Ϣ
	void PrintHuCardMessage(tagAnalyseItem *pAnalyseItem);

private:
	BYTE SwitchToCardIndex(BYTE cbCardData);
};

////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//�˿�����

public:
	CDebugMessage m_Debug;

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ƺ���
public:
	//�����˿�
	void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);

	//��������
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//����˿�
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[]);

	//�ȼ�����
public:

	//////////////babala
	//�õ��˿��Ƶȼ�
	BYTE GetCardRank(BYTE cbCardData);

	////////////////babala 11_15///////////////

	//�����ȼ�
	BYTE GetUserActionRank(BYTE cbUserAction);
	//�������������������У��ǻ��Ƶı��浽��������������
	void SaveHuaCard(BYTE cbCardData[],BYTE bCardCount,BYTE cbHuaCardIndex[]);
	////////////////babala 11_15
	//�����ж�
public:
	////////////////babala///////////////
	BYTE EstimateDuiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbSiTong[],BYTE cbSiTongCount,BYTE cbCurrentCard);

	BYTE EstimateZhaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbSiTong[],BYTE cbSiTongCount, BYTE cbCurrentCard);

	//�����ж�
public:
	//babala 11-31
	//�����ֵ
	BYTE Abs(BYTE cbFirst,BYTE cbSecond);
	//bool IsYaKou(const BYTE cbCardIndex[],BYTE &FirstIndex,BYTE &SecondeIndex);
	bool IsYaKou(BYTE cbCardIndex[],BYTE &FirstIndex,BYTE &SecondeIndex);

	//�����˿������ݣ��õ����Ʊ��浽����������
	//                  �˿�������        ����             ������������
	BYTE GetHuaCard(BYTE cbCardData[],BYTE bCardCount, BYTE cbHuaCardIndex[]);
	/////////////////////////////////////�����������/////////////////////////////////

	//������ͳ             �˿�������               �˿˸���             ��ͳ���     �Ѿ����ڵ���ͳ��
	BYTE  AnalyseSiTong(BYTE cbCardIndex[MAX_INDEX],  tagTongResult &SiTongResult);

	//������ͳ
	BYTE  AnalyseWuTong(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,tagTongResult &WuTongResult);

	//��������
	BYTE AnalyseHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,BYTE cbCurrentCard, BYTE HuaCardInex[], BYTE WeavHuaIndex[],tagHuCardResult &ChiHuResult);
	
	//����������еĲ���
	BYTE AnalyseWeaveTong(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[],BYTE cbItemCount,tagTongResult &WeaveTongResult);
	////////////////////babala////////////////////////////////////

	//ת������
public:
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT], BYTE cbHuaCardIndex[]);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//�ڲ�����
private:
	//����һ���������ͺͲ��������ƣ��õ������Ƶ�����
	BYTE GetWeaveIndex(BYTE cbWeaveKind,BYTE cbCenterCardIndex,BYTE cbCardIndex[]);
	//У������,����һ�������ݣ���һ��������
	bool CheckHuPoint(BYTE cbCardData,BYTE cbCardIndex);
	//�����˿�
	bool AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
};

//////////////////////////////////////////////////////////////////////////

#endif