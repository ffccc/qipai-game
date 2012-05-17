#ifndef CMD_GOBANG_HEAD_FILE
#define CMD_GOBANG_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//��������

#define KIND_ID						203									//��Ϸ I D
#define GAME_PLAYER					2									//��Ϸ����
#define GAME_NAME					TEXT("������")						//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//��Ϸ����

//������ɫ
#define NO_CHESS					0									//û������
#define BLACK_CHESS					1									//��ɫ����
#define WHITE_CHESS					2									//��ɫ����

//����״̬
#define GU_WAIT_PEACE				0x01								//�ȴ����
#define GU_WAIT_REGRET				0x02								//�ȴ�����

//ʧ��ԭ��
#define FR_COUNT_LIMIT				1									//��������
#define FR_PLAYER_OPPOSE			2									//��ҷ���

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_CHESS			101									//��������
#define SUB_S_REGRET_REQ			102									//��������
#define SUB_S_REGRET_FAILE			103									//����ʧ��
#define SUB_S_REGRET_RESULT			104									//������
#define SUB_S_PEACE_REQ				105									//��������
#define SUB_S_PEACE_ANSWER			106									//����Ӧ��
#define SUB_S_BLACK_TRADE			107									//�����Լ�
#define SUB_S_GAME_END				108									//��Ϸ����
#define SUB_S_CHESS_MANUAL			109									//������Ϣ

//��Ϸ״̬
struct CMD_S_StatusFree
{
	WORD							wBlackUser;							//�������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wGameClock;							//��ʱʱ��
	WORD							wBlackUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbRestrict;							//�Ƿ����
	BYTE							cbTradeUser;						//�Ƿ�Ի�
	BYTE							cbDoubleChess;						//�Ƿ�˫��
	WORD							wLeftClock[2];						//ʣ��ʱ��
	WORD							cbBegStatus[2];						//����״̬
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wGameClock;							//��ʱʱ��
	WORD							wBlackUser;							//�������
	BYTE							cbRestrict;							//�Ƿ����
	BYTE							cbTradeUser;						//�Ƿ�Ի�
	BYTE							cbDoubleChess;						//�Ƿ�˫��
};

//��������
struct CMD_S_PlaceChess
{
	BYTE							cbXPos;								//����λ��
	BYTE							cbYPos;								//����λ��
	WORD				 			wPlaceUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wLeftClock[2];						//��ʱʱ��
};

//����ʧ��
struct CMD_S_RegretFaile
{
	BYTE							cbFaileReason;						//ʧ��ԭ��
};

//������
struct CMD_S_RegretResult
{
	WORD							wRegretUser;						//�������
	WORD							wCurrentUser;						//��ǰ���
	WORD							wRegretCount;						//������Ŀ
	WORD							wLeftClock[2];						//��ʱʱ��
};

//��Ϸ����
struct CMD_S_GameEnd
{
	WORD							wWinUser;							//ʤ�����
	LONG							lUserScore[2];						//�û�����
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_CHESS			1									//��������
#define SUB_C_REGRET_REQ			2									//��������
#define SUB_C_REGRET_ANSWER			3									//����Ӧ��
#define SUB_C_PEACE_REQ				4									//��������
#define SUB_C_PEACE_ANSWER			5									//����Ӧ��
#define SUB_C_GIVEUP_REQ			6									//��������
#define SUB_C_TRADE_REQ				7									//��������

//��������
struct CMD_C_PlaceChess
{
	BYTE							cbXPos;								//����λ��
	BYTE							cbYPos;								//����λ��
};

//������Ӧ
struct CMD_C_RegretAnswer
{
	BYTE							cbApprove;							//ͬ���־
};

//������Ӧ
struct CMD_C_PeaceAnswer
{
	BYTE							cbApprove;							//ͬ���־
};

//////////////////////////////////////////////////////////////////////////

#endif