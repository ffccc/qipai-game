#ifndef CMD_FLIGHTCHESS_HEAD_FILE
#define CMD_FLIGHTCHESS_HEAD_FILE

#include "..\..\..\�����ļ�\GlobalDef.h"
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							401								//��Ϸ I D
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("��������Ϸ")					//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//���̸�����
#define 	CHESSCOUNT_W				21
#define		CHESSCOUNT_H				20

//������ɫ
#define RED								0									//��ɫ(����)
#define YELLOW							1									//��ɫ(����)
#define GREEN							2									//��ɫ(����)
#define BLUE							3									//��ɫ(����)

#define PLAYER_CHESS_COUNT				4

#define MAX_NORMAL_ROADWAY		51
#define PER_NORMAL_ROADWAY		(MAX_NORMAL_ROADWAY+1)/GAME_PLAYER
#define MAX_ROADWAY				75
#define MAX_ROADWAY_BASE		75 + GAME_PLAYER * PLAYER_CHESS_COUNT
#define MAX_VALID_DOWN			MAX_ROADWAY_BASE + GAME_PLAYER
#define MAX_RANDNUM				6
#define UP_SKY_STEP			    6

#define N						255
#define NONE_TILE				N

#define CHESS_NOT_FINISHED      1
#define CHESS_FINISHED			2

#define NOT_AUTOMATISM				0
#define HALF_AI_AUTOMATISM			1
#define ALL_AI_AUTOMATISM			2

#define LIMIT_TIME					20999



#define GAME_NOT_ACTIVE			0
#define GAME_WAIT_START			1
#define GAME_WAIT_DICE			2
#define GAME_DICE_ANIMATED		3
#define GAME_WAIT_CHOOSE		4
#define GAME_CHESS_ANIMATED		5

//#define GAME_WAIT_DICE_OTHER		3
//#define GAME_DICE_ANIMATED_OTHER	6
//#define GAME_WAIT_CHOOSE_OTHER		4
//#define GAME_CHESS_ANIMATED_OTHER	8
#define GAME_WAIT_OTHER				6
#define GAME_READY_TO_START			7



//���̵�ͼ
const BYTE FLIGHTCHESS_MAP[CHESSCOUNT_H][CHESSCOUNT_W] = 	{	
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,203,N,N,N,N,N,N,	
		N,N,202,N,N,N,N,33,34,35,36,37,38,39,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,32,N,N,70,N,N,40,N,N,N,N,N,N,N,		
		N,N,N,N,N,N,N,31,N,N,71,N,N,41,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,30,228,229,72,230,231,42,N,N,N,N,N,N,N,	
		N,N,N,26,27,28,29,212,N,N,73,N,N,213,43,44,45,46,N,N,N,
		N,N,N,25,N,N,227,N,N,N,74,N,N,N,232,N,N,47,N,N,N,	
		N,N,N,24,N,N,226,N,N,N,75,N,N,N,233,N,N,48,N,N,N,
		N,N,N,23,64,65,66,67,68,69,N,57,56,55,54,53,52,49,N,N,N,	
		N,N,N,22,N,N,225,N,N,N,63,N,N,N,234,N,N,50,N,N,N,
		N,N,N,21,N,N,224,N,N,N,62,N,N,N,235,N,N,51,N,N,N,	
		N,N,N,20,19,18,17,211,N,N,61,N,N,210,3,2,1,0,N,N,N,
		N,N,N,N,N,N,N,16,223,222,60,221,220,4,N,200,N,N,N,N,N,	
		N,N,N,201,N,N,N,15,N,N,59,N,N,5,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,14,N,N,58,N,N,6,N,N,N,N,N,N,N,		
		N,N,N,N,N,N,N,13,12,11,10,9,8,7,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,	
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N		
	};
//��������㣬specialPull[0],specialPull[1]�ֱ���specialDown[0],specialDown[1]���Ӧ

const BYTE SPECIALPULL[GAME_PLAYER][2]={{17,49}, {30,10},{43,23},{4,36}};//�����
const BYTE SPECIALTONEXT[GAME_PLAYER][3]={{29,52,66},{42,58,72},{3,64,54},{16,70,60}};  //���������;�����


//#define STATUS_WAIT			0
//#define STATUS_FLYUP		1
//#define	STATUS_MOVE			2
//#define STATUS_TOSKY		3
//#define STATUS_COMPLETE		4
//���Ӷ���
struct tagServerChessItem
{
	BYTE							cbTileIndex;						//���������̵�λ��
	BYTE							cbColor;							//������ɫ
	BYTE							cbChessID;							//���ӱ�ʶ���ɻ�0��1��2��3��
	BYTE							cbChessStatus;						//����״̬��0Ϊ�ȴ���ɣ�1Ϊ��ɣ�2Ϊ�ߣ�3Ϊ���죬4Ϊ��ɻػ��ء�
};

//���̸��Ӷ���
struct tagServerTileItem
{
	BYTE							cbColor;							//���̸�����ɫ
	BYTE							cbChessCount;						//���̵�������
};

//////////////////////////////////////////////////////////////////////////
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_DICE_CAST				101									//��Ϸ��ʼ
#define SUB_S_CHESS_CHOOSE			102									//��Ϸ��ʼ
#define SUB_S_USER_ENTER			103									//�û�����
#define SUB_S_USER_LEFT				104									//�û��뿪����Ϸδ��ʼ��
#define SUB_S_USER_GIVEUP			105									//�û��뿪����Ϸ��ʼ��
#define SUB_S_USER_READY			106									//�û�׼��
#define SUB_S_GAME_END				107
#define SUB_S_TIME_OUT				108
#define SUB_S_AUTOMATISM			109

#define SUB_C_DICE_CAST				1									//�ƶ�����
#define SUB_C_CHESS_CHOOSE			2
#define SUB_C_TIME_OUT				3									//���峬ʱ
#define SUB_C_AUTOMATISM			4
//////////////////////////////////////////////////////////////////////////
//����������ṹ
//��Ϸ��ʼ
struct CMD_S_StatusFree
{
	BYTE							cbPlayStatus[GAME_PLAYER];				//��ʼ��������
};

struct CMD_S_StatusPlay
{
	WORD							wCurrentUser;							//��ǰ���
	long							lLimitTimeCount;
	BYTE							cbPlayStatus[GAME_PLAYER];				//��ʼ��������
	BYTE							cbAutomatism[GAME_PLAYER];				//�����Զ��й�
	BYTE							cbDiceNum;								//�׳�������number
	BYTE							cbGameStatus;
	tagServerChessItem				aServerChess[GAME_PLAYER][PLAYER_CHESS_COUNT];
};

struct CMD_S_GameStart
{
	WORD							wCurrentUser;							//��ʼ��������
};

struct CMD_S_DiceCast
{
	BYTE							cbDiceNum;								//�׳�������number
};

struct CMD_S_ChessChoose
{
	WORD							wCurrentUser;							//����ĵ�ǰ���
	BYTE							cbChessID;
};

//��Ϸ����
struct CMD_S_GameEnd
{
	WORD							wWinUser;							//ʤ�����
	long							lUserScore[GAME_PLAYER];			//�û�����
};

//�뿪��Ϸ��δ��ʼ��
struct CMD_S_UserLeft
{
	WORD							wLeftUser;							//�ֵ���������
};


//�뿪��Ϸ���ѿ�ʼ��
struct CMD_S_UserLost
{
	WORD							wLeftUser;								//�뿪�����
	WORD							wCurrentUser;							//�ֵ���������
};


//������Ϸ
struct CMD_S_UserEnter
{
	WORD							wCurrentUser;							//��ʼ��������
};

//���׼��
struct CMD_S_UserReady
{
	WORD							wCurrentUser;							//��ʼ��������
};

//��ҳ�ʱ
/*struct CMD_S_UserTimeOut
{
	BYTE							cbTimeOutUser;							//��ʱ�����
};*/

//����й��¼�
struct CMD_S_UserAutomatism
{
	WORD							wAutoUser;
	BYTE							cbAutomatism;						//�����й��¼������
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

struct CMD_C_ChessChoose
{
	BYTE							cbChessId;
};

struct CMD_C_Automatism
{
	BYTE							Automatism;
};
//////////////////////////////////////////////////////////////////////////

#endif