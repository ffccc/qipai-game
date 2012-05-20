#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						1									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("����")						//��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GS_PLAYING							//��ע״̬
#define	GS_GAME_START				GS_PLAYING+1						//����״̬
#define	GS_GAME_END					GS_PLAYING+2						//����״̬

//�������
#define ID_HORSE1					1									//1������
#define ID_HORSE2					2									//2������
#define ID_HORSE3					3									//3������
#define ID_HORSE4					4									//4������
#define ID_HORSE5					5									//5������
#define ID_HORSE6					6									//6������
#define ID_HORSE7					7									//7������
#define ID_HORSE8					8									//8������

//���ʶ���
#define RATE_TWO_PAIR				8									//����

//�������
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//��������ʷ��¼
#define MAX_S_SCORE_HISTORY			16									//��ʷ����

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							cbWinner;							//Ӯ��
};

//�ͻ�����ʷ��¼
#define MAX_C_SCORE_HISTORY			65									//��ʷ����
//��¼��Ϣ
struct tagClientGameRecord
{
	enOperateResult					enOperateFlags;						//������ʶ
	BYTE							cbWinner;						//�мҵ���
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������
#define SUB_S_UPDATE_USER_SCORE		109									//������ҽ��
#define SUB_S_UPDATE_BANKER_SCORE	110									//����ׯ�ҽ��


//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	BYTE							lJettonArea;						//��ע����
	LONGLONG							lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	LONGLONG							lScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG							lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;						//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	LONGLONG							lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONGLONG							lUserMaxScore;						//��ҽ��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG							lBankerWinScore;					//ׯ�ҳɼ�
	LONGLONG							lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG							lApplyBankerCondition;				//��������
	LONGLONG							lAreaLimitScore;					//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע

	LONGLONG							lAllScore1;						//1����ע
	LONGLONG							lAllScore2;						//2����ע
	LONGLONG							lAllScore3;						//3����ע
	LONGLONG							lAllScore4;						//4����ע
	LONGLONG							lAllScore5;						//5����ע
	LONGLONG							lAllScore6;						//6����ע
	LONGLONG							lAllScore7;						//7����ע
	LONGLONG							lAllScore8;						//8����ע

	//�����ע
	LONGLONG							lUserScore1;						//1����ע
	LONGLONG							lUserScore2;						//2����ע
	LONGLONG							lUserScore3;						//3����ע
	LONGLONG							lUserScore4;						//4����ע
	LONGLONG							lUserScore5;						//5����ע
	LONGLONG							lUserScore6;						//6����ע
	LONGLONG							lUserScore7;						//7����ע
	LONGLONG							lUserScore8;						//8����ע

	//��һ���
	LONGLONG							lUserMaxScore;						//�����ע							

	//������Ϣ
	LONGLONG							lApplyBankerCondition;				//��������
	LONGLONG							lAreaLimitScore;					//��������

	////�˿���Ϣ
 //	BYTE							cbCardCount[2];						//�˿���Ŀ
	//BYTE							cbTableCardArray[2][3];				//�����˿�
	//�����ٶ�
	BYTE							cbHorseSpeed[8][8];

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG							lBankerWinScore;					//ׯ��Ӯ��
	LONGLONG							lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG							lEndBankerScore;					//ׯ�ҳɼ�
	LONGLONG							lEndUserScore;						//��ҳɼ�
	LONGLONG							lEndUserReturnScore;				//���ػ���
	LONGLONG							lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ��λ��
	LONGLONG						lBankerScore;						//ׯ�ҽ��
	LONGLONG						lUserMaxScore;						//�ҵĽ��
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	LONGLONG						lAreaLimitScore;					//��������
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	////�˿���Ϣ
	//BYTE							cbCardCount[2];						//�˿���Ŀ
	//BYTE							cbTableCardArray[2][3];				//�����˿�

	//�����ٶ�
	BYTE							cbHorseSpeed[8][8];
 //
	//ׯ����Ϣ
	LONGLONG							lBankerScore;						//ׯ�ҳɼ�
	LONGLONG							lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONGLONG							lUserScore;							//��ҳɼ�
	LONGLONG							lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONGLONG							lRevenue;							//��Ϸ˰��
};

//���³ɼ�
struct CMD_S_UpdateUserScore
{
	LONGLONG							lUserScore;							//�û��ɼ�
};

//���³ɼ�
struct CMD_S_UpdateBankerScore
{
	LONGLONG							lBankerScore;							//�û��ɼ�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_OPEN_BANK				4									//�������
//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

#endif
