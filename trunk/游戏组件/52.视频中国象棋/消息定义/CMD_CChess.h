#ifndef CMD_CHESS_HEAD_FILE
#define CMD_CHESS_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#ifdef VIDEO_GAME
#define KIND_ID						506									//��Ƶ��ϷID
#else 
#define KIND_ID						200									//��Ϸ I D
#endif

#define GAME_PLAYER					2									//��Ϸ����

#ifdef VIDEO_GAME
#define GAME_NAME					TEXT("��Ƶ�й�����")					//��Ϸ����
#else 
#define GAME_NAME					TEXT("�й�����")				//��Ϸ����
#endif

#define GAME_GENRE					GAME_GENRE_SCORE					//��Ϸ����

#define IS_VIDEO_GAME				0									//��Ƶ����

//////////////////////////////////////////////////////////////////////////
//��Ϸ����
#define CountChar(String) (sizeof(String)/sizeof(String[0])-1)
#define	LEN_ACCOUNTS				32									//���ֳ���

//����״̬
#define GU_WAIT_PEACE				0x01								//��͵ȴ�
#define GU_WAIT_REGRET				0x02								//����ȴ�

//ʧ��ԭ��
#define FR_RULE_LIMIT				1									//��������
#define FR_COUNT_LIMIT				2									//��������
#define FR_PLAYER_OPPOSE			3									//��ҷ���

//����ԭ��							
#define REASON_TWENTY_STEP			1									//��ʮ����
#define REASON_GIVE_UP				2									//�������
#define REASON_OVER_TIME			3									//��ʱ����
#define REASON_CHESS_FEAZE			4									//��������
#define REASON_CATCH_CHESS			5									//��׽����
#define REASON_CATCH_GENERA			6									//��������
#define REASON_USER_EXIT			7									//ǿ������
#define REASON_KILL_GENERA			8									//��������

#define REASON_AGREE_DRAW			9									//ͬ��;�
#define REASON_OVER_STEP			10									//�����;�
#define REASON_LEISURE_STEP			11									//�ղ��;�
#define REASON_TEN_STEP				12									//ʮ������

//////////////////////////////////////////////////////////////////////////
//������

//״̬����
#define GAME_SCENE_FREE				GS_FREE								//�ȴ���ʼ
#define GAME_SCENE_PLAY				GS_PLAYING							//��Ϸ����

//ģʽ����
#define MODE_FAST					1									//����ģʽ
#define MODE_SLOW					2									//����ģʽ
#define MODE_CUSTOM					3									//�Զ�ģʽ

//��ֵ��Χ
#define MAX_STEP_TIME				300									//��ʱʱ��
#define MAX_DRAW_TIME				7200								//��ʱʱ��
#define MAX_SECOND_TIME				300									//����ʱ��

//����ģʽ
#define MODE_FAST_STEP_TIME			60									//��ʱʱ��
#define MODE_FAST_DRAW_TIME			600									//��ʱʱ��
#define MODE_FAST_SECOND_TIME		10									//����ʱ��

//����ģʽ
#define MODE_SLOW_STEP_TIME			180									//��ʱʱ��
#define MODE_SLOW_DRAW_TIME			1800								//��ʱʱ��
#define MODE_SLOW_SECOND_TIME		0									//����ʱ��

//����ṹ
struct tagGameRuleInfo
{
	//��������
	BYTE							cbDirections;						//��ʾ��־
	BYTE							cbRegretFlag;						//�����־
	BYTE							cbCurrentMode;						//��ǰģʽ

	//��ʱ����
	WORD							wRuleStepTime;						//��ʱʱ��
	WORD							wRuleDrawTime;						//��ʱʱ��
	WORD							wRuleSecondTime;					//����ʱ��
};

//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_MOVE_CHESS			101									//�ƶ�����
#define SUB_S_REGRET_FAILE			102									//����ʧ��
#define SUB_S_REGRET_RESULT			103									//������
#define SUB_S_REGRET_REQUEST		104									//��������
#define SUB_S_PEACE_RESPOND			105									//������Ӧ
#define SUB_S_PEACE_REQUEST			106									//��������
#define SUB_S_GAME_END				107									//��Ϸ����
#define SUB_S_CHESS_MANUAL			108									//������Ϣ
#define SUB_S_REQUEST_RULE			109									//��������
#define SUB_S_REQUEST_AGREE			110									//����ͬ��

//����״̬
struct CMD_S_StatusFree
{
	BYTE							cbGameRule;							//�����־
	WORD							wBankerUser;						//ׯ�����
	WORD							wRuleRoundID;						//�����ʶ
	tagGameRuleInfo					GameRuleInfo;						//��Ϸ����
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ��Ϣ
	WORD							wBankerUser;						//ׯ�����
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wLeaveDrawTime[GAME_PLAYER];		//ʣ���ʱ
	BYTE							cbRequestStatus[GAME_PLAYER];		//����״̬

	//��Ϸ����
	tagGameRuleInfo					GameRuleInfo;						//��Ϸ����
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ�����
	tagGameRuleInfo					GameRuleInfo;						//��Ϸ����
};

//��������
struct CMD_S_MoveChess
{
	//�û���Ϣ
	WORD							wActionUser;						//�����û�
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wLeaveDrawTime;						//ʣ���ʱ

	//������Ϣ
	BYTE							cbXSourcePos;						//����λ��
	BYTE							cbYSourcePos;						//����λ��
	BYTE							cbXTargetPos;						//����λ��
	BYTE							cbYTargetPos;						//����λ��
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbReason;							//����ԭ��
	WORD							wWinerUser;							//ʤ�����
	LONGLONG						lGameScore[GAME_PLAYER];			//�û��ɼ�
};

//����ʧ��
struct CMD_S_RegretFaile
{
	BYTE							cbFaileReason;						//ʧ��ԭ��
};

//������
struct CMD_S_RegretResult
{
	WORD							wTargetStep;						//Ŀ����Ŀ
	WORD							wRegretUser;						//�������
	WORD							wCurrentUser;						//��ǰ���
	WORD							wLeaveDrawTime[GAME_PLAYER];		//ʣ���ʱ
};

//��������
struct CMD_S_RequestRule
{
	WORD							wRuleRoundID;						//�����ʶ
	tagGameRuleInfo					GameRuleInfo;						//��Ϸ����
};

//����ͬ��
struct CMD_S_RequestAgree
{
	WORD							wRuleRoundID;						//�����ʶ
	tagGameRuleInfo					GameRuleInfo;						//��Ϸ����
};

//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_C_MOVE_CHESS			1									//�ƶ�����
#define SUB_C_PEACE_REQUEST			2									//��������
#define SUB_C_PEACE_RESPOND			3									//����Ӧ��
#define SUB_C_REGRET_REQUEST		4									//��������
#define SUB_C_REGRET_RESPOND		5									//����Ӧ��
#define SUB_C_CONCLUDE_REQUEST		6									//��������

//�ƶ�����
struct CMD_C_MoveChess
{
	//λ�ñ���
	BYTE							cbXSourcePos;						//����λ��
	BYTE							cbYSourcePos;						//����λ��
	BYTE							cbXTargetPos;						//����λ��
	BYTE							cbYTargetPos;						//����λ��

	//ʱ�����
	WORD							wUsedTimeCount;						//ʹ��ʱ��
};

//������Ӧ
struct CMD_C_PeaceRespond
{
	BYTE							cbApprove;							//ͬ���־
};

//������Ӧ
struct CMD_C_RegretRespond
{
	BYTE							cbApprove;							//ͬ���־
};

//���ƹ���
struct CMD_C_CustomizeRult
{
	tagGameRuleInfo					GameRuleInfo;						//��Ϸ����
};

//�������
struct CMD_C_ConcludeRequest
{
	BYTE							cbReason;						//����ԭ��
};

//////////////////////////////////////////////////////////////////////////

#endif
