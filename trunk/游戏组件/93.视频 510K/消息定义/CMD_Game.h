#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//������

#define KIND_ID						119									//��Ϸ I D
#define GAME_NAME					TEXT("��Ƶ510K")						//��Ϸ����

//�������
#define GAME_PLAYER					4									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(5,1,0)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(5,1,0)				//����汾

//////////////////////////////////////////////////////////////////////////
//��Ŀ����

#define MAX_COUNT					28									//�����Ŀ
#define FULL_COUNT					108									//ȫ����Ŀ

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_SINGLE_LINK				3									//��������
#define CT_DOUBLE_LINK				4									//��������
#define CT_BOMB_3					5									//3ը��
#define CT_510K_DC					6									//��510K
#define CT_KING_DC					7									//��С��
#define CT_510K_SC					8									//��510K
#define CT_KING_ST					9									//��С��
#define CT_KING_SK					10									//������
#define CT_BOMB_4					11									//4ը����
#define CT_BOMB_5					12									//5������
#define CT_KING_THREE				13									//3������
#define CT_BOMB_6					14									//6ը����
#define CT_BOMB_7					15									//7ը����
#define CT_KING_FOUR				16									//4��ը��
#define CT_BOMB_8					17									//8ը����

//////////////////////////////////////////////////////////////////////////
//״̬����

#define GAME_SCENE_FREE				GS_FREE							//�ȴ���ʼ
#define GAME_SCENE_PLAY				GS_PLAYING						//��Ϸ����
#define GAME_SCENE_WAIT				GS_PLAYING+1					//�ȴ���ʼ

//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				104									//�û�����
#define SUB_S_PASS_CARD				105									//�û�����
#define SUB_S_CARD_INFO				106									//�˿���Ϣ
#define SUB_S_GAME_END				107									//��Ϸ����
#define SUB_S_CONTINUE_GAME			108									//������Ϸ
#define SUB_S_AUTOMATISM			109									//�й���Ϣ

//����й��¼�
struct CMD_S_UserAutomatism
{
	bool							bAutoStatus[4];						//�й�״̬
};

//����״̬
struct CMD_S_StatusFree
{
	//��Ԫ����
	LONG							lCellScore;							//��Ԫ����
	bool							bAutoStatus[4];						//�й�״̬
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG							lCellScore;							//��Ԫ����
	WORD							wCurrentUser;						//��ǰ���
	WORD							wPersistInfo[GAME_PLAYER][2];		//��Ϸ��Ϣ
	BYTE							b510KCardRecord[3][8];

	//�ȼ�����
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[GAME_PLAYER];			//�ȼ���ֵ

	//ʤ����Ϣ
	WORD							wWinCount;							//ʤ������
	WORD							wWinOrder[GAME_PLAYER];				//ʤ���б�

	//������Ϣ
	WORD							wTurnWiner;							//����ʤ��
	BYTE							cbTurnCardType;						//��������
	BYTE							cbTurnCardCount;					//������Ŀ
	BYTE							cbTurnCardData[MAX_COUNT];			//��������
	BYTE							cbMagicCardData[MAX_COUNT];			//����˿�

	//�˿���Ϣ
	BYTE							cbHandCardData[MAX_COUNT];			//�����˿�
	BYTE							cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ
	bool							bAutoStatus[4];						//�й�״̬
};

//�ȴ�״̬
struct CMD_S_StatusWait
{
	//��Ԫ����
	LONG							lCellScore;							//��Ԫ����

	//��Ϸ����
	BYTE							cbContinue[GAME_PLAYER];			//������־
	WORD							wPersistInfo[GAME_PLAYER][2];		//��Ϸ��Ϣ

	//�ȼ�����
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[GAME_PLAYER];			//�ȼ���ֵ
	bool							bAutoStatus[4];						//�й�״̬
};

//�����˿�
struct CMD_S_GameStart
{
	//��Ϸ��Ϣ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[GAME_PLAYER];			//�ȼ���ֵ
	WORD							wPersistInfo[GAME_PLAYER][2];		//��Ϸ��Ϣ

	//�˿���Ϣ
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[MAX_COUNT-1];			//�˿��б�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	int								TurnScore;
	BYTE							b510KCardRecord[3][8];
	BYTE							cbCardData[MAX_COUNT-1];			//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							cbTurnOver;							//һ�ֽ���
	WORD				 			wCurrentUser;						//��ǰ���
	WORD				 			wPassCardUser;						//�������
	int								TurnScore;
	int								PlayerScore[GAME_PLAYER];
};

//�˿���Ϣ
struct CMD_S_CardInfo
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	int								TurnScore;
	int								PlayerScore[GAME_PLAYER];

	//��Ϸ�ɼ�
	LONG							lGameScore[GAME_PLAYER];			//��Ϸ����

	//�ۼ�����
	WORD							wPersistInfo[GAME_PLAYER][2];		//��Ϸ��Ϣ

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//�˿��б�
};

//�û�����
struct CMD_S_ContinueGame
{
	WORD							wChairID;							//�����û�
};

//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_C_OUT_CARD				1									//�û�����
#define SUB_C_PASS_CARD				2									//�û�����
#define SUB_C_CONTINUE_GAME			5									//������Ϸ
#define SUB_C_AUTOMATISM			6									//�й���Ϣ

//�û��й�
struct CMD_C_Automatism
{
	bool							bAutomatism;
};

//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿�����
};


//////////////////////////////////////////////////////////////////////////

#endif