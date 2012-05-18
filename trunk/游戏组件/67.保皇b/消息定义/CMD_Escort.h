#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID						180								//��Ϸ I D
#define GAME_PLAYER					5									//��Ϸ����
#define PLAYER_CARDS				33									//ÿ��������ϵ�����
#define GAME_NAME					TEXT("����")						//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ

#define GS_WK_PLAYING				GS_PLAYING+1						//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_GAME_START			102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����
#define SUB_S_ENTHRONEMENT			106									//new��λ

//new��λ
struct CMD_S_Enthronement
{
	WORD							bCurrentEnthronement;				//new��λ
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[PLAYER_CARDS];						//�����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONG							lBaseScore;							//��������
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[PLAYER_CARDS];						//�����˿�
	BYTE							bCardCount[GAME_PLAYER];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[PLAYER_CARDS];					//�����б�
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wCurrentEnthronement;				//��ǰ��λ
	WORD							wCurrentHousecarl;					//��ǰ����
	BYTE							bCardData[PLAYER_CARDS];						//�˿��б�
};


//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[PLAYER_CARDS];						//�˿��б�
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
	LONG							lGameScore[GAME_PLAYER];						//��Ϸ����
	BYTE							bCardCount[GAME_PLAYER];						//�˿���Ŀ
	BYTE							bCardData[165];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_CHAIRID				4									//������
//������
struct CMD_C_ChairID
{
	bool IsChair;
	bool IsGet;
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[PLAYER_CARDS];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////

#endif