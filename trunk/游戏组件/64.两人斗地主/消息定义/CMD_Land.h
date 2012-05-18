#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#ifdef VIDEO_GAME
#define KIND_ID						535									//��Ϸ I D
#else
#define KIND_ID						142									//��Ϸ I D
#endif

#define GAME_PLAYER					2									//��Ϸ����
#define GAME_NAME					TEXT("���˶�����")						//��Ϸ����

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

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//��������
	LONG							lBaseScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[GAME_PLAYER];			//�з���Ϣ
	BYTE							bCardData[GAME_PLAYER][20];			//�����˿�
	bool							bUserTrustee[GAME_PLAYER];			//����й�

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
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
	BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[GAME_PLAYER][20];			//�����˿�
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[20];					//�����б�
	bool							bUserTrustee[GAME_PLAYER];			//����й�

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[17];						//�˿��б�
};

//�����˿�
struct CMD_S_SendAllCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[GAME_PLAYER][20];			//�˿��б�
	BYTE							bBackCardData[3];					//�����˿�
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
	BYTE							bBackCard[3];						//�����˿�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[20];						//�˿��б�
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
	LONG							lGameScore[GAME_PLAYER];			//��Ϸ����
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bCardData[54];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_LAND_SCORE			1									//�û��з�
#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_TRUSTEE				4									//�й���Ϣ

//�йܽṹ
struct CMD_C_UserTrustee {
	WORD							wUserChairID;						//�������
	bool							bTrustee;							//�йܱ�ʶ
};

//�û��з�
struct CMD_C_LandScore
{
	BYTE							bLandScore;							//��������
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[20];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////

#endif