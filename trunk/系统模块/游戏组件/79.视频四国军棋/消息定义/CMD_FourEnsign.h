#ifndef CMD_CHESS_HEAD_FILE
#define CMD_CHESS_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#ifdef VIDEO_GAME
#define KIND_ID						606									//��Ƶ��ϷID
#else 
#define KIND_ID						207						//��Ϸ I D
#endif

#define GAME_PLAYER					4									//��Ϸ����

#ifdef VIDEO_GAME
#define GAME_NAME					TEXT("��Ƶ�Ĺ�����")					//��Ϸ����
#else 
#define GAME_NAME					TEXT("�Ĺ�����")					//��Ϸ����
#endif

#define GAME_GENRE					GAME_GENRE_SCORE					//��Ϸ����

#define IS_VIDEO_GAME				0									//��Ƶ����

//////////////////////////////////////////////////////////////////////////
//��Ϸ״̬
/*
#define GS_WK_FREE				    	GS_FREE								//�ȴ���ʼ
#define GS_WK_PLAYING			    	GS_PLAYING						//��Ϸ����*/



//////////////////////////////////////////////////////////////////////////

//��Ϸ����
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	�������Ϳͻ��˵�����涨������Ļ����һ����X�����ң�Y���ϵ��£�
	�ڿͻ��ˣ��Լ�������������Զ��:X:6~10 , Y:11~16
	�����±꣺Ϊ�˺�������ͳһ����Ϸ�����������һ�±�Ϊ��(��ʾX��)���ڶ��±�Ϊ�У���ʾY�ᣩ

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

//������ɫ
#define CHESS_COLOR_NO				0									//��������
#define CHESS_COLOR_GREEN			1									//��ɫ����
#define CHESS_COLOR_BLACK			2									//��ɫ����
#define CHESS_COLOR_BLUE			3									//��ɫ����
#define CHESS_COLOR_RED				4									//��ɫ����


//��ʾģʽ
#define CHESS_NO					0									//û������
#define CHESS_BACK					1									//��ʾ����
#define CHESS_FORCE					2									//��ʾ����


//���Ӷ���
#define CHESS_SI_LING				12									//˾������
#define CHESS_JUN_ZHANG				11									//��������
#define CHESS_SHI_ZHANG				10									//ʦ������
#define CHESS_LV_ZHANG				9									//�ó�����
#define CHESS_TUAN_ZHANG			8									//�ų�����
#define CHESS_YING_ZHANG			7									//Ӫ������
#define CHESS_LIAN_ZHANG			6									//��������
#define CHESS_PAI_ZHANG				5									//�ų�����
#define CHESS_GONG_BING				4									//��������
#define CHESS_ZHA_DAN				3									//ը������
#define CHESS_DI_LEI				2									//��������
#define CHESS_JUN_QI				1									//��������
#define CHESS_NULL					0									//û������

//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////

//���Ӷ���
struct tagChessItem
{
	BYTE							bColor;								//������ɫ
	BYTE							bChessID;							//���ӱ�ʶ
	BYTE							bChessMode;							//��ʾģʽ
};

//���׽ṹ
struct tagChessManual
{
	tagChessItem					tagSourceChess;						//��ʼ����						

	tagChessItem					tagDesChess;						//Ŀ������	
};

//////////////////////////////////////////////////////////////////////////

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define	SUB_S_FINISH_PLACEMENT		101									//��ɲ���
#define SUB_S_MOVE_CHESS			102									//��������
#define SUB_S_ONE_LOSE				103									//ĳ��û��
#define SUB_S_GAME_END				104									//��Ϸ����
#define SUB_S_GAME_TIME_OUT			105									//��ҳ�ʱ
#define SUB_S_RECORD				106									//��ʷ��¼
#define SUB_S_CHESS_MANUAL			107									//��������
#define SUB_S_PLACEMENT				108									//���Ͳ���
#define SUB_S_REQUEST_PEACE			109									//�������
#define SUB_S_PEACE_FAIL			110									//����ʧ��
#define SUB_S_USER_UP				111									//�������

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wCurrentUser;						//��ǰ�û�
	tagChessItem					Chess[17][17];						//������Ϣ
	LONG							lStepCount;							//��ǰ����
};

//�������
struct CMD_S_UserUp
{
	WORD							wUser ;								//�������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wCurrentUser;						//��ǰ���
};

//��������
struct CMD_S_MoveChess
{
	BYTE							bXSourcePos;						//����λ��
	BYTE							bYSourcePos;						//����λ��
	BYTE							bXTargetPos;						//����λ��
	BYTE							bYTargetPos;						//����λ��
	BYTE							bSourceColor;						//������ɫ
	BYTE							bTargetColor;						//������ɫ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wMoveUser;							//�������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lUserScore[GAME_PLAYER];			//�û�����
};

//��ɲ���
struct CMD_S_FinishPlacement
{
	WORD							wCurrentUser;						//��ǰ���
	BYTE							bColor;								//������ɫ
	BYTE							bChessID[5][6];						//���ӱ�ʶ
};

//ĳ��û��
struct CMD_S_OneLose
{
	WORD							wLoseUser;							//û�����
	WORD							wCurrentUser;						//��ǰ���
};

//��ҳ�ʱ
struct CMD_S_GameTimeOut
{
	WORD							wCurrentUser;						//��ǰ���
};

//�������
struct CMD_S_RuquestPeace
{
	WORD							wRequestUser;						//�������
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_MOVE_CHESS			1									//�ƶ�����
#define SUB_C_FINISH_PLACEMENT		2									//��ɲ���
#define SUB_C_GAME_TIME_OUT			3									//��ҳ�ʱ
#define SUB_C_USER_SURRENDER		4									//�������
#define SUB_C_REQUEST_PEACE			5									//�������
#define SUB_C_PEACE_ANSWER			6									//����Ӧ��


//����Ӧ��
struct CMD_C_PeaceAnswer
{
	bool							bAgree;								//����Ӧ��
};

//�ƶ�����
struct CMD_C_MoveChess
{
	BYTE							bXSourcePos;						//����λ��
	BYTE							bYSourcePos;						//����λ��
	BYTE							bXTargetPos;						//����λ��
	BYTE							bYTargetPos;						//����λ��
};

//��ɲ���
struct CMD_C_FinishPlacement
{
	BYTE							bColor;								//������ɫ
	BYTE							bChessID[5][6];						//���ӱ�ʶ
};
//////////////////////////////////////////////////////////////////////////

#endif