#ifndef CMD_UNCOVERPIG_HEAD_FILE
#define CMD_UNCOVERPIG_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							380 								//��Ϸ  ID
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("����")						//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define HAND_CARD_COUNT                 13									//�˿���Ŀ    

//��Ϸ״̬
#define GS_WK_FREE				    	GS_FREE								//�ȴ���ʼ
#define GS_WK_SHOWCARD                  GS_PLAYING+1                        //�������
#define GS_WK_PLAYING			    	GS_PLAYING+2						//��Ϸ����

///////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD					100									//��������
#define SUB_S_GAME_START				101									//��Ϸ��ʼ
#define SUB_S_OUT_CARD					102									//�û�����
#define SUB_S_GAME_END					103									//��Ϸ����
#define SUB_S_SHOW_CARD					104									//�������

//��Ϸ״̬
struct CMD_S_StatusFree
{
};

//��Ϸ״̬
struct CMD_S_StatusShowCard
{
	WORD								wCurrentUser;						//��ǰ���
	WORD								ShowCardUser[GAME_PLAYER];	        //�������
	BYTE								ShowCard[GAME_PLAYER][4];		    //�������
	BYTE								ShowCardCount[GAME_PLAYER];			//������Ŀ
	BYTE								bCardData[HAND_CARD_COUNT];			//�����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD								wLastOutUser;						//������
	WORD								wFirstOutUser;                       //���ȳ���
	WORD								wCurrentUser;						//��ǰ���
	WORD								ShowCardUser[GAME_PLAYER];	        //�������
	BYTE								ShowCard[GAME_PLAYER][4];		    //�������
	BYTE								bCardData[HAND_CARD_COUNT];			//�����˿�
	BYTE								bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE								bTurnCardData[GAME_PLAYER];			//�������
	BYTE								ScoreCard[GAME_PLAYER][16];		    //�����˿�
	BYTE								bScoreCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE								bShowCardCount[GAME_PLAYER];		//������Ŀ
};

//�����˿�
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						 //��ǰ���
	BYTE								bCardData[HAND_CARD_COUNT];			 //�����˿�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD								wCurrentUser;						//��ǰ���
	bool								bFirstGame;							//�׾���Ϸ
};

//�û�����
struct CMD_S_OutCard
{
	BYTE								bCardCount;						    //������Ŀ
	WORD								wCurrentUser;						//��ǰ���
	WORD								wOutCardUser;						//�������
	BYTE								bCardData;					        //�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG								lGameTax;							//��Ϸ˰��
	LONG								lGameScore[4];						//��Ϸ����
};

//��������
struct CMD_S_ShowCard
{
	WORD								wShowCardUser;                     //�������
	BYTE								bShowCard[4];                      //�����б�
	BYTE								bShowCardCount;                    //������Ŀ
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CART				   1								  //�û�����
#define SUB_C_SHOW_CART                2                                  //�������

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE								bCardCount;						   //������Ŀ
	BYTE								bCardData;  					   //�˿��б�
};

//�������ݰ�
struct CMD_C_ShowCard
{
	BYTE								bShowCard[4];                      //�����б�
	BYTE								bShowCardCount;                    //������Ŀ
};

//////////////////////////////////////////////////////////////////////////

#endif