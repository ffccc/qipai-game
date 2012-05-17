#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						500									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("�����")						//��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GS_PLAYING							//��ע״̬
#define	GS_GAME_END					GS_PLAYING+1						//����״̬

//��ʷ��¼
#define MAX_SCORE_HISTORY			50									//��ʷ����

//�������
#define ID_BIG_SHARK				1									//�����
#define ID_FEI_QIN					2									//����
#define ID_ZOU_SHOU					3									//����
#define ID_YAN_ZI					4									//������
#define ID_TU_ZI					5									//������
#define ID_GE_ZI					6									//������
#define ID_XIONG_MAO				7									//��è
#define ID_KONG_QUE					8									//��ȸ
#define ID_HOU_ZI					9									//����
#define ID_LAO_YING					10									//��ӥ
#define ID_SHI_ZI					11									//ʨ��

//��ע�����С
#define SIZE_X						75									//X��
#define SIZE_Y						111									//Y��
#define SIZE_X_2					200									//X��
#define SIZE_Y_2					55									//Y��
#define SIZE_X_SHARK				145									//X��
#define SIZE_Y_SHARK				150									//Y��

//���ʶ���
#define JETTON_AREA_COUNT			11									//�������

//�������ඨ��
#define JETTON_COUNT				6									//��������	

//����λ��							
#define SELECT_TOP					0									//��
#define SELECT_BOTTOM				2									//��
#define SELECT_LEFT					3									//��
#define SELECT_RIGHT				1									//��
//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							cbKingWinner;						//����Ӯ��
	bool							bPlayerTwoPair;						//���ӱ�ʶ
	bool							bBankerTwoPair;						//���ӱ�ʶ
	BYTE							cbPlayerCount;						//�мҵ���
	BYTE							cbBankerCount;						//ׯ�ҵ���
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
#define SUB_S_SCORE_HISTORY			109									//��ʷ��¼
#define SUB_S_CHANGE_SYS_BANKER		110									//�л�ׯ��

//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	BYTE							lJettonArea;						//��ע����
	LONG							lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	DOUBLE							lScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	DOUBLE							lCurrentBankerScore;				//ׯ�ҷ���
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
	LONG							lBankerScore;						//ׯ�ҽ��
};

//�л�ׯ��
struct CMD_S_ChangeSysBanker
{
	bool							bEnableSysBanker;					//ϵͳ��ׯ
	WORD							cbBankerTime;						//ׯ�Ҿ���
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONG							lUserMaxScore;							//��ҽ��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONG							lBankerWinScore;					//ׯ�ҳɼ�
	LONG							lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONG							lApplyBankerCondition;				//��������
	LONG							lAreaLimitScore;					//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	LONG							lALLBigSharkScore;					//�Լ�����������
	LONG							lALLFeiQinScore;					//�Լ�����������
	LONG							lALLZouShouScore;					//�Լ�����������
	LONG							lALLYanZiScore;						//�Լ�����������
	LONG							lALLTuZiScore;						//����
	LONG							lALLGeZiScore;						//����
	LONG							lALLXiongMaoScore;					//��è
	LONG							lALLKongQueScore;					//��ȸ
	LONG							lALLHouZiScore;						//����
	LONG							lALLLaoYingScore;					//��ӥ
	LONG							lALLShiZiScore;						//ʨ��
	//������ע
	LONG							lUserBigSharkScore;					//�Լ�����������
	LONG							lUserFeiQinScore;					//�Լ����������
	LONG							lUserZouShouScore;					//�Լ�����������
	LONG							lUserYanZiScore;					//�Լ�����������
	LONG							lUserTuZiScore;						//����
	LONG							lUserGeZiScore;						//����
	LONG							lUserXiongMaoScore;					//��è
	LONG							lUserKongQueScore;					//��ȸ
	LONG							lUserHouZiScore;					//����
	LONG							lUserLaoYingScore;					//��ӥ
	LONG							lUserShiZiScore;					//ʨ��

	//��һ���
	LONG							lUserMaxScore;						//�����ע							

	//������Ϣ
	LONG							lApplyBankerCondition;				//��������
	LONG							lAreaLimitScore;					//��������

	//������Ϣ
	BYTE							cbMoveEndIndex;						//�ƶ���������
	BYTE							cbResultIndex;						//�������
	BYTE							cbEndIndex;							//ֹͣ����

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONG							lBankerWinScore;					//ׯ��Ӯ��
	LONG							lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONG							lEndBankerScore;					//ׯ�ҳɼ�
	LONG							lEndUserScore;						//��ҳɼ�
	LONG							lEndUserReturnScore;				//���ػ���
	LONG							lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬

	//���״̬
	BYTE							cbUserStatus;						//�û�״̬
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
	LONG							lBankerScore;						//ׯ�ҽ��
	LONG							lUserMaxScore;						//�ҵĽ��
	BYTE							cbTimeLeave;						//ʣ��ʱ��
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
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//������Ϣ
	BYTE							cbMoveEndIndex;						//�ƶ���������
	BYTE							cbResultIndex;						//�������
	BYTE							cbEndIndex;							//ֹͣ����
 
	//ׯ����Ϣ
	LONG							lBankerScore;						//ׯ�ҳɼ�
	LONG							lBankerTotallScore;					//ׯ���ܳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONG							lUserScore;							//��ҳɼ�
	LONG							lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONG							lRevenue;							//��Ϸ˰��

	//�����ע��Ϣ
	LONG							lUserBigSharkScore;					//�Լ�����������
	LONG							lUserFeiQinScore;					//�Լ����������
	LONG							lUserZouShouScore;					//�Լ�����������
	LONG							lUserYanZiScore;					//�Լ�����������
	LONG							lUserTuZiScore;						//����
	LONG							lUserGeZiScore;						//����
	LONG							lUserXiongMaoScore;					//��è
	LONG							lUserKongQueScore;					//��ȸ
	LONG							lUserHouZiScore;					//����
	LONG							lUserLaoYingScore;					//��ӥ
	LONG							lUserShiZiScore;					//ʨ��
};

//��Ϸ����
struct CMD_S_ScoreHistory
{
	BYTE							cbScoreHistroy[MAX_SCORE_HISTORY];					//��ʷ��Ϣ
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONG							lJettonScore;						//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

#endif
