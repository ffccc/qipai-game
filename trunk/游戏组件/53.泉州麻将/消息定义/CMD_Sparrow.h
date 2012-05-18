#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#define TAX_MIN_NUM	10 //Ҫ��˰�����ٵ�
#define KIND_ID						ID_GAME_QZ_MJ									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("Ȫ���齫")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GS_FREE								//����״̬
#define GS_MJ_BEFOREPLAY			(GS_PLAYING+1)						//��Ϸǰ״̬
#define GS_MJ_PLAY					(GS_PLAYING+2)						//��Ϸ״̬



//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����齫
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//����״̬
enum enGoldSwim
{
	GoldSwim_None,					//������״̬
	GoldSwim_Single,				//�ν�״̬
	GoldSwim_Double,				//˫��״̬
	GoldSwim_Three					//����״̬
};

//��������
#define	MAX_LEFT_COUNT				16									//��������

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����齫
#define SUB_S_OPERATE_NOTIFY		103									//������ʾ
#define SUB_S_OPERATE_RESULT		104									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_OPENDOOR				107									//��������
#define SUB_S_OPENGOLD				108									//��������
#define SUB_S_REPAIRFLOWER			109									//��������
#define SUB_S_END_FLOWER			110									//��������
#define SUB_S_GOLD_SWIM				111									//����״̬

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG							lCellScore;							//��Ԫ����
	WORD							wSiceCount;							//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	//״̬����
	BYTE							cbActionCard;						//�����齫
	BYTE							cbActionMask;						//��������
	BYTE							cbLeftCardCount;					//ʣ����Ŀ

	//������Ϣ
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����齫
	BYTE							cbDiscardCount[4];					//������Ŀ
	BYTE							cbDiscardCard[4][55];				//������¼

	//�齫����
	BYTE							cbCardCount;						//�齫��Ŀ
	BYTE							cbCardData[14];						//�齫�б�

	//����齫
	BYTE							cbWeaveCount[4];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[4][5];				//����齫
};

//����״̬
struct CMD_S_OpenDoor
{
	WORD							wDiceCount;							//���Ӵ�С
	BYTE							cbDoorCard;							//���ƴ�С
	WORD							wOpenGoldUser;						//�������
};

//����״̬
struct CMD_S_OpenGold
{
	WORD							wDiceCount;							//���Ӵ�С
	BYTE							cbGoldCard[4];						//�����齫
	BYTE							cbKeyGoldCard;						//�ؼ��齫
	BYTE							cbFlowerCard[8];					//�����齫
	
};

//����״̬
struct CMD_S_RepairFlower
{

	WORD							wChairID;							//�������
	BYTE							cbHandFlower;						//���л���
	BYTE							cbCardData;							//�����齫
	BYTE							cbCardPos;							//����λ��

};

//��Ϸ��ʼ 
struct CMD_S_GameStart
{	
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�	
	BYTE							cbCardData[17];						//�齫�б�
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����齫
};

//�����齫
struct CMD_S_SendCard
{	
	BYTE							cbCardData;							//�齫����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	bool							bRepairFlower;						//��������	
	bool							bGameSatusPlay;						//��Ϸ״̬��
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����齫
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����齫
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbChiHuCard;						//�Ժ��齫
	WORD							wProvideUser;						//�����û�
	bool							wWinner[4];							//Ӯ��
	LONG							lGameScore[4];						//��Ϸ����
	WORD							wChiHuKind[4];						//��������
	BYTE							cbCardCount[4];						//�齫��Ŀ
	BYTE							cbCardData[4][17];					//�齫����
	char							strEnd[100];
	int								lGameTax;
	bool							mbChui[4];							//�����û�
	BYTE							bFlowerCount[4];					//�������
	BYTE							bGoldCount[4];						//���Ƹ���
	BYTE							bGangCount[4];						//���Ʒ��� 
	BYTE							bGoOnBankerTime;					//��ׯ����
	bool							bAllSidesCard[4];					//��������
	bool							bAllSeasonCard[4];					//�����ﶬ
	bool							bAllPlantCard[4];					//÷�����
	LONG							lCellScore;							//�׷ִ�С
};

//����״̬
struct CMD_S_GoldSwim
{
	enGoldSwim						GoldSwim[4];						//����״̬	
};


//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			2									//�����齫
#define SUB_C_OPENDOOR				3									//��������
#define SUB_C_OPENGOLD				4									//��������
#define SUB_C_REPAIRFLOWER			5									//��������

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�齫����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����齫
};

//��������
struct CMD_C_RepairFlower
{
	WORD							wChairID;							//�������
	BYTE							cbCurrentCard;						//��ǰ�齫
};


//////////////////////////////////////////////////////////////////////////

#endif