#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#ifdef VIDEO_GAME
#define KIND_ID						523									//��Ϸ I D
#else
#define KIND_ID						135									//��Ϸ I D
#endif

#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("�Ĵ�������")						//��Ϸ����

//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ
#define GS_WK_SCORE					GS_PLAYING							//�з�״̬
#define GS_WK_PLAYING				GS_PLAYING+1						//��Ϸ����

//��Ϸ����
#define BACK_CATCH					0x01								//��ץ
#define MING_CATCH					0x02								//��ץ
#define LOOK_CARD					0x03								//����
#define DOUBLE_SCORE				0x04								//����
#define CALL_BANKER					0x05								//��ׯ
#define CALL_SCORE					0x06								//�з�
#define CALL_TWO_SCORE				0x07								//�д��
#define PASS_ACTION					0x08								//����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_CHOICE_LOOK			106									//��ׯ����
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
	LONG							lBaseScore;							//��������
	WORD							wFirstUser;							//�׽����
	WORD							wBankerUser;						//ׯ���û�
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[20];						//�����˿�
	BYTE							bBackCard[3];						//�����˿�
	BYTE				 			bMingCard;							//�׽�����
	BYTE							bCurrentState;						//��ǰ״̬
	BYTE							bCallScoreTimes[3];					//�зִ���
	BYTE							bBackCatchTag;						//��ץ��־
	bool							bUserTrustee[GAME_PLAYER];			//����й�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONG							lBaseScore;							//��������
	WORD							wFirstUser;							//�׽����
	WORD							wBankerUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wLastOutUser;						//���Ƶ���
	WORD							wOutBombCount;						//ը����Ŀ
	BYTE				 			bMingCard;							//�׽�����
	BYTE							bCallScoreTimes[3];					//�зִ���
	BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[20];						//�����˿�
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[20];					//�����б�
	BYTE							bScoreTimes[3];						//��������
	bool							bUserTrustee[GAME_PLAYER];			//����й�
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

//�û���ׯ
struct CMD_S_CallBanker
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE				 			bMingCard;							//�׽�����
};

//�û��з�
struct CMD_S_LandScore
{
	WORD							bLandUser;							//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bLandScore;							//�ϴνз�
	BYTE							bCurrentState;						//��ǰ״̬
	BYTE							bUserCallTimes;						//�û�����
	WORD							wBankerUser;						//ׯ���û�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//WORD				 			wLandUser;							//�������
	BYTE							bLandScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[3];						//�����˿�
	WORD							wUserMultiple[3];					//�û�����
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
	LONG							lGameScore[3];						//��Ϸ����
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bCardData[54];						//�˿��б� 
	BYTE							bBackCard[3];						//�����˿� 
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
