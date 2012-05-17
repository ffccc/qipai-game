#ifndef CMD_RUNFAST_HEAD_FILE
#define CMD_RUNFAST_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#ifdef VIDEO_GAME
	#define KIND_ID						516									//��Ϸ I D
#else
	#define KIND_ID						112									//��Ϸ I D
#endif

#define GAME_PLAYER					2									//��Ϸ����
#define GAME_NAME					TEXT("����")						//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_RF_FREE					GS_FREE								//�ȴ���ʼ
#define GS_RF_PLAYING				GS_PLAYING							//�з�״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			101									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				102									//�û�����
#define SUB_S_PASS_CARD				103									//��������
#define SUB_S_GAME_END				104									//��Ϸ����
#define SUB_S_AUTOMATISM			105

//����й��¼�
struct CMD_S_UserAutomatism
{
	bool							bAutoStatus[2];						//�й�״̬
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
	bool							bAutoStatus[2];						//�й�״̬
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONG							lBaseScore;							//��������
	BYTE							cbBombCount;						//ը����Ŀ
	WORD							wBankerUser;						//ׯ���û�
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[17];						//�����˿�
	BYTE							bCardCount[2];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[17];					//�����б�
	bool							bAutoStatus[2];						//�й�״̬
};

//�����˿�
struct CMD_S_GameStart
{
	LONG							lBaseScore;							//��������
	WORD							wBankerUser;						//ׯ���û�
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[17];						//�˿��б�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//�˿���Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[17];						//�˿��б�
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
	LONG							lGameScore[2];						//��Ϸ����
	BYTE							bCardCount[2];						//�˿���Ŀ
	BYTE							bCardData[34];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CART				1									//�û�����
#define SUB_C_PASS_CARD				2									//��������
#define SUB_C_AUTOMATISM			4

struct CMD_C_Automatism
{
	bool							bAutomatism;
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[17];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////

#endif