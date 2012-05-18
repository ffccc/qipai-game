#ifndef CMD_LIANG_FU_TOU_HEAD_FILE
#define CMD_LIANG_FU_TOU_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						63									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("������")						//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_RF_FREE					GS_FREE								//�ȴ���ʼ
#define GS_RF_PLAYING				GS_PLAYING							//��Ϸ״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			101									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONG							lBaseScore;							//��������
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	LONG							lGameScore[3];						//��Ϸ�÷�
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[27];					//�����б�
	BYTE							cbCardData[2][27];					//�˿��б�
};

//�����˿�
struct CMD_S_GameStart
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[2][27];					//�˿��б�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//�˿���Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[27];						//�˿��б�
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
	LONG							lDrawScore[2];						//��Ϸ����
	LONG							lGameScore[4];						//��Ϸ����
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bCardData[108];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//�û�����
#define SUB_C_PASS_CARD				2									//��������

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[27];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////

#endif