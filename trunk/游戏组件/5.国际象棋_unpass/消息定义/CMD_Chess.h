#ifndef CMD_CHESS_HEAD_FILE
#define CMD_CHESS_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						ID_CHESS    							//��Ϸ I D
#define GAME_PLAYER					2									//��Ϸ����
#define GAME_NAME					TEXT("��������")					//��Ϸ����
#define GAME_GENRE					GAME_GENRE_SCORE					//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//��Ϸ����

//������ɫ
#define NO_CHESS					0									//û������
#define BLACK_CHESS					1									//��ɫ����
#define WHITE_CHESS					2									//��ɫ����

//���Ӷ���
#define CHESS_KING					1									//��
#define CHESS_QUEEN					2									//��
#define CHESS_ROOK					3									//��
#define CHESS_ELEPHANT				4									//��
#define CHESS_HORSE					5									//��
#define CHESS_SOLDIER				6									//��

//����״̬
#define GU_WAIT_PEACE				0x01								//�ȴ����
#define GU_WAIT_REGRET				0x02								//�ȴ�����

//ʧ��ԭ��
#define FR_COUNT_LIMIT				1									//��������
#define FR_PLAYER_OPPOSE			2									//��ҷ���

//���Ӷ���
struct tagChessItem
{
	BYTE							cbXPos;								//��������
	BYTE							cbYPos;								//��������
	BYTE							cbColor;							//������ɫ
	BYTE							cbChess;							//����Ȩλ
	BYTE							cbChessID;							//���ӱ�ʶ
	WORD							wWalkCount;							//���߲���
};

//����ṹ
struct tagGameRuleInfo
{
	BYTE							cbGameMode;							//�Ծ�ģʽ
	BYTE							cbDirections;						//��ʾ��־
	BYTE							cbRegretFlag;						//�������
	WORD							wRuleStepTime;						//��Ϸ��ʱ
	WORD							wRuleLimitTime;						//�޶�ʱ��
	WORD							wRuleTimeOutCount;					//��ʱ����
};


//���׽ṹ
struct tagChessManual
{
	//�ƶ�����
	BYTE							cbSourceID;							//���ӱ�ʶ
	BYTE							cbSourceColor;						//������ɫ
	BYTE							cbSourceChess;						//����Ȩλ
	BYTE							cbXSourceChessPos;					//����λ��
	BYTE							cbYSourceChessPos;					//����λ��
	BYTE							cbXTargetChessPos;					//����λ��
	BYTE							cbYTargetChessPos;					//����λ��

	//��������
	BYTE							cbUniteID;							//���ӱ�ʶ
	BYTE							cbUniteColor;						//������ɫ
	BYTE							cbUniteChess;						//����Ȩλ
	BYTE							cbSwitchChess;						//��������
	BYTE							cbXUniteChessPos;					//����λ��
	BYTE							cbYUniteChessPos;					//����λ��
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_MOVE_CHESS			101									//�ƶ�����
#define SUB_S_REGRET_REQ			102									//��������
#define SUB_S_REGRET_FAILE			103									//����ʧ��
#define SUB_S_REGRET_RESULT			104									//������
#define SUB_S_PEACE_REQ				105									//��������
#define SUB_S_PEACE_ANSWER			106									//����Ӧ��
#define SUB_S_GAME_END				107									//��Ϸ����
#define SUB_S_CHESS_MANUAL			108									//������Ϣ
#define SUB_S_REQ_SET_RULE			109									//��������
#define SUB_S_REQ_AGREE_RULE		110									//����ͬ��

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
	WORD							wLeftClock[2];						//ʣ��ʱ��
	WORD							cbBegStatus[2];						//����״̬
	WORD							wChessStepCount[2];					//���Ӳ���
	tagChessItem					ChessItemArray[2][16];				//������Ϣ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wGameClock;							//��ʱʱ��
	WORD							wBlackUser;							//�������
};

//��������
struct CMD_S_MoveChess
{
	BYTE							cbXSourcePos;						//����λ��
	BYTE							cbYSourcePos;						//����λ��
	BYTE							cbXTargetPos;						//����λ��
	BYTE							cbYTargetPos;						//����λ��
	BYTE							cbSwitchChess;						//�任����
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
	BYTE							cbRegretCount;						//������Ŀ
	WORD							wLeftClock[2];						//��ʱʱ��
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;							//��Ϸ˰��
	WORD							wWinUser;							//ʤ�����
	LONG							lUserScore[2];						//�û�����
};

//��������
struct CMD_S_Req_SetRult
{
	tagGameRuleInfo					GameRuleInfo;						//��Ϸ����
};

//����ͬ��
struct CMD_S_Req_AgreeRult
{
	tagGameRuleInfo					GameRuleInfo;						//��Ϸ����
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_MOVE_CHESS			1									//�ƶ�����
#define SUB_C_REGRET_REQ			2									//��������
#define SUB_C_REGRET_ANSWER			3									//����Ӧ��
#define SUB_C_PEACE_REQ				4									//��������
#define SUB_C_PEACE_ANSWER			5									//����Ӧ��
#define SUB_C_GIVEUP_REQ			6									//��������

//�ƶ�����
struct CMD_C_MoveChess
{
	BYTE							cbXSourcePos;						//����λ��
	BYTE							cbYSourcePos;						//����λ��
	BYTE							cbXTargetPos;						//����λ��
	BYTE							cbYTargetPos;						//����λ��
	BYTE							cbSwitchChess;						//�任����
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

//���ƹ���
struct CMD_C_CustomizeRult
{
	tagGameRuleInfo					GameRuleInfo;						//��Ϸ����
};
//////////////////////////////////////////////////////////////////////////

#endif