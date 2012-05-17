#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//�����궨��

#define KIND_ID						1171							//��Ϸ I D
#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("��ˮ�ڿ�")						//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//Add by 20071129
#define LEFT_CARD_NUM	4 //����
#define ONE_USER_GET_CARD_NUM	14//ÿ�˵���
#define ZHUANG_CARD_NUM   (LEFT_CARD_NUM+ONE_USER_GET_CARD_NUM)       //ׯ������
#define ALL_CARD_NUM    (ONE_USER_GET_CARD_NUM*GAME_PLAYER + LEFT_CARD_NUM)//������ 46 52

//0x24,0x25,0x26,0x27,0x28
#define RED_4   0x24//����4
#define MAX_RED_LENGTH  4//��8 

#define CARD_VALUE_THREE 16
#define CARD_VALUE_K	0x0D
//End add

//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ
#define GS_WK_SCORE					GS_PLAYING							//�з�״̬
#define GS_WK_PLAYING				GS_PLAYING+1						//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_LAND_SCORE			101									//�з�����
#define SUB_S_GAME_START			102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//��������
	LONG							lBaseScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[GAME_PLAYER/*3*/];						//�з���Ϣ
	BYTE							bCardData[ONE_USER_GET_CARD_NUM/*17*/];						//�����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wLandUser;							//�������
	WORD							wBombTime;							//ը������
	LONG							lBaseScore;							//��������
	BYTE							bLandScore;							//��������
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[LEFT_CARD_NUM/*3*/];						//�����˿�
	BYTE							bCardData[ZHUANG_CARD_NUM /*20*/];						//�����˿�
	BYTE							bCardCount[GAME_PLAYER/*3*/];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[ZHUANG_CARD_NUM/*20*/];					//�����б�
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[ONE_USER_GET_CARD_NUM/*17*/];						//�˿��б�
};

//�û��з�
struct CMD_S_LandScore
{
	WORD							bLandUser;							//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bLandScore;							//�ϴνз�
	BYTE							bCurrentScore;						//��ǰ�з�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD				 			wLandUser;							//�������
	BYTE							bLandScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[LEFT_CARD_NUM/*3*/];						//�����˿�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[ZHUANG_CARD_NUM/*20*/];						//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;							//��Ϸ˰��
	LONG							lGameScore[GAME_PLAYER/*3*/];						//��Ϸ����
	BYTE							bCardCount[GAME_PLAYER/*3*/];						//�˿���Ŀ
	BYTE							bCardData[ALL_CARD_NUM];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_LAND_SCORE			1									//�û��з�
#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������

//�û��з�
struct CMD_C_LandScore
{
	BYTE							bLandScore;							//��������
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[ZHUANG_CARD_NUM/*20*/];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////

#endif