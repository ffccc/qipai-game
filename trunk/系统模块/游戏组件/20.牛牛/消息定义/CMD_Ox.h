#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#ifdef VIDEO_GAME
#define KIND_ID						529									//��Ƶ��ϷID
#else 
#define KIND_ID						5									//��Ϸ I D
#endif
#define GAME_PLAYER						4									//��Ϸ����


#ifdef VIDEO_GAME
#define GAME_NAME					TEXT("��Ƶţţ")					//��Ϸ����
#else 
#define GAME_NAME					TEXT("ţţ")						//��Ϸ����
#endif

#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define MAXCOUNT						5									//�˿���Ŀ

//����ԭ��
#define GER_NO_PLAYER					0x10								//û�����

//��Ϸ״̬
#define GS_TK_FREE					GS_FREE									//�ȴ���ʼ
#define GS_TK_CALL					GS_PLAYING								//��ׯ״̬
#define GS_TK_SCORE					GS_PLAYING+1							//��ע״̬
#define GS_TK_PLAYING				GS_PLAYING+2							//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE					101									//��ע���
#define SUB_S_PLAYER_EXIT				102									//�û�ǿ��
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����
#define SUB_S_OPEN_CARD					105									//�û�̯��
#define SUB_S_CALL_BANKER				106									//�û���ׯ

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG								lCellScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusCall
{
	WORD								wCallBanker;						//��ׯ�û�
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	//��ע��Ϣ
	LONG								lTurnMaxScore;						//�����ע
	//LONG								lTurnLessScore;						//��С��ע
	LONG								lTableScore[GAME_PLAYER];			//��ע��Ŀ
	WORD								wBankerUser;						//ׯ���û�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//״̬��Ϣ
	LONG								lTurnMaxScore;						//�����ע
	//LONG								lTurnLessScore;						//��С��ע
	LONG								lTableScore[GAME_PLAYER];			//��ע��Ŀ
	WORD								wBankerUser;						//ׯ���û�

	//�˿���Ϣ
	BYTE								cbHandCardData[GAME_PLAYER][MAXCOUNT];//�����˿�
	BYTE								bOxCard[GAME_PLAYER];				//ţţ����
};

//�û���ׯ
struct CMD_S_CallBanker
{
	WORD								wCallBanker;						//��ׯ�û�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��ע��Ϣ
	LONG								lTurnMaxScore;						//�����ע
	WORD								wBankerUser;						//ׯ���û�
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wAddScoreUser;						//��ע�û�
	LONG								lAddScoreCount;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG								lGameTax[GAME_PLAYER];				//��Ϸ˰��
	LONG								lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE								cbCardData[GAME_PLAYER];			//�û��˿�
};

//�������ݰ�
struct CMD_S_SendCard
{
	BYTE								cbCardData[GAME_PLAYER][MAXCOUNT];	//�û��˿�
};

//�û��˳�
struct CMD_S_PlayerExit
{
	WORD								wPlayerID;							//�˳��û�
};

//�û�̯��
struct CMD_S_Open_Card
{
	WORD								wPlayerID;							//̯���û�
	BYTE								bOpen;								//̯�Ʊ�־
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_CALL_BANKER				1									//�û���ׯ
#define SUB_C_ADD_SCORE					2									//�û���ע
#define SUB_C_OPEN_CARD					3									//�û�̯��

//�û���ׯ
struct CMD_C_CallBanker
{
	BYTE								bBanker;							//��ׯ��־
};

//�û���ע
struct CMD_C_AddScore
{
	LONG								lScore;								//��ע��Ŀ
};

//�û�̯��
struct CMD_C_OxCard
{
	BYTE								bOX;								//ţţ��־
};

//////////////////////////////////////////////////////////////////////////

#endif
