#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						185									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("ɭ�����")						//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_BET_TIME				106									//��עʱ�俪ʼ
#define SUB_S_PLACE_TIMERTYPE		108									//ʱ������
#define TIME_PLACE_JETTON		    20									//��עʱ��
#define TIME_GAME_SENDPAI			30									//���Ƶ���һ�ֿ�ʼ��ʱ��

#define SUB_S_SUPPERUSER			454									//��������
#define SUB_S_GAME			        110									//��������
#define SUB_S_PLU			        111									//��������
//���»���
struct CMD_S_Game
{
	LONG							GameBi;							//���Ӻ���

};
//���»���
struct CMD_S_SUPPERUSER
{
	bool							supuser;							//���Ӻ���

};
//���»���
struct CMD_S_ChangeApplyScore
{
	WORD							wChairID;							//���Ӻ���
	INT64							lGold;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	WORD							wBankerCount;						//ׯ�Ҿ���
	INT64							lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wChairID;							//���Ӻ���
	CHAR							szAccount[32];					    //�������
	CHAR							szName[32];					        //��������ǳ�
	DWORD							dwUserID;							//�������
	INT64							lGold;								//��ҽ��
	bool							bApplyBanker;						//�����ʶ
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wChairID;							//���Ӻ���
	WORD							wBankerCount;						//ׯ�Ҿ���
	LONG							lBankerWinScore;					//ׯ�ҷ���
	LONG							lBankerMaxLoss;						//ׯ������⸶
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	LONG                            lBankerScore;

	//�ҵ���ע
	INT64							lMyMaxStake;						//�����ע
	INT64							lMaxStakes[INDEX_MAX];				//ÿ������������ע��
};

//��ʷ��¼
struct tagHistory
{
	BYTE							cbPlayerValue;						//����

	tagHistory()
	{
		cbPlayerValue = 0;


	};

	tagHistory(BYTE acbPlayerValue )
	{
		cbPlayerValue = acbPlayerValue;

	};

};
//ʱ��״̬
struct CMD_S_TimerTyper
{
	WORD							TimerType;		
    WORD                            Timernum;
};
//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lMyStakes[INDEX_MAX];				//�Լ�����ע��Ϣ
	LONG							lAllStakes[INDEX_MAX];				//�Լ�����ע��Ϣ
	LONG                            lMyGame;                            //���ѵ���Ϸ��
	BYTE							cbCardCount[INDEX_MAX];						//����
	BYTE							cbCardColoct[24];						//��ɫ
	LONG                            cdPrizemoney;                       //�ʽ�
	LONG                            lCellGold  ;                        //��λ���   
	WORD							wHistoryCount;
	tagHistory						HistoryInfo[HISTORY_COUNT];			//��ʷ��¼
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	WORD							wBankerCount;						//ׯ�Ҿ���
	INT64							lCurrentBankerScore;				//ׯ�ҷ���
	INT64							lApplyBankerCondition;				//������ׯ����ͷ���
	WORD							wApplyCountMax;						//������ׯ���������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��ע��Ϣ
	LONG                            cdPrizemoney;                       //�ʽ�
	LONG							lMyStakes[INDEX_MAX];				//�Լ�����ע��Ϣ
	LONG							lAllStakes[INDEX_MAX];				//�Լ�����ע��Ϣ
	LONG                            lMyGame;                            //���ѵ���Ϸ��
	BYTE							cbCardCount[INDEX_MAX];						//����
	BYTE							cbCardColoct[24];						//��ɫ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	WORD							wBankerCount;						//ׯ�Ҿ���
	INT64							lCurrentBankerScore;				//ׯ�ҷ���
	INT64							lApplyBankerCondition;				//������ׯ����ͷ���
	WORD							wApplyCountMax;						//������ׯ���������

	LONG                            lCellGold  ;                        //��λ���                           
	WORD							wHistoryCount;
	tagHistory						HistoryInfo[HISTORY_COUNT];			//��ʷ��¼
};

struct CMD_S_GamePLU
{
	BYTE							cbCardCount[INDEX_MAX];						//����
	BYTE							cbCardColoct[24];						//��ɫ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE							cbCard;						//�˿���Ŀ
	BYTE                            cbCardsite;
	BOOL                            cbWinning;                  //�Ƿ񷢷Ųʽ�;
	LONG                            cbPrize;                    //֧���Ĳʽ��
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�ɼ���¼
	LONG							lMyGameScore[INDEX_MAX];						//�ҵĳɼ�
	LONG                            lPrizemoney;

};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;						//�û�λ��
	BYTE							cbBetArea;						//��������
	LONG							lBetAmount;						//��ע��Ŀ
	LONG							lGame;
	LONG                            cdPrizemoney;//�ʽ�
	LONG							cdAreaStakes[INDEX_MAX];			
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_PlAY_OVER			    88									//����ׯ��
#define SUB_C_CHI                   1235           
#define SUB_C_PlAY_TRUE			    44									//�Ƿ���
#define SUB_C_FANG                  9                                   //������Ϸ��                          
#define SUB_C_Settlement            10                                   //������Ϸ��  

struct CMD_C_Settlement
{
	bool							Settlement;						//������Ϸ��
};
struct CMD_C_FANG
{
	int							cbFang;						//������Ϸ��
};
struct CMD_C_TrueArea
{
	BYTE							cbBetArea;						//����
};

struct CMD_C_CHI
{
	BYTE							cbBetArea;						//��������
};
//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbBetArea;						//��������
	INT64							lBetAmount;						//��ע��Ŀ
};

//����ׯ��
struct CMD_C_ApplyBanker
{
	bool							bApplyBanker;						//�����ʶ
	bool                            bAI;
};

//////////////////////////////////////////////////////////////////////////

#endif