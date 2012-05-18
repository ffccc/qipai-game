#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#ifdef VIDEO_GAME
#define KIND_ID						513									//��Ϸ I D
#else
#define KIND_ID						113									//��Ϸ I D
#endif

#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("���ֶ�����")						//��Ϸ����

//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ
#define GS_WK_SCORE					GS_PLAYING							//�з�״̬
#define GS_WK_DOUBLE_SCORE			GS_PLAYING+1						//�ӱ�״̬
#define GS_WK_PLAYING				GS_PLAYING+2						//��Ϸ����

//�׶α���
#define CSD_NORMAL		0
#define CSD_SNATCHLAND	1
#define CSD_BRIGHTCARD	2
#define CSD_GAMESTART	3

//��������
#define BRIGHT_START_TIME				8									//���ƿ�ʼ
#define BRIGHT_CARD_TIME				4									//���Ʊ���

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_LAND_SCORE			101									//�з�����
#define SUB_S_GAME_START			102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����
#define SUB_S_BRIGHT_START			106									//���ƿ�ʼ
#define SUB_S_BRIGHT_CARD			107									//�������
#define SUB_S_DOUBLE_SCORE			108									//�ӱ�����
#define SUB_S_USER_DOUBLE			109									//�ӱ�����


//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
	bool							bBrightStart[GAME_PLAYER];			//�������
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//��������
	LONG							lBaseScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[3];						//�з���Ϣ
	BYTE							bCardData[3][20];					//�����˿�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	BYTE							bCallScorePhase;					//���ƽ׶�
	BYTE							bBrightTime;						//���Ʊ���
	bool							bUserBrightCard[GAME_PLAYER];		//�������
};

//�ӱ�״̬
struct CMD_S_StatusDoubleScore
{
	WORD							wLandUser;							//�������
	LONG							lBaseScore;							//��������
	BYTE							bLandScore;							//��������
	BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[3][20];					//�����˿�
	BYTE							bCardCount[3];						//�˿���Ŀ
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	bool							bAllowDouble;						//����ӱ�
	bool							bDoubleUser[GAME_PLAYER];			//�ӱ����
	bool							bUserBrightCard[GAME_PLAYER];		//�������

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
	BYTE							bCardData[3][20];					//�����˿�
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[20];					//�����б�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	BYTE							bBrightTime;						//���Ʊ���
	bool							bUserBrightCard[GAME_PLAYER];		//�������
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
	bool							bUserBrightCard[GAME_PLAYER];		//�������
};

//�û��з�
struct CMD_S_LandScore
{
	WORD							bLandUser;							//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bLandScore;							//�ϴνз�
	BYTE							bCurrentScore;						//��ǰ�з�
	BYTE							bPreCallScorePhase;					//֮ǰ�׶�
	BYTE							bCallScorePhase;					//���ƽ׶�
	bool							bBrightCardUser[GAME_PLAYER];		//�������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD				 			wLandUser;							//�������
	BYTE							bLandScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���	
	BYTE							bBrightCard;						//���Ʊ�־
	bool							bUserBrightCard[GAME_PLAYER];		//�������
	BYTE							bBackCardData[3];					//�����˿�
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
};

//���ƿ�ʼ
struct CMD_S_BrightStart
{
	WORD							wBrightUser;						//�������
	bool							bBright;							//���Ʊ�ʶ
};

//�������
struct CMD_S_BrightCard
{
	WORD							wBrightUser;						//�������
	BYTE							cbBrightTime;						//���Ʊ���
	BYTE							cbCurrenBrightTime;					//��ǰ����
	BYTE							cbCallScorePhase;					//�зֽ׶�
};

//�ӱ��ṹ
struct CMD_S_DoubleScore
{
	bool							bAllowDouble;						//����ӱ�
	BYTE							bBackCard[3];						//�����˿�
	WORD				 			wLandUser;							//�������
	BYTE							bCurrentScore;						//��ǰ�з�
};

//�ӱ��ṹ
struct CMD_S_UserDouble
{
	WORD							wDoubleUser;						//�ӱ����
	bool							bDoubleScore;						//�Ƿ�ӱ�
	BYTE							bCurrentScore;						//��ǰ�з�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_LAND_SCORE			1									//�û��з�
#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_TRUSTEE				4									//�й���Ϣ
#define SUB_C_BRIGHT_START			5									//���ƿ�ʼ
#define SUB_C_BRIGHT				6									//�������
#define SUB_C_DOUBLE_SCORE			7									//�ӱ�����

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

//�������
struct CMD_C_BrightCard
{
	BYTE							cbBrightCardTime;					//���Ʊ���
};

//��Ҽӱ�
struct CMD_C_DoubleScore
{
	bool							bDoubleScore;						//��Ҽӱ�
};

//////////////////////////////////////////////////////////////////////////

#endif