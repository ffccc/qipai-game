#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						14									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("��˫")						//��Ϸ����

//�������
#define ID_ODD						1									//�µ�����
#define ID_EVEN						2									//��˫����

#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ
#define GS_WK_GAME					GS_PLAYING							//��Ϸ״̬
#define GS_WK_END					GS_PLAYING+1						//����״̬

//��¼��Ϣ
struct tagServerGameRecord
{
	WORD							wWinner;							//ʤ�����
	BYTE							cbDicePoint;						//���ӵ���
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_GAME_SCORE			108									//���ͻ���
#define SUB_S_END_PLACE_JETTON		109									//������ע

//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	LONG							lJettonArea;						//��ע����
	LONG							lPlaceScore;						//��ǰ��ע
	LONG							lMaxLimitScore;						//���ƴ�С
	LONG							lFinishPlaceScore;					//����ע��
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	LONG							lScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	LONG							lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	CHAR							szAccount[ 32 ];					//�������
	LONG							lScore;								//��ҽ��
	bool							bApplyBanker;						//�����ʶ
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wChairID;							//���Ӻ���
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	LONG							lBankerScore;						//ׯ�ҷ���
	LONG							lBankerTreasure;					//ׯ�ҽ��

	//�ҵ���ע
	LONG							lAreaLimitScore;					//��������
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//��ע��Ϣ
	LONG							lOddcore;							//����ע
	LONG							lEvenScore;							//��˫��ע

	//�ҵ���ע
	LONG							lMeMaxScore;						//�����ע
	LONG							lMeOddScore;						//����ע
	LONG							lMeEvenScore;						//��˫��ע

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	LONG							lCurrentBankerScore;				//ׯ�ҷ���
	LONG							lApplyBankerCondition;				//��������
	LONG							lAreaLimitScore;					//��������
	LONG							lBankerTreasure;					//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��ע��Ϣ
	LONG							lOddcore;							//����ע
	LONG							lEvenScore;							//��˫��ע

	//�ҵ���ע
	LONG							lMeMaxScore;						//�����ע
	LONG							lMeOddScore;						//����ע
	LONG							lMeEvenScore;						//��˫��ע

	//ɫ����Ϣ
 	BYTE							cbDice[2];							//ɫ�ӵ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	LONG							lCurrentBankerScore;				//ׯ�ҷ���
	LONG							lApplyBankerCondition;				//��������
	LONG							lAreaLimitScore;					//��������
	LONG							lBankerTreasure;					//ׯ�ҽ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
 	BYTE							cbGameTime;							//��Ϸʱ��
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONG							lJettonScore;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	LONG							lMeMaxScore;						//�����ע
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�ɼ���¼
	BYTE							cbWinner;							//ʤ�����
	LONG							lBankerTreasure;					//ׯ�ҽ��

	LONG							lBankerTotalScore;					//ׯ�ҳɼ�
	LONG							lBankerScore;						//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����
};

//��Ϸ�÷�
struct CMD_S_GameScore
{
	//�ɼ���¼
	BYTE							cbWinner;							//ʤ�����
	LONG							lMeGameScore;						//�ҵĳɼ�
	LONG							lMeReturnScore;						//����ע��
	LONG							lBankerScore;						//ׯ�ҳɼ�

	//��ע��Ϣ
	LONG							lDrawOddScore;						//����ע
	LONG							lDrawEvenScore;						//��˫��ע

	//�ҵ���ע
	LONG							lMeOddScore;						//����ע
	LONG							lMeEvenScore;						//��˫��ע
};

//������ע
struct CMD_S_EndPlaceJetton
{
 	BYTE							cbGameTime;							//��Ϸʱ��
	BYTE							cbDice[2];							//ɫ������
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONG							lJettonScore;						//��ע��Ŀ
};

//����ׯ��
struct CMD_C_ApplyBanker
{
	bool							bApplyBanker;						//�����ʶ
};

//////////////////////////////////////////////////////////////////////////

#endif